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

    // CÉL: Játék objektum alapállapotba hozása (srand init is itt történik)
    Game();
    
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
    std::string playerName;  // aktuális játékos neve
    HighScoreTable hsTable;  // dicsőséglista (konstruktorban betöltődik)

    //! --- JÁTÉKÁLLAPOT ---
    int  currentLevel; // aktuális szintindex (0-alapú, 0..LEVELS-1)
    int  finalPrize;   // a kifizetendő nyeremény (biztos szint vagy aktuális szint)
    bool gameOver;     // igaz, ha a játék véget ért (rossz válasz vagy leállás)
    bool walkAway;     // igaz, ha a játékos önként állt meg (Q gomb)

    //! --- SEGÍTSÉGEK ---
    bool used5050;          // az 50:50 segítség el lett-e már használva
    bool usedAudience;      // a közönség segítség el lett-e már használva
    int  hiddenResponses[2];// 50:50 által elrejtett válaszok indexei (-1 = nincs elrejtve)
    int  audienceValues[4]; // közönség szavazatok [A, B, C, D] %-ban (applyAudience tölti)

    //! --- PRIVÁT METÓDUSOK ---

    /*
        CÉL: Teljes játékmenet lebonyolítása
        BE: mode - 1=feleletválasztós, 2=sorrendezős, 3=vegyes
    */
    void play(int mode);

    /*
        CÉL: Feleletválasztós kérdés feltevése és megválaszoltatása
        BE: q - az aktuális kérdés objektum (referencia, mert 50:50 módosítja a megjelenítést)
    */
    void askChooseQuestion(ChooseQuestion& q);

    /*
        CÉL: Sorrendezős kérdés feltevése és megválaszoltatása
        BE: q - az aktuális sorrendezős kérdés objektum
    */
    void askOrderQuestion(OrderQuestion& q);

    /*
        CÉL: 50:50 segítség: 2 véletlenszerű helytelen válasz elrejtése
        BE: q - az aktuális kérdés (helyes válasz kiolvasásához)
    */
    void apply5050(ChooseQuestion& q);

    /*
        CÉL: Közönség segítség: pseudo-random szavazateloszlás generálása
        BE: q - az aktuális kérdés (helyes válasz meghatározásához)
    */
    void applyAudience(ChooseQuestion& q);

    // CÉL: Minden játékállapot-változó nullázása / alapállapotba hozása új játék előtt
    void resetGameState();

    /*
        CÉL: Játékos nevének bekérése érvényesség-ellenőrzéssel (max. 20 karakter)
        KI: A megadott név; üres bemenet -> "Jatekos"
    */
    std::string getPlayerName();

public:
    //! --- STATIKUS SEGÉDESZKÖZÖK ---

    /*
        CÉL: Egész szám ezres tagolással formázott stringgé alakítása
        BE: prize - összeg Ft-ban (pl. 5000000)
        KI: Formázott string (pl. "5.000.000")
    */
    static std::string formatPrize(int prize);

    /*
        CÉL: Megmondja, hogy az adott szintindex biztos szint-e
        BE: index - 0-alapú szintindex
        KI: true ha 4 (5. kérdés) vagy 9 (10. kérdés)
    */
    static bool isSafeLevel(int index);

    /*
        CÉL: Rossz válasz esetén a garantált nyeremény visszaadása
        BE: index - a kiesés szintje (0-alapú)
        KI: Az utolsó elért biztos szint összege (0 ha nem értük el)
    */
    static int getSafePrize(int index);

    // CÉL: Unicode elválasztóvonal kiírása (64 db ─ karakter)
    static void printSeparator();

    // CÉL: Enter billentyű megnyomásának várakozása
    static void waitEnter();

    // CÉL: Konzol törlése ANSI escape kóddal
    static void clearScreen();
};

#endif
