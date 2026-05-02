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

#include <iostream>
#include <vector>

// Headerek importálása
#include "filemanager.h"
#include "highscore.h"
#include "question.h"
#include "game.h"

using std::vector;
using std::string;
using std::cout;
using std::endl;

int main() {
    //! 1. Sorrendezős kérdés minta
    //* Válaszlehetőségek
    vector<string> valaszok = { "Párizs", "Budapest", "Berlin", "Bécs" };

    //* Kérdés létrehozása, (Nehézség, Kérdés, Kategória, Helyes válasz, Válaszlehetőségek)
    ChooseQuestion q(2, "Mi Franciaország fővárosa?", "Földrajz", "A", valaszok);

    //* Kérdés kiírása + válasz csekkolás
    q.display(); // majd hívja a metódust 
    cout << "checkAnswer(\"A\"): " << (q.checkAnswer("A") ? "helyes" : "hibas") << endl;

    //! 2. Kérdések betöltése CSV-ből
    //* vektorok létrehozása és feltöltése
    vector<ChooseQuestion> chooseList = FileManager::loadChooseQuestions("kerdesek.csv");
    vector<OrderQuestion> orderList   = FileManager::loadOrderQuestions("sorkerdesek.csv");

    //* betöltött kérdések számának kiírása
    cout << "Kérdésszám: choose=" << chooseList.size() << endl;
    cout << "Kérdésszám: order=" << orderList.size() << endl;

    //! 3. Scoreboard mentés
    //* hs objektum = highscore tábla
    //* Ha nem létezik, akkor létrehozza a filet
    HighScoreTable hs("dicsoseglista.csv");

    hs.load();
    hs.add("TesztJatekos", 1000); // + A dátumot automatikusan hozzáadja
    hs.save();

    return 0;
}