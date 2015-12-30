cmake_minimum_required(VERSION 3.3)
project(lurun)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")

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
        src/lua/string_object.cpp
        src/lua/string_object.h
        src/lua/lua.h
        src/lua/instruction_list.cpp
        src/lua/instruction_list.h
        src/lua/value.cpp
        src/lua/value.h
        src/lua/container.h
        src/lua/upvalue.cpp
        src/lua/upvalue.h
        src/vm/vm.cpp
        src/vm/vm.h
        src/vm/vm_common.h
        src/lua/table.cpp
        src/lua/table.h
        src/vm/closure.cpp
        src/vm/closure.h
        src/vm/call_frame.h
        src/lua/native.cpp
        src/lua/native.h
        src/vm/call_frame.cpp
        src/vm/base_enviroment.h
        src/vm/base_enviroment.cpp
        src/vm/heap.cpp
        src/vm/heap.h)

add_executable(lurun ${SOURCE_FILES})