/*======================================================================
    game.cpp - Játékmenet logikája
----------------------------------------------------------------------
    FELADAT:
     - mainMenu() és play() működése
     - 12 szintnyi játék, szintenként kérdés választás
     - Opciók:
         - 50:50    | A eltüntet 2 helytelen válaszlehetőséget
         - közönség | Pseudo-random százalékos eloszlással segít (LEHET HOGY FÉLREVEZET)
         - feladás  | A játék feladása és a nyeremény megtartása
     - Válaszellenőrzés, nyeremenylista, biztos szintek (5., 10.)
     - Játékos neve, aktuális szint, végső nyeremény kezelése
======================================================================*/

#include "memtrace.h" // memóriaszivárgás check

#include "game.h"
#include "filemanager.h"
#include "colors.h"
#include "question.h"

#include "utils.h" // Amit írtál, hogy legyen egy Utils (file)

#include <algorithm> // shuffle
#include <cctype>    // toupper
#include <iostream>  // konzol kiírás / bemenet
#include <limits>    // numeric_limits
#include <random>    // mt19937

using std::string;
using std::vector;
using std::cout;
using std::endl;
using std::cin;


enum QuestionType {
    ChooseQuestionType,
    OrderQuestionType
};

//! ---------- SEGÉDFÜGGVÉNYEK ----------

void printMainMenuScreen() {

    clearScreen();
    printSeparator();
    cout << Color::BOLD_YELLOW << "                   LEGYEN ÖN IS MILLIOMOS!\n" << Color::RESET;
    printSeparator();
    cout << "\n";
    cout << "  " << Color::BOLD_YELLOW << "[1]" << Color::RESET << " ÚJ JÁTÉK\n";
    cout << "  " << Color::BOLD_YELLOW << "[2]" << Color::RESET << " DICSŐSÉGLISTA\n";
    cout << "  " << Color::BOLD_YELLOW << "[3]" << Color::RESET << " KILÉPÉS\n";
    cout << "\n";
    printSeparator();
}



// CÉL: Főmenü választás olvasása (1-3), érvénytelen bemenetnél hibaüzenet + újrakérdezés
int readMenuChoice() {
    while (true) {
        cout << "Valasztas: ";
        int v = readInt();

        if (v >= 1 && v <= 3) return v;

        cout << Color::BOLD_RED << "Érvénytelen bemenet!\n" << Color::RESET;
    }
}


// CÉL: Összekeveri a kérdések sorrendjét
vector<int> buildShuffledOrderIndexes(int count) {
    vector<int> indexes;

    // Feltöltjük: 0, 1, 2, 3 ... (jövőben flexibilisen updatelhető)
    for (int i = 0; i < count; i++) {
        indexes.push_back(i);
    }

    // Összekeverjük az indexeket (iterátorokkal)
    std::shuffle(indexes.begin(), indexes.end(), rng());

    return indexes;
}


// CÉL: Eldönti, hogy milyen típusú kérdés következzen
QuestionType pickQuestionType(int mode, bool hasChoose, bool hasOrder, int level) {
    if (mode == 1) return ChooseQuestionType; // Feleletválasztós
    if (mode == 2) return OrderQuestionType;  // Sorrendezős
    
    if (!hasOrder)  return ChooseQuestionType; // Feleletválasztós
    if (!hasChoose) return OrderQuestionType;  // Sorrendezős

    // Vegyes módban a 3. 7. és 11. kérdés sorrendezős, a többi feleletválasztós
    // (azért nem minden 3. kérdés, mert akkor a 12. kérdés is sorrendezős lenne, ami azért tényleg elég nehéz lenne)
    return (level + 1 == 3 || level + 1 == 7 || level + 1 == 11) ? OrderQuestionType : ChooseQuestionType;
}

// CÉL: Kiírja a játék kimenetelét (megállt, nyert, veszített) és a nyereményt
//! ITT LEHETNE GAMESTATE-ET IS HASZNÁLNI!!!
void printGameResult(
    bool walkAway,              // Megállt?
    bool gameOver,              // Játék loop vége?
    int currentLevel,           // Jelenlegi szint
    const string& playerName,   // Játékos neve
    int finalPrize) {           // Végső nyeremény (Nem itt számoljuk ki)

    string prize = formatPrize(finalPrize) + " Ft";

    if (walkAway) {
        cout << "\nMegálltál. Nyereményed: ";
    } else if (!gameOver && currentLevel >= LEVELS) {
        cout << "\n" << Color::BOLD_YELLOW
             << "Gratulálok, " << playerName << "!\n"
             << Color::RESET;

        cout << "Megnyerted a főnyereményt: ";
    } else {
        cout << "\nJáték vége. Nyereményed: ";
    }

    cout << Color::BOLD_GREEN << prize << Color::RESET << "\n";
}



