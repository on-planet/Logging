cmake_minimum_required(VERSION 3.16)
project(logging)

set(CMAKE_CXX_STANDARD 20)

add_executable(
        logging
        main.cpp
        FormatItem.cpp
        Formatter.cpp
)
