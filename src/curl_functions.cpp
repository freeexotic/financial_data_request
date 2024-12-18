#include "curl_functions.hpp"
#include <curl/curl.h>
#include <sstream>
#include <stdexcept>

static size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* output) {
    size_t total_size = size * nmemb;
    output->append(static_cast<char*>(contents), total_size);
    return total_size;
}

std::string get_price(const std::string& ticker_symbol, const std::string& api_key) {
    CURL* curl = curl_easy_init();
    // Инициализация библиотеки cURL для выполнения HTTP-запросов.
    std::string response;
    // Объявление переменной response для хранения ответа от API.
    if (!curl) {
        throw std::runtime_error("Failed to initialize CURL");
    }
    std::string url = "https://api.twelvedata.com/price?symbol=" + ticker_symbol + "&apikey=" + api_key;
    // Формирование URL для запроса к API Twelve Data, включая символ тикера и API-ключ.
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    // Эта строка устанавливает URL для запроса cURL.
    //     curl - указатель на объект CURL, инициализированный ранее.
    //     CURLOPT_URL - опция cURL для установки URL.
    //     url.c_str() - преобразует C++ строку url в C-строку, которую ожидает cURL.
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    // Здесь устанавливается функция обратного вызова для обработки полученных данных.
    //     CURLOPT_WRITEFUNCTION - опция для указания функции, которая будет вызываться для записи полученных данных.
    //     WriteCallback - пользовательская функция, которая будет обрабатывать полученные данные.
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
    // Эта строка устанавливает пользовательские данные для функции обратного вызова.
    //     CURLOPT_WRITEDATA - опция для передачи пользовательских данных в функцию обратного вызова.
    //     &response - адрес переменной response, куда будут записываться полученные данные.
    curl_easy_setopt(curl, CURLOPT_FAILONERROR, 1L);
    // Эта опция указывает cURL считать HTTP-ответы с кодом 400 и выше ошибками.
    //     CURLOPT_FAILONERROR - опция для обработки HTTP-ошибок.
    //     1L - значение типа long, устанавливающее эту опцию в true.
    CURLcode res = curl_easy_perform(curl);
    // Выполнение HTTP-запроса.

    curl_easy_cleanup(curl);
    // Освобождение ресурсов, выделенных для cURL.

    if (res != CURLE_OK) {
        throw std::runtime_error("CURL request failed: " + std::string(curl_easy_strerror(res)));
    }
    // Проверка успешности выполнения запроса. Если произошла ошибка, выбрасывается исключение с описанием ошибки.

    Json::Value root;
    // Этот объект представляет собой корневой элемент JSON-структуры, который будет использоваться для хранения данных после их парсинга.
    Json::CharReaderBuilder reader;
    // Создается объект типа Json::CharReaderBuilder, который используется для создания парсера JSON.
    // Это стандартный способ в jsoncpp для настройки и инициализации парсинга JSON-данных.

    std::istringstream response_stream(response);
    // Создается поток данных (std::istringstream) на основе строки response.
    // Переменная response содержит текст ответа от API (в формате JSON).

    std::string errs;
    // Создается строка errs, которая будет использоваться для хранения сообщений об ошибках, возникающих во время парсинга JSON.

    if (!Json::parseFromStream(reader, response_stream, &root, &errs)) {
        throw std::runtime_error("Failed to parse JSON: " + errs);
    }
    if (root.isMember("status") && root["status"] == "error") {
        throw std::runtime_error("API error: " + root["message"].asString());
    }
    if (!root.isMember("price") || !root["price"].isString()) {
        throw std::runtime_error("Invalid JSON structure: 'price' field not found or not a string");
    }
    return root["price"].asString();
    // Возврат значения цены в виде строки.
}

Json::Value get_stock_quote(const std::string& ticker_symbol, const std::string& api_key) {
    CURL* curl = curl_easy_init();
    std::string response;
    if (!curl) {
        throw std::runtime_error("Failed to initialize CURL");
    }
    std::string url = "https://api.twelvedata.com/quote?symbol=" + ticker_symbol + "&apikey=" + api_key;
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
    curl_easy_setopt(curl, CURLOPT_FAILONERROR, 1L);
    CURLcode res = curl_easy_perform(curl);
    curl_easy_cleanup(curl);
    if (res != CURLE_OK) {
        throw std::runtime_error("CURL request failed: " + std::string(curl_easy_strerror(res)));
    }
    Json::Value root;
    Json::CharReaderBuilder reader;
    std::istringstream response_stream(response);
    std::string errs;
    if (!Json::parseFromStream(reader, response_stream, &root, &errs)) {
        throw std::runtime_error("Failed to parse JSON: " + errs);
    }
    return root;
}
