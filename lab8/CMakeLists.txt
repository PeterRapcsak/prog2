# lab_08 project
#
# Originally created: 2023-04-13.
#
# CMakeLists.txt --
#   CMake file a lab_08 projekt fordításához.
#   Használat:
#       - parancssor: cmake -B _build -S . && cmake --build _build
#       - CLion/Visual Studio: Megnyitni a mappát projektként
cmake_minimum_required(VERSION 3.18) # Debian Bullseye

include(CheckCXXCompilerFlag)

project(lab_08
        VERSION 1.0.0
        LANGUAGES CXX)

add_executable(generikus1
               generikus1/generikus1_teszt.cpp
               generikus1/integer.h
               )
add_executable(halmaz
               halmaz/gtest_lite.h
               halmaz/halmaz_teszt.cpp
               halmaz/integer.h
               halmaz/point.h
               )

target_compile_features(generikus1 PRIVATE cxx_std_98)
target_compile_features(halmaz PRIVATE cxx_std_98)

set_target_properties(generikus1 PROPERTIES CXX_EXTENSIONS OFF)
set_target_properties(halmaz PROPERTIES CXX_EXTENSIONS OFF)

target_compile_options(generikus1 PRIVATE
					   $<$<CXX_COMPILER_ID:GNU,Clang,AppleClang>:-Werror>
					   $<$<CXX_COMPILER_ID:MSVC>:/WX>)
target_compile_options(halmaz PRIVATE
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
        no-missing-prototypes # kiír nincs előre deklarálva
        no-deprecated-copy-with-user-provided-dtor # Integer dtor miatt már nincs copy ctora
        no-c++98-compat-pedantic # variadic macro
        no-reserved-identifier # _Is_...
        no-old-style-cast      # gtest_lite
        no-shadow-field-in-constructor # gtest_lite
        no-shorten-64-to-32    # gtest_lite
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

generate_warnings(generikus1)
generate_warnings(halmaz)
