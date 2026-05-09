/*======================================================================
    main.cpp - Főprogram
----------------------------------------------------------------------
    CÉL:
     - Game objektum létrehozása
     - Kérdések betöltése CSV fileokból
     - Főmenü (loop) indítása
    
    Fordító parancs: 
    PATH = az elérése az adott foldernek.
    
    g++ -Wall -Wextra -g3 \
    "PATH/main.cpp" \
    "PATH/question.cpp" \
    "PATH/filemanager.cpp" \
    "PATH/game.cpp" \
    "PATH/highscore.cpp" \
    -o "PATH/output/main"
======================================================================*/

#include "game.h"

// CÉL: CSV file nevek konstansként (könnyű átírni)
const char* FILE_CHOOSE = "kerdesek.csv";
const char* FILE_ORDER  = "sorkerdesek.csv";

/*
    CÉL: Program belépési pont
     - Játék példány létrehozása
     - CSV kérdések betöltése
     - Főmenü futtatása
*/
int main() {
    Game game;

    game.loadQuestions(FILE_CHOOSE, FILE_ORDER); // CSV fileok betöltése
    game.mainMenu();                              // Főmenü indítása

    return 0;
}
