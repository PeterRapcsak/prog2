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

//! ---------- JÁTÉK VEZÉRLÉS ----------

// CÉL: A teljes játékmenetet kezelő osztály (menü, kérdések, segítségek, eredmény)
class Game {
public:
    static const int LEVELS = 12;          // szintek száma (kérdések száma egy játékban)
    static const int PRIZE_LADDER[LEVELS]; // nyereménylétra Ft-ban (5.000 -> 5.000.000)

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

    //! --- JÁTÉKOS ---
    std::string playerName; // aktuális játékos neve
    HighScoreTable hsTable; // dicsőséglista (konstruktorban betöltődik)

    //! --- JÁTÉKÁLLAPOT ---
    int  currentLevel; // aktuális szintindex (0 - LEVELS-1)
    int  finalPrize;   // végső nyeremény (biztos szint vagy aktuális szint)
    bool gameOver;     // Játék loop vége?
    bool walkAway;     // Megállt? 

    //! --- SEGÍTSÉGEK ---
    bool used5050;           // Használta az 50:50 segítséget?
    bool usedAudience;       // Használta a közönség segítséget?
    int  hiddenResponses[2]; // 50:50 által elrejtett válaszok indexei (-1 = nincs elrejtve)
    int  audienceValues[4];  // közönség szavazatok [A, B, C, D] %-ban

    //! --- PRIVÁT METÓDUSOK ---

    /*
        CÉL: Teljes játékmenet lebonyolítása
        BE: mode 
            1 = feleletválasztós
            2 = sorrendezős
            3 = vegyes
    */
    void play(int mode);

    // CÉL: Feleletválasztós kérdés
    void askChooseQuestion(ChooseQuestion& q);

    // CÉL: Sorrendezős kérdés
    void askOrderQuestion(OrderQuestion& q);

    // 50:50 segítség: 2 véletlenszerű helytelen válasz elrejtése
    void apply5050(ChooseQuestion& q);

    // Közönség segítség: pseudo-random szavazateloszlás generálása
    void applyAudience(ChooseQuestion& q);

    // CÉL: Minden játékállapot-változó nullázása új játék előtt
    void resetGameState();

    /*
        CÉL: Játékos nevének bekérése érvényesség-ellenőrzéssel (max 20 karakter)
        KI: A megadott név; üres bemenet -> "Jatekos"
    */
    std::string getPlayerName();

public:
    //! --- STATIKUS SEGÉDESZKÖZÖK ---

    /*
        CÉL: Egész szám ezres tagolással formázva
        BE: prize - összeg Ft-ban (pld: 5000000)
        KI: Formázott string (pld: "5.000.000")
    */
    static std::string formatPrize(int prize);

    /*
        CÉL: Megmondja, hogy az adott szintindex biztos szint-e
        BE: index
        KI: igaz ha 4 (5. kérdés) vagy 9 (10. kérdés)
    */
    static bool isSafeLevel(int index);
        
    // CÉL: Rossz válasz esetén a garantált nyeremény visszaadása
    static int getSafePrize(int index);

    // CÉL: Unicode elválasztóvonal kiírása (64 db "─" karakter)
    static void printSeparator();

    // CÉL: Enter billentyű megnyomásának várása
    static void waitEnter();

    // CÉL: Konzol törlése ANSI escape kóddal
    static void clearScreen();
};

#endif