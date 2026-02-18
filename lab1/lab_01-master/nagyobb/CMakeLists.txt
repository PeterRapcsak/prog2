# nagyobb project
#
# CMakeLists.txt --
#   Elsődleges CMake file a nagyobb projekt fordításához.
#   Használat:
#       - parancssor: cmake -B _build -S .
#       - CLion/Visual Studio: Megnyitni a mappát projektként
#       - HSZK Visual Studio: az eddig is létező .sln fájlt érdemes használni
cmake_minimum_required(VERSION 3.18) # Debian Bullseye

include(CheckCXXCompilerFlag)

project(cpp_labor01
        HOMEPAGE_URL "https://git.ik.bme.hu/Prog2/labor_peldak/lab_01"
        VERSION 1.0.0
        LANGUAGES CXX)

add_executable(nagyobb
               nagyobb_main.cpp
               fuggvenyeim.cpp)

target_compile_features(nagyobb PRIVATE cxx_std_98)
set_target_properties(nagyobb PROPERTIES
                      CXX_EXTENSIONS OFF)
target_compile_options(nagyobb PRIVATE
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
        parentheses logical-op reorder c++98-compat
        no-reserved-macro-identifier no-unused-macros no-float-equal
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

generate_warnings(nagyobb)
