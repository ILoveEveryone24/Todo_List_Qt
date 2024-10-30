cmake_minimum_required(VERSION 3.16)

project(todo_list LANGUAGES CXX)

find_package(Qt6 REQUIRED COMPONENTS Core Widgets)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

add_executable(todo_list
    src/main.cpp
)

target_include_directories(todo_list PRIVATE include)

target_link_libraries(todo_list Qt6::Core Qt6::Widgets)
