cmake_minimum_required(VERSION 3.23)
project(Team_complier)

set(CMAKE_CXX_STANDARD 14)

add_executable(Team_complier
        main.cpp
        Scanner.h)

add_executable(main main.cpp)