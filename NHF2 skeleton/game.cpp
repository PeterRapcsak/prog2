/*======================================================================
    game.cpp - Játékmenet logikája
----------------------------------------------------------------------
    FELADAT:
     - mainMenu() és play() működése
     - 12 szintnyi játék, szintenként kérdés választás
     - Opciók:
	 	 - 50:50	| A eltüntet 2 helytelen válaszlehetőséget
		 - közönség	| Pseudo-random százalékos eloszlással segít (LEHET HOGY FÉLREVEZET)
		 - feladás	| A játék feladása és a nyeremény megtartására
     - Válaszellenőrzés, nyeremenylista, biztos szintek (5., 10.)
     - Játékos neve, aktuális szint, végső nyeremény kezelése
======================================================================*/

#include <iostream>
#include "game.h"
#include "filemanager.h"

using std::string;

// Nyereménylétra tömb
const int Game::PRIZE_LADDER[Game::LEVELS] = {
    500, 1000, 2000, 5000, 10000, 20000, 50000,
	100000, 200000, 500000, 1000000, 5000000
};

// Konstruktor: Default state init
Game::Game()
    : currentLevel(0),          // Aktuális szint
      finalPrize(0),            // Végső nyeremény
      gameOver(false),          // Játék vége állapot
      walkAway(false),          // Játékos feladta állapot
      used5050(false),          // 50:50 segítség használva
      usedAudience(false),      // Közönség segítség használva
      hiddenResponses{-1, -1},  // Elrejtett válaszok indexei (50:50 hez használva)
      hasChoose(false),         // Feleletválasztós kérdés betöltve
      hasOrder(false) {}        // Sorrendezős kérdés betöltve


// CÉL: Kérdések betöltése a két CSV fájlból
void Game::loadQuestions(const string&, const string&) {}

// CÉL: Főmenü futtatása
void Game::mainMenu() {}


// CÉL: Egy teljes játékmenet futtatása a kiválasztott módban
void Game::play(int) {}

// CÉL: Feleletválasztós kérdés kezelése
void Game::askChooseQuestion(ChooseQuestion&) {}

// CÉL: Sorrendezős kérdés kezelése
void Game::askOrderQuestion(OrderQuestion&) {}

// CÉL: 50:50 segítség alkalmazása
void Game::apply5050(ChooseQuestion&) {}

// CÉL: Közönség segítség szimulálása
void Game::applyAudience(ChooseQuestion&) {}

// CÉL: Játékállapot nullázása új játék előtt
void Game::resetGameState() {}

// CÉL: Játékosnév bekérése és tisztítása
string Game::getPlayerName() { return "Jatekos"; }


// CÉL: Összeg formázása (pl. 5000000 -> 5.000.000)
//  - A "Ft"-et elég  kiíráskor hozzátenni
string Game::formatPrize(int) { return "0"; }

// CÉL: Biztos szint ellenőrzése
bool Game::isSafeLevel(int) { return false; }

// CÉL: Garantált nyeremény visszaadása kiesés esetén
int Game::getSafePrize(int) { return 0; }

// CÉL: Elválasztó vonal kiírása
void Game::printSeparator() {}

// CÉL: Enter várakozás
void Game::waitEnter() {}

//! Ez a sok komment most csúnyán néz ki, és még pontosan ugyanaz mint a game.h-ban
//! de majd úgy írom meg a metódusokat, hogy pld írok olyan fieldeket is hogy BE: és KI:
//! hogy teljesen egyértelmű legyen hogy pontosan mi mit csinál.  :D