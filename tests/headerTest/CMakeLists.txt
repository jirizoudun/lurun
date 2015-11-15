cmake_minimum_required(VERSION 3.3)
project(LuaHeaderChecker)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp
                 header.cpp)
add_executable(LuaHeaderChecker ${SOURCE_FILES})
