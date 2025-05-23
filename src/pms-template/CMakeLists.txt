cmake_minimum_required(VERSION 3.31)
project(pms_template)

set(CMAKE_CXX_STANDARD 20)

add_executable(pms_template main.cpp
        src/utils.cpp
        include/utils.h
        src/data_structures.cpp
        include/data_structures.h
        src/parking_lot.cpp
        include/parking_lot.h
        src/parking_network.cpp
        include/parking_network.h)
