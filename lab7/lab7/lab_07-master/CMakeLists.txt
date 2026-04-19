# lab_07 project
#
# Originally created: 2023-04-13.
#
# CMakeLists.txt --
#   CMake file a lab_07 projekt fordításához.
#   Használat:
#       - parancssor: cmake -B _build -S . && cmake --build _build
#       - CLion/Visual Studio: Megnyitni a mappát projektként
cmake_minimum_required(VERSION 3.18) # Debian Bullseye

include(CheckCXXCompilerFlag)

project(lab_07
        VERSION 1.0.0
        LANGUAGES CXX)

add_library(memtrace STATIC
            Alakzatlab/memtrace.cpp Alakzatlab/memtrace.h)
target_compile_features(memtrace PRIVATE cxx_std_98)
target_compile_definitions(memtrace PUBLIC MEMTRACE)

add_library(alakzat-common OBJECT
            Alakzatlab/alakzat.cpp
            Alakzatlab/alakzat.h
            Alakzatlab/ellipszis.h
            Alakzatlab/haromszog.cpp
            Alakzatlab/haromszog.h
            Alakzatlab/kor.cpp
            Alakzatlab/kor.h
            Alakzatlab/pont.cpp
            Alakzatlab/pont.h
            Alakzatlab/szakasz.cpp
            Alakzatlab/szakasz.h
            Alakzatlab/szin.cpp
            Alakzatlab/szin.h
            )
target_link_libraries(alakzat-common PUBLIC memtrace)

add_executable(alakzat
               Alakzatlab/alakzat_main.cpp
               )
add_executable(alakzat_rajztabla
               Alakzatlab/alakzat2_main.cpp
               Alakzatlab/rajztabla.cpp
               Alakzatlab/rajztabla.h
               )

target_link_libraries(alakzat PRIVATE alakzat-common)
target_compile_features(alakzat PRIVATE cxx_std_98)
set_target_properties(alakzat PROPERTIES
                      CXX_EXTENSIONS OFF)
target_compile_options(alakzat PRIVATE
					   $<$<CXX_COMPILER_ID:GNU,Clang,AppleClang>:-Werror>
					   $<$<CXX_COMPILER_ID:MSVC>:/WX>)

target_link_libraries(alakzat_rajztabla PRIVATE alakzat-common)
target_compile_features(alakzat_rajztabla PRIVATE cxx_std_98)
set_target_properties(alakzat_rajztabla PROPERTIES
                      CXX_EXTENSIONS OFF)
target_compile_options(alakzat_rajztabla PRIVATE
					   $<$<CXX_COMPILER_ID:GNU,Clang,AppleClang>:-Werror>
					   $<$<CXX_COMPILER_ID:MSVC>:/WX>)

## CheckWarningFlag(OptionName CacheName) --
# 	Ellenőrzi, hogy az használt fordító érti-e a kapott parancssori kapcsolót.
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
# 	Beállít hibakapcsolókat a kapott target-en.
function(generate_warnings _Target)
    set(gw_known_warnings
        # GCC/Clang
        extra pedantic error=vla error=non-virtual-dtor
        parentheses logical-op reorder no-c++98-compat
        no-reserved-macro-identifier no-unused-macros no-float-equal
        no-global-constructors no-exit-time-destructors no-unsafe-buffer-usage
        no-shadow-field-in-constructor no-suggest-override no-old-style-cast
        # MSVC
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

generate_warnings(alakzat)
generate_warnings(alakzat_rajztabla)