//! ---------- KONSTRUKTOR ----------

Game::Game() {
    hasChoose = false; // Létezik feleletválasztós kérdés?
    hasOrder  = false; // Létezik sorrendezős kérdés?

    state.walkAway = false;
    state.gameOver = false;
    state.used5050 = false;
    state.usedAudience = false;
    state.currentLevel = 0;
    state.finalPrize = 0;
    state.hiddenResponses[0] = state.hiddenResponses[1] = -1;
    state.audienceValues[0] = state.audienceValues[1] = state.audienceValues[2] = state.audienceValues[3] = 0;

    rng(); // Random seed generálása

}

//! ---------- KÉRDÉSEK BETÖLTÉSE ----------


// CÉL: Kérdések betöltése fileokból, majd szintek szerint eltárolása
void Game::loadQuestions(const string& chooseFile, const string& orderFile) {

    // Feleletválasztós kérdések, temp változóba, ömlesztve
    vector<ChooseQuestion> temp = FileManager::loadChooseQuestions(chooseFile);

    // Sorrendezős kérdések
    orderQuestions = FileManager::loadOrderQuestions(orderFile);

    // Feleletválasztós kérdések vektorának kiürítése
    // Csak 1x hívjuk de biztos ami biztos 
    for (int i = 0; i < LEVELS; i++) {
        chooseQuestions[i].clear();
    }

    // Feleletválasztós kérdések berakása a nehézségük szerinti szintre
    for (size_t i = 0; i < temp.size(); i++) {

        const ChooseQuestion& question = temp[i]; // Ha referencia akkor nem másolgat mindig

        int diff = question.getDifficulty(); // Kérdés nehézségi szintje

        // Csak akkor rakjuk be, ha érvényes szint: 1-12
        if (diff >= 1 && diff <= LEVELS) {
            chooseQuestions[diff - 1].push_back(question); // diff 1-től indul, tömb index 0-tól
        }
    }

    // Eltároljuk, hogy van-e legalább egy betöltött kérdés az adott típusból
    hasChoose = !temp.empty();
    hasOrder = !orderQuestions.empty();
}

//! ---------- FŐMENÜ ----------

// Ezt nem kommentelem túl, mert sok minden mást hívok ami már dokumentálva van
void Game::mainMenu() {
    bool exitMenu = false;

    while (!exitMenu) {
        printMainMenuScreen();
        int choice = readMenuChoice();

        switch (choice) {

            // Új játék
            case 1: {
                int mode = selectGameMode();

                if (mode == 4) {
                    break;
                }

                // Kérdések meglétének ellenőrzése
                if (mode == 1 && !hasChoose) {
                    cout << "Nincs betöltött feleletválasztós kérdés.\n";
                    waitEnter();
                    break;
                }

                if (mode == 2 && !hasOrder) {
                    cout << "Nincs betöltött sorrendezős kérdés.\n";
                    waitEnter();
                    break;
                }

                if (mode == 3 && !hasChoose && !hasOrder) {
                    cout << "Nincs betöltött kérdés.\n";
                    waitEnter();
                    break;
                }

                play(mode);
                break;
            }

            // Dicsőséglista
            case 2: {
                hsTable.load();

                clearScreen();
                hsTable.display();

                cout << "\n  " << Color::BOLD_YELLOW << "[1]" << Color::RESET << " Vissza\n";
                cout << "  "   << Color::BOLD_YELLOW << "[2]" << Color::RESET << " Lista törlése\n";

                while (true) {
                    cout << "\nValasztas: ";
                    int v = readInt();

                    if (v == 1) {
                        break;
                    }

                    if (v == 2) {
                        hsTable.reset();
                        cout << Color::BOLD_RED << "A dicsőséglista törölve.\n" << Color::RESET;
                        waitEnter();
                        break;
                    }

                    cout << Color::BOLD_RED << "Érvénytelen bemenet!\n" << Color::RESET;
                }

                break;
            }

            // Kilépés
            case 3:
                exitMenu = true;
                break;
        }
    }
}

//! ---------- JÁTÉKMENET ----------

