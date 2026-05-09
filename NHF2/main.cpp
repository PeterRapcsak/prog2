/*======================================================================
    main.cpp - Főprogram
----------------------------------------------------------------------
    FELADAT:
     - Game objektum létrehozása
     - Kérdések betöltése CSV fileokból
     - Főmenü (loop) indítása

    Fordítási parancs (PATH = projekt könyvtár):
    g++ -std=c++11 -Wall -Wextra \
        PATH/main.cpp PATH/question.cpp PATH/filemanager.cpp \
        PATH/game.cpp PATH/highscore.cpp \
        -o PATH/nhf2_main
======================================================================*/

#include "game.h"

// CÉL: CSV filenames konstansként – ha változnak, csak itt kell átírni
const char* FILE_CHOOSE = "kerdesek.csv"; // feleletválasztós kérdések
const char* FILE_ORDER  = "sorkerdesek.csv"; // sorrendezős kérdések

/*
    CÉL: Program belépési pont
     - Game példány létrehozása (konstruktor betölti a dicsőséglistát)
     - Kérdések betöltése mindkét CSV fileból
     - Főmenü elindítása (loop – addig fut, amíg a játékos ki nem lép)
*/
int main() {
    Game game;

    game.loadQuestions(FILE_CHOOSE, FILE_ORDER); // CSV fileok betöltése kérdéstömbökbe
    game.mainMenu(); // főmenü ciklus indítása

    return 0;
}
