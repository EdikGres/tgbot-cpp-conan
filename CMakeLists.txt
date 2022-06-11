cmake_minimum_required(VERSION 3.22)
project(kostya_bot)

set(CMAKE_CXX_STANDARD 14)

find_package(tgbot)

add_executable(kostya_bot src/main.cpp)

target_link_libraries(${PROJECT_NAME} tgbot::tgbot)