cmake_minimum_required(VERSION 3.22)
project(kostya_bot)

set(CMAKE_CXX_STANDARD 14)
#add_compile_options(-std=c++11)

find_package(tgbot)
#find_package(libmysqlclient-dev)
#find_package(mariadb-connector-c)

include_directories(/usr/include/mysql)
link_directories("/usr/lib/x86_64-linux-gnu/libmysqlclient.so")
#link_directories("/usr/lib/x86_64-linux-gnu/libmysqlcppconn-static.a"  )
link_directories("/usr/lib/x86_64-linux-gnu/libmysqlclient.a")

add_executable(kostya_bot src/main.cpp src/lib/CommandRecorder.h src/lib/CommandRecorder.cpp src/lib/KeyboardGenerator.h src/lib/KeyboardGenerator.cpp src/driver/DBHandler.cpp src/driver/DBHandler.h src/driver/StringParser.cpp src/driver/StringParser.h src/driver/StringBuilder.cpp src/driver/StringBuilder.h src/lib/MessageRecorder.cpp src/lib/MessageRecorder.h src/lib/CallbackQueryRecorder.cpp src/lib/CallbackQueryRecorder.h src/lib/my_lib.h src/lib/my_lib.cpp src/driver/strings.h src/driver/strings.cpp src/driver/StringBuilderDB.cpp src/driver/StringBuilderDB.h)

target_link_libraries(kostya_bot libmysqlclient.so)
#target_link_libraries(${PROJECT_NAME} mariadb-connector-c::mariadb-connector-c)
target_link_libraries(${PROJECT_NAME} tgbot::tgbot)