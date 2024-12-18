#include "curl_functions.hpp"
#include "trading_functions.hpp"
#include <iostream>
#include <thread>
#include <chrono>
#include <stdexcept>
#include <limits>

int main() {
    std::string ticker = "NEM";
    std::string api_key = "53b9d9dceca74b6ab054bab41838d8ec";

    while (true) {
        try {
            std::string price_str = get_price(ticker, api_key);
            if (price_str.empty()) {
                std::cerr << "Failed to fetch price." << std::endl;
                std::this_thread::sleep_for(std::chrono::seconds(5));
                continue;
            }

            float price = std::stof(price_str);
            Json::Value stock_data = get_stock_quote(ticker, api_key);

            if (!stock_data.empty()) {
                print_data(stock_data, price);

                float open, close, high, low;
                try {
                    open = stock_data["open"].isString() ? std::stof(stock_data["open"].asString()) : std::numeric_limits<float>::quiet_NaN();
                    close = stock_data["close"].isString() ? std::stof(stock_data["close"].asString()) : std::numeric_limits<float>::quiet_NaN();
                    high = stock_data["high"].isString() ? std::stof(stock_data["high"].asString()) : std::numeric_limits<float>::quiet_NaN();
                    low = stock_data["low"].isString() ? std::stof(stock_data["low"].asString()) : std::numeric_limits<float>::quiet_NaN();
                } catch (const std::invalid_argument& e) {
                    std::cerr << "Error: Invalid numeric value in stock data." << std::endl;
                    continue;
                } catch (const std::out_of_range& e) {
                    std::cerr << "Error: Numeric value out of range in stock data." << std::endl;
                    continue;
                }

                std::string name = stock_data["name"].isString() ? stock_data["name"].asString() : "Unknown";

                calculate_profit_loss(open, close, name);
                send_signal(price, high, low);
            } else {
                std::cerr << "Failed to fetch stock quote." << std::endl;
            }
        } catch (const std::exception& e) {
            std::cerr << "An error occurred: " << e.what() << std::endl;
        }

        std::this_thread::sleep_for(std::chrono::seconds(3));
    }

    return 0;
}
