# Lab02 project
# string2 target
#
# Originally created: 2023-02-27.
#
# CMakeLists.txt --
#   CMake file a lab_02 projekt string2 céljának fordításához.
#   Használat:
#       - parancssor: cmake -B _build -S . && cmake --build _build
#       - CLion/Visual Studio: Megnyitni a mappát projektként
#       - Code::Blocks:
#           1) cmake -G "CodeBlocks - <1>" -B _build -S .
#               - Windows: <1> = MinGW Makefiles
#               - Unix: <2> = Unix Makefiles
#           2) A _build/*.cbp fájlt megnyitni

cmake_minimum_required(VERSION 3.18) # Debian Bullseye

include(CheckCXXCompilerFlag)

project(cpp_lab_02_str2
        HOMEPAGE_URL "https://git.ik.bme.hu/Prog2/labor_peldak/lab_02"
        VERSION 1.0.0
        LANGUAGES CXX)

add_library(memtrace_cxx STATIC
            memtrace.cpp memtrace.h)
target_compile_features(memtrace_cxx PRIVATE cxx_std_98)
target_compile_definitions(memtrace_cxx PUBLIC MEMTRACE)

add_executable(lab_02_string2
               gtest_lite.h
               string2.cpp
               string2.h
               string2_main.cpp)

target_compile_features(lab_02_string2 PRIVATE cxx_std_98)
set_target_properties(lab_02_string2 PROPERTIES
                      CXX_EXTENSIONS OFF)
target_link_libraries(lab_02_string2 PRIVATE memtrace_cxx)
target_compile_options(lab_02_string2 PRIVATE
                       $<$<CXX_COMPILER_ID:GNU,Clang,AppleClang>:-Werror>
                       $<$<CXX_COMPILER_ID:MSVC>:/WX>)

## CheckWarningFlag(OptionName CacheName) --
#     Ellenőrzi, hogy az használt fordító érti-e a kapott parancssori kapcsolót.
function(check_warning_flag OptionName CacheName)
    if (OptionName MATCHES [[^/]])
        set(WarningPrefix "")
    else ()
        set(WarningPrefix "-W")
    endif ()
    check_cxx_compiler_flag("${WarningPrefix}${OptionName}" "HasWarning_${CacheName}")
    set("HAS_WARNING_${CacheName}" ${HasWarning_${CacheName}} PARENT_SCOPE)
endfunction()

## generate_warnings(&Target) --
#     Beállít hibakapcsolókat a kapott target-en.
function(generate_warnings _Target)
    set(gw_known_warnings
        # GCC/Clang
        extra pedantic error=vla error=non-virtual-dtor
        parentheses logical-op reorder no-c++98-compat
        no-reserved-macro-identifier no-unused-macros no-float-equal
        no-zero-as-null-pointer-constant no-global-constructors
        no-exit-time-destructors no-unsafe-buffer-usage
        no-reserved-identifier # _Is_...
        no-old-style-cast      # gtest_lite
        # MSVC
        # conditional expression is constant (nem szereti az ELKESZULT) markot
        /wd4127
        # 'function': member function does not override any base class virtual member function
        /we4263 
        # 'class': class has virtual functions, but destructor is not virtual
        /we4265 
        # 'identifier': illegal qualified name in member declaration
        /we4596 
        # 'symbol': exception specification does not match previous declaration
        /we4986 
        4
        /permissive-
        /diagnostics:caret)
    # MSVC /Wall == clang -Weverything, arra nincs szükségünk
    set(gw_found_warnings $<$<CXX_COMPILER_ID:GNU,Clang,AppleClang>:-Wall>)

    foreach (warn IN LISTS gw_known_warnings)
        string(MAKE_C_IDENTIFIER "${warn}" CacheName)
        check_warning_flag("${warn}" ${CacheName})
        if (HAS_WARNING_${CacheName})
            if (warn MATCHES [[^/]])
                set(WarningPrefix "")
            else ()
                set(WarningPrefix "-W")
            endif ()
            list(APPEND gw_found_warnings "${WarningPrefix}${warn}")
        endif ()
    endforeach ()

    target_compile_options("${_Target}" PUBLIC ${gw_found_warnings})
endfunction()

generate_warnings(lab_02_string2)
