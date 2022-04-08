cmake_minimum_required(VERSION 3.21)
project(Demo_Authentication)

set(CMAKE_CXX_STANDARD 14)

add_executable(Demo_Authentication
        Admin_class.cpp
        Auth_class.cpp
        H_S_F_class.cpp
        test.cpp)
