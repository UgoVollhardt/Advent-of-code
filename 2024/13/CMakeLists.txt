cmake_minimum_required(VERSION 3.28)

set(target AoC)
project(${target} LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(${target}
    main.cpp
)