void Game::play(int mode) {
    resetGameState();
    reseedRng();
    clearScreen();

    playerName = getPlayerName();

    // A sorrendezős kérdések összekeverése
    // vektorban int-ek, kell cast mert különben size_t lenne
    vector<int> orderQuestionIndexes = buildShuffledOrderIndexes(static_cast<int>(orderQuestions.size()));
    std::size_t nextOrderQuestion = 0;

    // Végigmegyünk a szinteken
    for (state.currentLevel = 0; state.currentLevel < LEVELS && !state.gameOver; ++state.currentLevel) {
        
        // Függvény hivása, ami eldönti hogy milyen kérdés típus legyen
        QuestionType questionType = pickQuestionType(
            mode,
            hasChoose,
            hasOrder,
            state.currentLevel
        );

        switch (questionType) {

            //? FELELETVÁLASZTÓS KÉRDÉS
            case ChooseQuestionType: {
                
                // Egyik leghosszabb változónév amit valaha adtam
                // Megadja hogy van-e választós kérdés egy adott szinten
                bool noChooseQuestionOnThisLevel = chooseQuestions[state.currentLevel].empty();

                if (noChooseQuestionOnThisLevel) {
                    cout << "Nincs elég feleletválasztós kérdés ezen a szinten, a játék leáll.\n";

                    state.gameOver = true;
                    state.finalPrize = getSafePrize(state.currentLevel - 1);
                    break;
                }

                // size_t -> int , ebből random indexet generálunk
                int questionCount = static_cast<int>(chooseQuestions[state.currentLevel].size());
                int randomIndex = randomInt(0, questionCount - 1);

                ChooseQuestion& question = chooseQuestions[state.currentLevel][randomIndex];

                question.ask(state);
                break;
            }

            //? SORRENDEZŐS KÉRDÉS
            case OrderQuestionType: {

                // Megadja hogy van-e sorrendezős kérdés (Nem kell szintet csekkolni)
                bool noOrderQuestions = orderQuestionIndexes.empty();

                if (noOrderQuestions) {
                    cout << "Nincs sorrendezős kérdés, a játék leáll.\n";

                    state.gameOver = true;
                    state.finalPrize = getSafePrize(state.currentLevel - 1);
                    break;
                }

                // Ha elfogytak a sorrendezős kérdések, kezdjük újra az elejéről
                if (nextOrderQuestion >= orderQuestionIndexes.size()) {
                    nextOrderQuestion = 0;
                }

                // egyesével léptetjük őket a MÁR kevert index listán
                int questionIndex = orderQuestionIndexes[nextOrderQuestion];
                ++nextOrderQuestion;

                OrderQuestion& question = orderQuestions[questionIndex];

                question.ask(state);
                break;
            }
        }

        // Ha a kérdés után még nem lett vége a játéknak,
        // akkor a játékos elérte az aktuális szint nyereményét
        if (!state.gameOver) {
            state.finalPrize = PRIZE_LADDER[state.currentLevel];
        }
    }

    // Játék vége: képernyő törlése, eredmény kiírása, pontszám mentése
    clearScreen();

    //! EZT IS LEHETNE STATE-TEL
    printGameResult(
        state.walkAway,
        state.gameOver,
        state.currentLevel,
        playerName,
        state.finalPrize
    );

    hsTable.add(playerName, state.finalPrize); // Dátumot ott számolunk

    waitEnter();
}

//! ---------- JÁTÉKÁLLAPOT ----------

void Game::resetGameState() {
    state.currentLevel       = 0;
    state.finalPrize         = 0;
    state.gameOver           = false;
    state.walkAway           = false;
    state.used5050           = false;
    state.usedAudience       = false;
    state.hiddenResponses[0] = -1;
    state.hiddenResponses[1] = -1;
    state.audienceValues[0]  = state.audienceValues[1] = state.audienceValues[2] = state.audienceValues[3] = 0;
}

string Game::getPlayerName() {
    while (true) {

        cout << "Játékos neve (max 20 karakter): ";
        string name;
        getline(cin, name);

        if (name.empty()) return "Jatekos";

        // UTF-8 kompatibilis hossz check
        // https://www.daniweb.com/programming/software-development/threads/474127/reading-and-checking-unusual-characters
        std::size_t visLen = 0;

        for (std::size_t i = 0; i < name.size(); ++i) {
            unsigned char c = static_cast<unsigned char>(name[i]);


            // 0xC0 = 11000000
            // 0x80 = 10000000
            if ((c & 0xC0) != 0x80) {
                ++visLen;
            }
            
            // Őszintén fogalmam sincs hogy csinalja ezt :<
            //* Mostmár értem
        }

        if (visLen > 20) {
            cout << Color::BOLD_RED << "A játékos neve túl hosszú! (max 20 karakter)\n" << Color::RESET;
            continue;
        }

        return name;
    }
}
