cmake_minimum_required(VERSION 3.16)
project(logging_project)

set(CMAKE_CXX_STANDARD 20)

add_library(
        logging_lib STATIC
        src/FormatItem.cpp
        src/Formatter.cpp
        src/LogSink.cpp
)

target_include_directories(logging_lib PUBLIC include)

# 查找并链接 pthreads 库
find_package(Threads REQUIRED)
target_link_libraries(logging_lib PRIVATE Threads::Threads)

add_executable(
        logging_app
        example/main.cpp
)

target_link_libraries(logging_app PRIVATE logging_lib)