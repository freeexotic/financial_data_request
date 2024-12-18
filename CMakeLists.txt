cmake_minimum_required(VERSION 3.10)
project(trading_app)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Находим необходимые пакеты
find_package(CURL REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_check_modules(JSONCPP jsoncpp REQUIRED)

# Добавляем исполняемый файл
add_executable(trading_app
    src/main.cpp
    src/curl_functions.cpp
    src/trading_functions.cpp
    README.md
)

# Добавляем пути к заголовочным файлам
target_include_directories(trading_app PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${JSONCPP_INCLUDE_DIRS}
)

# Линкуем библиотеки
target_link_libraries(trading_app PRIVATE
    CURL::libcurl
    ${JSONCPP_LIBRARIES}
)

# Копируем config.json в директорию сборки
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/config.json
               ${CMAKE_CURRENT_BINARY_DIR}/config.json COPYONLY)

