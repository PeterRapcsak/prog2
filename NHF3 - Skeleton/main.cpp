#include "game.h"

/*======================================================================
	main.cpp - Skeleton belépési pont
----------------------------------------------------------------------
	CÉL:
	 - A játék indításáért felelős main függvény
======================================================================*/

// Ez csak egy placeholder main függvény
int main() {
    Game game;

    // CÉL: Tervezett indítási lépések demonstrálása
    game.loadQuestions("kerdesek.csv", "sorkerdesek.csv");
    game.mainMenu();

    return 0;
}