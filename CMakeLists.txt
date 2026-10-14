cmake_minimum_required(VERSION 4.3)
project(CSCI111_ColorVisionProject)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(CSCI111_ColorVisionProject
        main.cpp
        test_zone.cpp
        test_zone.h
        )
