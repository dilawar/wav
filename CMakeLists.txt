cmake_minimum_required(VERSION 2.6)
project(wavparser)

set(CMAKE_BUILD_TYPE Release )

add_definitions( -std=c++11 )
add_executable(test_wav test_wav.cpp)
