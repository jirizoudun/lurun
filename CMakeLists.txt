cmake_minimum_required(VERSION 3.3)
project(lurun)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
        src/header.cpp
        src/header.h
        src/main.cpp
        .gitignore
        LICENSE
        README.md
        src/reader.cpp
        src/reader.h
        src/common.h
        src/lua/function.cpp
        src/lua/function.h
        src/lua/instruction.cpp
        src/lua/instruction.h
        src/lua/string.cpp
        src/lua/string.h
        src/lua/lua.h
        src/lua/instruction_list.cpp
        src/lua/instruction_list.h
        src/lua/value.cpp
        src/lua/value.h
        src/lua/container.cpp
        src/lua/container.h src/lua/upvalue.cpp src/lua/upvalue.h)

add_executable(lurun ${SOURCE_FILES})