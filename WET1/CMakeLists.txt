cmake_minimum_required(VERSION 3.9)
project(WET1)

set(CMAKE_CXX_STANDARD 11)

add_executable(WET1 library1_tests.cpp dict_list.h dict_avl.h library1_list.cpp)