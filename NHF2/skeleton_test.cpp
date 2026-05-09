/*======================================================================
    skeleton_test.cpp - Skeleton tesztprogram
----------------------------------------------------------------------
    CÉL:
     - Osztályok alap használatának bemutatása
     - Betöltés és statikus segédfüggvények kipróbálása
======================================================================*/

#include <vector>
#include <iostream>

#include "filemanager.h"
#include "game.h"
#include "highscore.h"
#include "question.h"

//! ---------- TESZTPROGRAM VÁZ ----------

// CÉL: Egyszerű tesztprogram futtatása
int main() {
    HighScoreTable table("dicsoseglista.csv");

    // Dicsőséglista kezelés
    table.load();
    table.add("TesztElek", 100000);
    table.display();

    // Kérdések betöltése fileból
    std::vector<ChooseQuestion> choose = FileManager::loadChooseQuestions("kerdesek.csv");
    std::vector<OrderQuestion> order = FileManager::loadOrderQuestions("sorkerdesek.csv");

    // Statikus segédfüggvények ellenőrzése
    std::cout << "Formatált nyeremény: "        << Game::formatPrize(1000000) << std::endl;
    std::cout << "5. szint biztos? "            << (Game::isSafeLevel(4) ? "Igen" : "Nem") << std::endl;
    std::cout << "10. szint után garantált: "   << Game::getSafePrize(9) << std::endl;

    // Betöltött kérdések száma
    std::cout << "Feleletválasztós kérdések: "  << choose.size() << std::endl;
    std::cout << "Sorrendezős kérdések: "       << order.size() << std::endl;

    return 0;
}