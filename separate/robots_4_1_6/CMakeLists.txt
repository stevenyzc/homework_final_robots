cmake_minimum_required(VERSION 3.30)
project(robots_4_1_6)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(robots_4_1_6
        main.cpp
        manager.cpp
        manager.h
        robots.h)
