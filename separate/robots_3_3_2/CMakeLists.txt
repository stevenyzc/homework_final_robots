cmake_minimum_required(VERSION 3.30)
project(robots_3_3_2)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(robots_3_3_2
        main.cpp
        manager.cpp
        manager.h
        robots.h)
