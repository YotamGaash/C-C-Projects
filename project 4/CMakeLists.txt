cmake_minimum_required(VERSION 3.22)
project(ex4_gaash)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(ex4_gaash
        Activation.h
        Dense.h
        main.cpp
        Matrix.h
        MlpNetwork.h Activation.cpp Dense.cpp MlpNetwork.cpp Matrix.cpp)
