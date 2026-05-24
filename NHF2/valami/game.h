/*======================================================================
    game.h - Főjáték osztály deklarációja
----------------------------------------------------------------------
    FELADAT:
     - Game osztály: teljes játékmenet vezérlése
     - Konstansok: szintszám, nyereménylétra
     - Segédeszközök: formatPrize, printSeparator, stb.
======================================================================*/

#ifndef GAME_H
#define GAME_H

#include <string>
#include <vector>

#include "highscore.h"
#include "question.h"
#include "utils.h" // Mert kell a LEVELS konstans
#include "gamestate.h" // GameState struct


//! ---------- JÁTÉK VEZÉRLÉS ----------

// CÉL: A teljes játékmenetet kezelő osztály (menü, kérdések, segítségek, eredmény)
class Game {
public:

    // CÉL: Játék objektum init (random generátor init is itt történik)
    Game();
    // Minden játék elején random seed = A jövőben lehet integrálni a játékba
    // seed kezelést, pld: ha valaki ugyanazt a játékot akarja újrajátszani
    
    // CÉL: Kérdések betöltése mindkét CSV fájlból
    void loadQuestions(const std::string& chooseFile, const std::string& orderFile);

    // CÉL: Főmenü ciklus elindítása (addig fut, amíg a játékos ki nem lép)
    void mainMenu();


private:
    //! --- KÉRDÉSEK ---
    std::vector<ChooseQuestion> chooseQuestions[LEVELS]; // feleletválasztós kérdések szint szerint (0-alapú)
    std::vector<OrderQuestion>  orderQuestions;          // sorrendezős kérdések
    bool hasChoose; // van-e betöltött feleletválasztós kérdés
    bool hasOrder;  // van-e betöltött sorrendezős kérdés
    // Ha az egyik hiányzik, a vegyes módban csak a másik fut

    GameState state; // Játékállapot tárolása

    //! --- JÁTÉKOS ---
    std::string playerName; // aktuális játékos neve
    HighScoreTable hsTable; // dicsőséglista (konstruktorban betöltődik)

    //! --- PRIVÁT METÓDUSOK ---

    /*
        CÉL: Teljes játékmenet lebonyolítása
        BE: mode 
            1 = feleletválasztós
            2 = sorrendezős
            3 = vegyes
    */
    void play(int mode);

    // CÉL: Minden játékállapot-változó nullázása új játék előtt
    void resetGameState();

    /*
        CÉL: Játékos nevének bekérése érvényesség-ellenőrzéssel (max 20 karakter)
        KI: A megadott név; üres bemenet -> "Jatekos"
    */
    std::string getPlayerName();
};




#endif