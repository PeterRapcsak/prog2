# lab_08 project
#
# Originally created: 2023-04-26.
#
# CMakeLists.txt --
#   CMake file a lab_09 projekt fordításához.
#   Használat:
#       - parancssor: cmake -B _build -S . && cmake --build _build
#       - CLion/Visual Studio: Megnyitni a mappát projektként
cmake_minimum_required(VERSION 3.18) # Debian Bullseye

include(CheckCXXCompilerFlag)

project(lab_09
        VERSION 1.0.0
        LANGUAGES CXX)

add_executable(generikus2
               generikus2/generikus2_teszt.cpp
               generikus2/fancy_iterators.hpp
               generikus2/sablonok.hpp
               generikus2/gtest_lite.h
               )
add_executable(genArray3
               genArray3/gen_array_iter3_test.cpp
               genArray3/gen_array_iter3.hpp
               genArray3/gtest_lite.h
               genArray3/integer.h
               genArray3/isclass.hpp
               genArray3/sablonok.hpp
               )

target_compile_features(generikus2 PRIVATE cxx_std_11)
target_compile_features(genArray3 PRIVATE cxx_std_11)

set_target_properties(generikus2 PROPERTIES CXX_EXTENSIONS OFF)
set_target_properties(genArray3 PROPERTIES CXX_EXTENSIONS OFF)

target_compile_options(generikus2 PRIVATE
					   $<$<CXX_COMPILER_ID:GNU,Clang,AppleClang>:-Werror>
					   $<$<CXX_COMPILER_ID:MSVC>:/WX>)
target_compile_options(genArray3 PRIVATE
					   $<$<CXX_COMPILER_ID:GNU,Clang,AppleClang>:-Werror>
					   $<$<CXX_COMPILER_ID:MSVC>:/WX>)


# CheckWarningFlag(OptionName CacheName) --
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
        no-reserved-identifier # _Is_... (gtest_lite)
        no-old-style-cast      # gtest_lite
        no-shadow-field-in-constructor # gtest_lite
        no-shorten-64-to-32    # gtest_lite
        no-missing-variable-declarations # generikus2 test globálisok nem statikusak
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

generate_warnings(generikus2)
generate_warnings(genArray3)
