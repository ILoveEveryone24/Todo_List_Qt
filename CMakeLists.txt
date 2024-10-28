cmake_minimum_required(VERSION 3.16)

project(program LANGUAGES CXX)

find_package(Qt6 REQUIRED COMPONENTS Core Widgets)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

add_executable(program
    src/main.cpp
)

target_include_directories(program PRIVATE include)

target_link_libraries(program Qt6::Core Qt6::Widgets)
