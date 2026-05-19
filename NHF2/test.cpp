/*======================================================================
    TESZT PROGRAM!!!
----------------------------------------------------------------------
    CÉL:
     - Osztályok használatának bemutatása
     - Betöltés és statikus segédfüggvények kipróbálása (olykor rossz inputokkal is)
======================================================================*/

#include <vector>
#include <iostream>

#include "filemanager.h"
#include "game.h"
#include "highscore.h"
#include "question.h"

//! ---------- TESZTPROGRAM ----------

// Visszatérési érték: 0 = siker/nem futtatjuk, 1 = futtattuk
int tesztelek(bool run) {
    if (!run) {
        return 0; 
    }

    //! ---------- GAME ----------
    //? Game segédfüggvényei hibás inputokkal
    std::cout << "GAME formatPrize, 0 Ft: \t\t\t"      << Game::formatPrize(0)        << std::endl;
    std::cout << "GAME formatPrize, -5000000 Ft: \t\t" << Game::formatPrize(-5000000) << std::endl;
    std::cout << "GAME formatPrize, 999 Ft: \t\t\t"    << Game::formatPrize(999)      << std::endl << "\n";

    std::cout << "GAME isSafeLevel, index -1: \t\t"          << (Game::isSafeLevel(-1) ? "Biztos" : "Nem")  << std::endl;
    std::cout << "GAME isSafeLevel, index 0: \t\t"           << (Game::isSafeLevel(0) ? "Biztos" : "Nem")   << std::endl;
    std::cout << "GAME isSafeLevel, index 4 (5. szint): \t"  << (Game::isSafeLevel(4) ? "Biztos" : "Nem")   << std::endl;
    std::cout << "GAME isSafeLevel, index 9 (10. szint): \t" << (Game::isSafeLevel(9) ? "Biztos" : "Nem")   << std::endl;
    std::cout << "GAME isSafeLevel, index 100: \t\t"         << (Game::isSafeLevel(100) ? "Biztos" : "Nem") << std::endl << "\n";

    std::cout << "GAME getSafePrize, index -1: \t\t"     << Game::getSafePrize(-1)  << std::endl;
    std::cout << "GAME getSafePrize, index 0: \t\t"      << Game::getSafePrize(0)   << std::endl;
    std::cout << "GAME getSafePrize, index 4 (után): \t" << Game::getSafePrize(4)   << std::endl;
    std::cout << "GAME getSafePrize, index 9 (után): \t" << Game::getSafePrize(9)   << std::endl;
    std::cout << "GAME getSafePrize, index 100: \t\t"    << Game::getSafePrize(100) << std::endl << "\n";


    std::vector<std::string> temp = {"optA", "optB", "optC", "optD"};


    //! ---------- CHOOSE QUESTION ----------
    //? Hibás input: CHOOSE class 
    ChooseQuestion badChoose(0, "", "", "A", temp); // A helyes vélasz = 'A'

    std::cout << "CHOOSE class, hibás input: 'Z'\t\t";
    if (badChoose.checkAnswer("Z"))
        std::cout << "Elfogadva"  << std::endl;
    else
        std::cout << "Elutasítva" << std::endl;

    std::cout << "CHOOSE class, helyes input: 'A'\t\t";
    if (badChoose.checkAnswer("A"))
        std::cout << "Elfogadva"  << std::endl;
    else
        std::cout << "Elutasítva" << std::endl;


    //! ---------- ORDER QUESTION ----------
    //? Hibás input: ORDER class
    OrderQuestion badOrder("Valami bla bla", "cat", "ABCD", temp); // A helyes vélasz = "ABCD"

    std::cout << "ORDER class, hibás input: 'asdf1'\t";
    if (badOrder.checkAnswer("AB"))
        std::cout << "Elfogadva"  << std::endl;
    else
        std::cout << "Elutasítva" << std::endl;

    std::cout << "ORDER class, hibás input: 'ABCD'\t";
    if (badOrder.checkAnswer("ABCD"))
        std::cout << "Elfogadva"  << std::endl;
    else
        std::cout << "Elutasítva" << std::endl;


    //! ---------- LOADING ----------
    //? Nincs file / rossz path
    std::cout << "CHOOSE QUESTION - rossz path: ";
    std::vector<ChooseQuestion> missingChoose = FileManager::loadChooseQuestions("hajnal.csv");

    if (missingChoose.empty())
        std::cout << "\t\tNem töltött be semmit"   << std::endl;
    else
        std::cout << "\t\tVan adat :<" << std::endl;


    std::cout << "ORDER QUESTION - rossz path: ";
    std::vector<OrderQuestion> missingOrder = FileManager::loadOrderQuestions("kettővan.csv");

    if (missingOrder.empty())
        std::cout << "\t\tNem töltött be semmit"   << std::endl;
    else
        std::cout << "\t\tVan adat :<" << std::endl;


    //? Kérdések betöltése fileból
    std::vector<ChooseQuestion> choose = FileManager::loadChooseQuestions("kerdesek.csv");
    std::vector<OrderQuestion> order = FileManager::loadOrderQuestions("sorkerdesek.csv");

    //? Betöltött kérdések száma
    std::cout << "Feleletválasztós kérdések: \t\t"<< choose.size() << std::endl;
    std::cout << "Sorrendezős kérdések: \t\t\t"<< order.size()     << std::endl;


    //! ---------- DICSŐSÉGLISTA ----------
    //? HighScoreTable üres / negatív nyeremény
    std::cout << "\nHIGHSCORE TABLE - üres név: \t\t";
    HighScoreTable testTable("test.csv");
    testTable.add("", 500000);
    std::cout << "Hozzáadva" << std::endl;

    std::cout << "HIGHSCORE TABLE - negatív nyeremény: \t";
    testTable.add("Negativ", -100000);
    std::cout << "Hozzáadva" << std::endl;

    std::cout << "HIGHSCORE TABLE - nulla nyeremény: \t";
    testTable.add("Nulla", 0);
    std::cout << "Hozzáadva" << std::endl;


    //! ---------- DICSŐSÉGLISTA CHECK ----------

    HighScoreTable table("test.csv");

    table.load();
    table.display();


    std::cout << "\nTeszt vége\n";

    Game::waitEnter(); // el is lehessen olvasni a teszt eredményét

    return 1;
}