/*======================================================================
    main.cpp - Főprogram
----------------------------------------------------------------------
    FELADAT:
     - Game objektum létrehozása
     - Kérdések betöltése CSV fileokból
     - Főmenü (loop) indítása

    //! Fordítási parancs:
    //! g++ -std=c++11 -Wall -Wextra -o loim main.cpp game.cpp highscore.cpp question.cpp filemanager.cpp test.cpp
======================================================================*/

#include "game.h"

// teszteléshez, később törölhető
int tesztelek(bool run);

// CÉL: CSV filenevek
const char* FILE_CHOOSE = "kerdesek.csv";    // feleletválasztós kérdések
const char* FILE_ORDER  = "sorkerdesek.csv"; // sorrendezős kérdések

// CÉL: Program belépési pont
int main() {
    
    // TESZTELŐ FÜGGVÉNY, ha nem akarjuk futtatni akkor csak simán írjuk át tesztelek(false)-ra
    // test.cpp file-t futtatja
    tesztelek(false);


    Game game; // konstruktor betölti a dicsőséglistát
    game.loadQuestions(FILE_CHOOSE, FILE_ORDER); // CSV fileok betöltése kérdéstömbökbe
    game.mainMenu(); // főmenü ciklus indítása

    return 0;
}
