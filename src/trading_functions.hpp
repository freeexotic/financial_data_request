#pragma once
#include <string>
#include <json/json.h>

void print_data(const Json::Value& stock_data, float price);
void calculate_profit_loss(float open, float close, const std::string& name);
void send_signal(float price, float high, float low);
