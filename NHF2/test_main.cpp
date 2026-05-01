/*======================================================================
    test_main.cpp - Egyszerű fordítás/linkelés teszt
----------------------------------------------------------------------
    CÉL:
     - Ellenőrizni hogy minden .cpp együtt fordul és linkel
     - Ellenőrizni hogy a CSV fileok elérhetőek-e
======================================================================*/

#include <iostream>

#include "filemanager.h"
#include "game.h"

//! ---------- TESZT BELÉPÉSI PONT ----------

/*
    CÉL: Minimális teszt futtatása
     - file létezés ellenőrzés
     - Game objektum létrehozás
     - loadQuestions hívás
*/
int main() {
    std::cout << "[TESZT] Forditas + linkeles ellenorzes indul...\n";

    std::cout << "[TESZT] kerdesek.csv letezik: "
              << (FileManager::fileExists("kerdesek.csv") ? "igen" : "nem")
              << "\n";
    std::cout << "[TESZT] sorkerdesek.csv letezik: "
              << (FileManager::fileExists("sorkerdesek.csv") ? "igen" : "nem")
              << "\n";

    Game game;
    game.loadQuestions("kerdesek.csv", "sorkerdesek.csv");

    std::cout << "[TESZT] OK: Game objektum letrejott, kerdesek betoltese meghivva.\n";
    std::cout << "[TESZT] Linkeles rendben, a program fut.\n";

    return 0;
}
