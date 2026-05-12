/*======================================================================
    main.cpp - Főprogram
----------------------------------------------------------------------
    FELADAT:
     - Game objektum létrehozása
     - Kérdések betöltése CSV fileokból
     - Főmenü (loop) indítása

    //! Fordítási parancs:
    //! g++ -std=c++11 -Wall -Wextra -o nhf2_main main.cpp game.cpp highscore.cpp question.cpp filemanager.cpp
======================================================================*/

#include "game.h"

// CÉL: CSV filenevek
const char* FILE_CHOOSE = "src/kerdesek.csv";    // feleletválasztós kérdések
const char* FILE_ORDER  = "src/sorkerdesek.csv"; // sorrendezős kérdések

// CÉL: Program belépési pont
int main() {
    Game game; // konstruktor betölti a dicsőséglistát

    game.loadQuestions(FILE_CHOOSE, FILE_ORDER); // CSV fileok betöltése kérdéstömbökbe
    game.mainMenu(); // főmenü ciklus indítása

    return 0;
}
