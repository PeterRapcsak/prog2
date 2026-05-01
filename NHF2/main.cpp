/*======================================================================
    main.cpp - Főprogram
----------------------------------------------------------------------
    CÉL:
     - Game objektum létrehozása
     - Kérdések betöltése CSV fileokból
     - Főmenü indítása
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
