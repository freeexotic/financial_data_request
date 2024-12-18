#pragma once
#include <string>
#include <json/json.h>

std::string get_price(const std::string& ticker_symbol, const std::string& api_key);
Json::Value get_stock_quote(const std::string& ticker_symbol, const std::string& api_key);
