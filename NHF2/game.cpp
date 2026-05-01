/*======================================================================
    game.cpp - Játék logika implementáció
----------------------------------------------------------------------
    CÉL:
     - Game osztály metódusok implementálása
     - Nyereménylétra és alap játékállapot kezelés
======================================================================*/

#include "game.h"
#include "filemanager.h"
#include <iostream> // konzol kiírás / bemenet

//! ---------- KONSTANSOK ----------

// CÉL: 12 lépcsős nyereménylétra (Ft)
const int Game::PRIZE_LADDER[Game::LEVELS] = {
    5000, 10000, 20000, 50000, 100000, 200000,
    300000, 500000, 800000, 1500000, 3000000, 5000000
};

//! ---------- KONSTRUKTOR ----------

/*
    CÉL: Game objektum alap állapotának beállítása
*/
Game::Game():
    currentLevel(0),
    finalPrize(0),
    gameOver(false),
    walkAway(false),
    used5050(false),
    usedAudience(false),
    hasChoose(false),
    hasOrder(false)
{
    hiddenResponses[0] = -1;
    hiddenResponses[1] = -1;
}

//! ---------- KÉRDÉSEK BETÖLTÉSE ----------

/*
    CÉL: Kérdések betöltése 2 CSV fileból
    BE:
     - chooseFile: feleletválasztós CSV neve
     - orderFile: sorrendezős CSV neve
*/
void Game::loadQuestions(const std::string& chooseFile, const std::string& orderFile) {
    std::vector<ChooseQuestion> choose = FileManager::loadChooseQuestions(chooseFile);
    std::vector<OrderQuestion> order   = FileManager::loadOrderQuestions(orderFile);

    // Szintek ürítése újrabetöltés előtt
    for (int i = 0; i < LEVELS; ++i) {
        chooseQuestions[i].clear();
    }

    // Feleletválasztós kérdések szétosztása nehézség szerint
    for (std::size_t i = 0; i < choose.size(); ++i) {
        int diff = choose[i].getDifficulty();
        if (diff >= 1 && diff <= LEVELS) {
            chooseQuestions[diff - 1].push_back(choose[i]); // 0-alapú indexelés
        }
    }

    orderQuestions = order;

    // Legalább 1 kérdés esetén az adott mód elérhető
    hasChoose = !choose.empty();
    hasOrder  = !orderQuestions.empty();
}

//! ---------- FŐMENÜ ----------

/*
    CÉL: Főmenü indítása (jelenleg skeleton placeholder)
*/
void Game::mainMenu() {
    std::cout << "[INFO] Skeleton mainMenu: teszt uzemmod.\n";
}

//! ---------- JÁTÉKMENET ----------

/*
    CÉL: Egy teljes játékmenet lebonyolítása
    BE: mode - játékmód
*/
void Game::play(int mode) {
    (void)mode; // TODO: implementálandó
}

/*
    CÉL: Feleletválasztós kérdés feltevése
    BE: q - aktuális kérdés objektum
*/
void Game::askChooseQuestion(ChooseQuestion& q) {
    (void)q; // TODO: implementálandó
}

/*
    CÉL: Sorrendezős kérdés feltevése
    BE: q - aktuális kérdés objektum
*/
void Game::askOrderQuestion(OrderQuestion& q) {
    (void)q; // TODO: implementálandó
}

//! ---------- SEGÍTSÉGEK ----------

/*
    CÉL: 50:50 segítség alkalmazása
    BE: q - aktuális feleletválasztós kérdés
*/
void Game::apply5050(ChooseQuestion& q) {
    (void)q; // TODO: implementálandó
}

/*
    CÉL: Közönség segítség szimulálása
    BE: q - aktuális feleletválasztós kérdés
*/
void Game::applyAudience(ChooseQuestion& q) {
    (void)q; // TODO: implementálandó
}

//! ---------- JÁTÉKÁLLAPOT ----------

/*
    CÉL: Játékállapot nullázása új játék előtt
*/
void Game::resetGameState() {
    currentLevel       = 0;
    finalPrize         = 0;
    gameOver           = false;
    walkAway           = false;
    used5050           = false;
    usedAudience       = false;
    hiddenResponses[0] = -1;
    hiddenResponses[1] = -1;
}

/*
    CÉL: Játékos név visszaadása
    KI: játékos neve (jelenleg fix)
*/
std::string Game::getPlayerName() {
    return "Jatekos"; // TODO: később valódi névbekérés
}

//! ---------- STATIKUS SEGÉDESZKÖZÖK ----------

/*
    CÉL: Nyeremény összeg formázása
    BE: prize - a formázandó összeg
    KI: formázott string
*/
std::string Game::formatPrize(int prize) {
    return std::to_string(prize); // TODO: ezres tagolás később
}

/*
    CÉL: Megmondja hogy az adott szint biztos szint-e
    BE: index - 0-alapú szintindex
    KI: true = biztos szint, false = nem
*/
bool Game::isSafeLevel(int index) {
    return index == 4 || index == 9;
}

/*
    CÉL: Garantált nyeremény visszaadása kiesés esetén
    BE: index - kiesés szintindexe
    KI: utolsó biztos szint összege
*/
int Game::getSafePrize(int index) {
    if (index >= 9) return PRIZE_LADDER[9]; // 10. kérdés után
    if (index >= 4) return PRIZE_LADDER[4]; // 5. kérdés után
    return 0;
}

/*
    CÉL: Elválasztó vonal kiírása a konzolra
*/
void Game::printSeparator() {
    std::cout << "----------------------------------------------------------------\n";
}

/*
    CÉL: Enter várakozás
*/
void Game::waitEnter() {
    std::cout << "\n[Nyomj Entert a folytatáshoz...]";
    std::cin.ignore();
}
