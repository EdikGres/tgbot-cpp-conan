cmake_minimum_required(VERSION 3.22)
project(kostya_bot)

set(CMAKE_CXX_STANDARD 14)

find_package(tgbot)

add_executable(kostya_bot src/main.cpp src/lib/CommandRecorder.h src/lib/CommandRecorder.cpp src/lib/KeyboardGenerator.h src/lib/KeyboardGenerator.cpp)

target_link_libraries(${PROJECT_NAME} tgbot::tgbot)