# Lab02 project
#
# Originally created: 2023-02-27.
#
# CMakeLists.txt --
#   Elsődleges CMake file a lab_02 projekt fordításához.
#   Fontos!
#     Ez a fájl egy összesített script a két alprojekt kezeléséhez.
#     Direkt egyesével is használhatóak, amennyiben a megfelelő
#     almappát nyitjuk meg projektként.
#     Ha ezt használjuk, akkor figyeljünk, mit próbálunk fordítani/futtatni!
#     Például CLion-ban a futtatás gomb mellett megfelelő program van
#     kiválasztva; VS-ben pedig megfelelő projektet probálunk elindítani.
#   Használat:
#       - parancssor: cmake -B _build -S . && cmake --build _build
#       - CLion/Visual Studio: Megnyitni a mappát projektként
#       - HSZK Visual Studio: HSZK-VS17 mappában előre legenerált projekt elérhető
cmake_minimum_required(VERSION 3.18) # Debian Bullseye

project(cpp_lab_02
        HOMEPAGE_URL "https://git.ik.bme.hu/Prog2/labor_peldak/lab_02"
        VERSION 1.0.0
        LANGUAGES C CXX)

add_subdirectory(string1)
add_subdirectory(string2_cpp)

#  A szóban megoldandó feladatok ellenőrzésére, ezeket
#  nem érdemes fordítani, mert nem helyesek, ha valaki
#  szeretné, az alábbi sorok kikommenzezésével lehet őket
#  ismertetni CMake-el. Így meg lehet nézni, konkrétan
#  mit ír ki a használt fordító a hibákra, esetleg 
#  önellenőrzésre
#add_executable(szoveges_feldat_const const_h.cpp)
#add_executable(szoveges_feldat_ref ref_h.cpp)
