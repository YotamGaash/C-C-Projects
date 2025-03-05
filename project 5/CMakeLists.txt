cmake_minimum_required(VERSION 3.22)
project(ex5_gaash)

set(CMAKE_CXX_STANDARD 14)

include_directories(. , io)
add_executable(ex5_gaash
        Movie.cpp
        Movie.h
        print_main.cpp
        RecommendationSystem.h
        RecommendationSystemLoader.cpp
        RecommendationSystemLoader.h
        User.cpp
        User.h
        UsersLoader.cpp
        RecommendationSystemLoader_input.txt
        UsersLoader_input.txt
        presubmit.in_m
        presubmit.in_m4
        presubmit.in_m5
        presubmit.in_m5
        presubmit.in_m6
        presubmit.in_m7
        presubmit.in_m15
        presubmit.in_m16
        presubmit.in_u
        presubmit.in_u4
        presubmit.in_u5
        presubmit.in_u6
        presubmit.in_u7
        presubmit.in_u14
        presubmit.in_u16
        UsersLoader.h RecommendationSystem.cpp RecommendationSystem.cpp presubmit.cpp)
