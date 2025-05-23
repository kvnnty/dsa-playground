cmake_minimum_required(VERSION 3.31)
project(dsa_playground)

set(CMAKE_CXX_STANDARD 20)

add_executable(dsa_playground main.cpp
        src/graph.cpp
        include/graph.h
        src/hospital.cpp
        include/hospital.h
        include/models.h
        src/utility.cpp
        include/utility.h)
