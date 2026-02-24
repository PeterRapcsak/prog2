# Lab02 project
# string1 target
#
# Originally created: 2023-03-06.
#
# CMakeLists.txt --
#   CMake file a lab_02 projekt string1 céljának fordításához.
#   Használat:
#       - parancssor: cmake -B _build -S . && cmake --build _build
#       - CLion/Visual Studio: Megnyitni a mappát projektként
cmake_minimum_required(VERSION 3.18) # Debian Bullseye

project(cpp_lab_02_str1
        HOMEPAGE_URL "https://git.ik.bme.hu/Prog2/labor_peldak/lab_02"
        VERSION 1.0.0
        LANGUAGES C)

add_library(memtrace_c STATIC
            memtrace.c memtrace.h)
target_compile_features(memtrace_c PRIVATE c_std_99)
target_compile_definitions(memtrace_c PUBLIC MEMTRACE
                           _CRT_SECURE_NO_WARNINGS)

add_executable(lab_02_string1
               string1.c
               string1.h
               string1_main.c
               gtest_lite.h)

target_compile_features(lab_02_string1 PRIVATE c_std_99)
set_target_properties(lab_02_string1 PROPERTIES
                      CXX_EXTENSIONS OFF)
target_link_libraries(lab_02_string1 PRIVATE memtrace_c)
# A C fordításra nincs teljes generate_warnings, csak az egyszerű konstans 
# kapcsolókat használjuk.
target_compile_options(lab_02_string1 PRIVATE
                       $<$<CXX_COMPILER_ID:GNU,Clang,AppleClang>:-Wall -Wextra -Werror>
                       $<$<CXX_COMPILER_ID:Clang>:-Wno-reserved-macro-identifier
                                                  -Wno-unused-macros -Wno-format
                                                  -Wno-strict-prototypes
                                                  -Wno-shorten-64-to-32
                                                  -Wno-unsafe-buffer-usage
                                                  -Wno-declaration-after-statement>
                       $<$<CXX_COMPILER_ID:MSVC>:/W4 /WX /wd4267>)

