#include "trading_functions.hpp"
#include <iostream>
#include <iomanip>
#include <cmath>
#include <limits>

void print_data(const Json::Value& stock_data, float price) {
    std::cout << "\nGenerating Data....\n\n";
    std::cout << std::fixed << std::setprecision(2);

    try {
        std::cout << "Name: " << stock_data.get("name", "N/A").asString() << "\n"
                  << "Exchange: " << stock_data.get("exchange", "N/A").asString() << "\n"
                  << "Currency: " << stock_data.get("currency", "N/A").asString() << "\n"
                  << "Open price: " << stock_data.get("open", "N/A").asString() << "\n"
                  << "High price: " << stock_data.get("high", "N/A").asString() << "\n"
                  << "Low price: " << stock_data.get("low", "N/A").asString() << "\n"
                  << "Close price: " << stock_data.get("close", "N/A").asString() << "\n"
                  << "Volume: " << stock_data.get("volume", "N/A").asString() << "\n"
                  << "Change: " << stock_data.get("change", "N/A").asString() << "\n"
                  << "Current Price: " << price << "\n";
    } catch (const Json::LogicError& e) {
        std::cerr << "Error parsing JSON data: " << e.what() << std::endl;
    }
}

void calculate_profit_loss(float open, float close, const std::string& name) {
    const float epsilon = std::numeric_limits<float>::epsilon();
    if (std::isnan(open) || std::isnan(close) || std::abs(open) < epsilon) {
        std::cerr << "Error: Invalid open or close price. Cannot calculate profit/loss." << std::endl;
        return;
    }
    float percentage_change = ((close - open) / open) * 100;
    std::cout << std::fixed << std::setprecision(2);

    if (percentage_change > epsilon) {
        std::cout << "\nThe " << name << " stock is in profit of " << percentage_change << "%\n";
    } else if (percentage_change < -epsilon) {
        std::cout << "\nThe " << name << " stock is in loss of " << std::abs(percentage_change) << "%\n";
    } else {
        std::cout << "\nThe " << name << " stock is in a neutral state\n";
    }
}

void send_signal(float price, float high, float low) {
    const float epsilon = std::numeric_limits<float>::epsilon();
    if (std::isnan(high) || std::isnan(low) || std::abs(high - low) < epsilon) {
        std::cerr << "Error: Invalid high or low price." << std::endl;
        return;
    }
    if (price <= low) {
        std::cout << "Sending Buying signal\n\n";
    } else if (price >= high) {
        std::cout << "Sending Selling signal\n\n";
    } else {
        std::cout << "Not suitable for buying and selling\nHappy Trading!!!\n\n";
    }
}
