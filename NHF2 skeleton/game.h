/*======================================================================
    game.h - Header
----------------------------------------------------------------------
    CÉL:
     - Fő játék osztály deklarációja, konstansok és metódusok
======================================================================*/

#ifndef GAME_H
#define GAME_H

#include <string>
#include <vector>

#include "highscore.h"
#include "question.h"

//! ---------- JÁTÉK VEZÉRLÉS ----------

// CÉL: A teljes játékmenet összefogó osztálya.
class Game {
public:
    static const int LEVELS = 12;           // Szintek száma
    static const int PRIZE_LADDER[LEVELS];  // Nyereménylétra tömb

    // CÉL: Játék objektum létrehozása
    Game();

    // CÉL: Kérdések betöltése a két CSV fájlból
    void loadQuestions(const std::string& chooseFile, const std::string& orderFile);

    // CÉL: Főmenü futtatása
    void mainMenu();

private:
    std::vector<ChooseQuestion> chooseQuestions[LEVELS];
    std::vector<OrderQuestion> orderQuestions;
    HighScoreTable hsTable;

    std::string playerName;
    int currentLevel;       // Aktuális szint
    int finalPrize;         // Végső nyeremény
    bool gameOver;          // Játék vége állapot
    bool walkAway;          // Játékos feladta állapot
    bool used5050;          // 50:50 segítség használva
    bool usedAudience;      // Közönség segítség használva
    int hiddenResponses[2]; // Elrejtett válaszok indexei (50:50 hez használva)

    bool hasChoose; // Feleletválasztós kérdés betöltve
    bool hasOrder;  // Sorrendezős kérdés betöltve
    // Ezek azért kellenek, mert ha nem találunk pld sorrendezős kérdést
    //  akkor még lehet feleletválasztós kérdéssel is jatszani

    // CÉL: Egy teljes játékmenet futtatása a kiválasztott módban
    void play(int mode);

    // CÉL: Feleletválasztós kérdés kezelése
    void askChooseQuestion(ChooseQuestion& q);

    // CÉL: Sorrendezős kérdés kezelése
    void askOrderQuestion(OrderQuestion& q);

    // CÉL: 50:50 segítség alkalmazása
    void apply5050(ChooseQuestion& q);

    // CÉL: Közönség segítség szimulálása
    void applyAudience(ChooseQuestion& q);

    // CÉL: Játékállapot nullázása új játék előtt
    void resetGameState();

    // CÉL: Játékosnév bekérése és tisztítása
    std::string getPlayerName();

public:
    // CÉL: Összeg formázása (pl. 5000000 -> 5.000.000)
    //  - A "Ft"-et elég  kiíráskor hozzátenni
    static std::string formatPrize(int prize);

    // CÉL: Biztos szint ellenőrzése
    static bool isSafeLevel(int index);

    // CÉL: Garantált nyeremény visszaadása kiesés esetén
    static int getSafePrize(int index);

    // CÉL: Elválasztó vonal kiírása
    static void printSeparator();

    // CÉL: Enter várakozás
    static void waitEnter();
};

#endif