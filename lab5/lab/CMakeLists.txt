cmake_minimum_required(VERSION 3.11)

project(lab_05)

set(CMAKE_CXX_STANDARD 11)

if (NOT MSVC)
    add_compile_options(-pedantic -Wall -Werror)
endif()

add_executable(string5
    memtrace.cpp
    rendez.cpp
    string5.cpp
    string5_test.cpp
)

target_compile_definitions(string5 PRIVATE MEMTRACE)
if (MSVC)
    # ignore comparison of int and size_t
    target_compile_options(string5 PRIVATE /wd4267)
endif()
