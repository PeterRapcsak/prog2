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

#include "game.h"
#include "filemanager.h"
#include "colors.h"

#include <algorithm> // shuffle
#include <cctype>    // toupper
#include <cstdlib>   // rand
#include <ctime>     // time
#include <iostream>  // konzol kiírás / bemenet
#include <limits>    // numeric_limits
#include <random>    // mt19937

using std::string;
using std::vector;
using std::cout;
using std::endl;
using std::cin;


namespace {

static const string SEP = "────────────────────────────────────────────────────────────────";

//! ---------- SEGÉDFÜGGVÉNYEK ----------

void printMainMenuScreen() {

    Game::clearScreen();
    cout << SEP << "\n";
    cout << Color::BOLD_YELLOW << "                   LEGYEN ÖN IS MILLIOMOS!\n" << Color::RESET;
    cout << SEP << "\n\n";
    cout << "  " << Color::BOLD_YELLOW << "[1]" << Color::RESET << " ÚJ JÁTÉK\n";
    cout << "  " << Color::BOLD_YELLOW << "[2]" << Color::RESET << " DICSŐSÉGLISTA\n";
    cout << "  " << Color::BOLD_YELLOW << "[3]" << Color::RESET << " KILÉPÉS\n";
    cout << "\n" << SEP << "\n";
}

// CÉL: Egész szám olvasása, nem szám bemenet esetén -1-et ad vissza
int readInt() {
    int value;

    if (cin >> value) {
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Ez egy buffer ami kiignorál minden maradék inputot
        // https://stackoverflow.com/questions/25020129/cin-ignorenumeric-limitsstreamsizemax-n

        return value;
    }
    cin.clear(); // cin hibás állapotának clear-elése
    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // ugyanúgy ignorálás
    return -1;
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

// CÉL: Játékmód választás olvasása (1-4), érvénytelen bemenetnél hibaüzenet + újrakérdezés
int selectGameMode() {

    cout << "\nJátékmódok:\n";
    cout << "  " << Color::BOLD_YELLOW << "[1]" << Color::RESET << " Feleletválasztós\n";
    cout << "  " << Color::BOLD_YELLOW << "[2]" << Color::RESET << " Sorrendezős\n";
    cout << "  " << Color::BOLD_YELLOW << "[3]" << Color::RESET << " Vegyes\n";
    cout << "  " << Color::BOLD_YELLOW << "[4]" << Color::RESET << " Vissza\n";

    while (true) {
        cout << "Mód: ";
        int v = readInt();

        if (v >= 1 && v <= 4) return v;
        
        cout << Color::BOLD_RED << "Érvénytelen bemenet!\n" << Color::RESET;
    }
}

// CÉL: Minden input-ot nagybetűsítünk
void normalizeInput(string& input) {
    for (size_t i = 0; i < input.size(); i++) { 
        input[i] = toupper(input[i]); // Végigmegyünk a bemeneten és toupper minden karakterre
    }
}

// CÉL: Sorrendezős válasz validitás ellenőrzése
bool isValidOrderInput(const string& input) {
    if (input.size() != 4) {
        return false;
    }

    for (int i = 0; i < 4; i++) {
        // Csak A, B, C, D lehet
        if (input[i] < 'A' || input[i] > 'D') { // ASCII szám alapján
            return false;
        }

        // Megnézzük, hogy volt-e már ugyanilyen betű korábban
        for (int j = 0; j < i; j++) {
            if (input[i] == input[j]) {
                return false;
            }
        }
    }

    return true;
}

// CÉL: Összekeveri a kérdések sorrendjét
vector<int> buildShuffledOrderIndexes(int count) {
    vector<int> indexes;

    // Feltöltjük: 0, 1, 2, 3 (jövőben flexibilisen updatelhető)
    for (int i = 0; i < count; i++) {
        indexes.push_back(i);
    }

    // Véletlenszám-generátor az aktuális idő alapján
    std::mt19937 rng(time(nullptr));

    // Összekeverjük az indexeket (iterátorokkal)
    std::shuffle(indexes.begin(), indexes.end(), rng);

    return indexes;
}

// CÉL: Szint fejlécének kiírása
void printLevelHeader(int level) {
    int questionNumber = level + 1;
    int prize = Game::PRIZE_LADDER[level];

    cout << SEP << "\n";

    cout << "  "
         << Color::BOLD_YELLOW << questionNumber << ". KÉRDÉS"
         << Color::RESET;

    cout << "  |  Tét: "
         << Color::BOLD_GREEN << Game::formatPrize(prize) << " Ft"
         << Color::RESET << "\n";

    cout << SEP << "\n";
}


int pickQuestionType(int mode, bool hasChoose, bool hasOrder, int level) {
    if (mode == 1) return 1; // Feleletválasztós
    if (mode == 2) return 2; // Sorrendezős
    
    if (!hasOrder)  return 1; // Feleletválasztós
    if (!hasChoose) return 2; // Sorrendezős

    // Vegyes módban minden 3. szint sorrendezős
    return ((level + 1) % 3 == 0) ? 2 : 1;
}

void printGameResult(
    bool walkAway,              // Feladta?
    bool gameOver,              // Veszített?
    int currentLevel,           // Jelenlegi szint
    const string& playerName,   // Játékos neve
    int finalPrize) {           // Végső nyeremény (Nem itt számoljuk ki)

    string prize = Game::formatPrize(finalPrize) + " Ft";

    if (walkAway) {
        cout << "\nMegálltál. Nyereményed: ";
    }

    else if (!gameOver && currentLevel >= Game::LEVELS) {
        cout << "\n" << Color::BOLD_YELLOW
             << "Gratulálok, " << playerName << "!\n"
             << Color::RESET;

        cout << "Megnyerted a főnyereményt: ";
    }
    else {
        cout << "\nJáték vége. Nyereményed: ";
    }

    cout << Color::BOLD_GREEN << prize << Color::RESET << "\n";
}

// Visszaadja hogy a játékos nem elrejtett választ választott
bool isHiddenBy5050(int selectedIndex, int hidden0, int hidden1) {
    return selectedIndex == hidden0 || selectedIndex == hidden1;
}

} // NAMESPACE
  // Mivel így csak itt láthatók, ugyanis ezek csak segédfüggvények

//! ---------- KONSTANSOK ----------

const int Game::PRIZE_LADDER[Game::LEVELS] = {
    5000, 10000, 20000, 50000, 100000, 200000,
    300000, 500000, 800000, 1500000, 3000000, 5000000
};

//! ---------- KONSTRUKTOR ----------

Game::Game():
    hasChoose(false),   // Létezik feleletválasztós kérdés?
    hasOrder(false),    // Létezik sorrendezős kérdés?
    currentLevel(0),    // Aktuális szint
    finalPrize(0),      // Végső nyeremény
    gameOver(false),    // Veszített?
    walkAway(false),    // Feladta?
    used5050(false),    // Használta az 50:50 segítséget?
    usedAudience(false) // Használta a közönség segítséget?
{
    srand(time(nullptr)); // majd rand()-dal használjuk
    hiddenResponses[0] = -1;
    hiddenResponses[1] = -1;
    audienceValues[0]  = 0;
    audienceValues[1]  = 0;
    audienceValues[2]  = 0;
    audienceValues[3]  = 0;
}

//! ---------- KÉRDÉSEK BETÖLTÉSE ----------


// CÉL: Kérdések betöltése fileokból, majd szintek szerint eltárolása
void Game::loadQuestions(const string& chooseFile, const string& orderFile) {

    // Feleletválasztós kérdések, temp változóba, ömlesztve
    vector<ChooseQuestion> temp = FileManager::loadChooseQuestions(chooseFile);

    // Sorrendezős kérdések
    orderQuestions = FileManager::loadOrderQuestions(orderFile);

    // Feleletválasztós kérdések vektorának kiürítése
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
    clearScreen();
    playerName = getPlayerName();

    vector<int> orderIndexes = buildShuffledOrderIndexes(orderQuestions.size());
    std::size_t orderCursor  = 0;

    for (currentLevel = 0; currentLevel < LEVELS && !gameOver; ++currentLevel) {
        int questionType = pickQuestionType(mode, hasChoose, hasOrder, currentLevel);

        if (questionType == 1) {
            if (chooseQuestions[currentLevel].empty()) {
                cout << "Nincs elég kérdés ezen a szinten, a játék leáll.\n";
                gameOver   = true;
                finalPrize = getSafePrize(currentLevel - 1);
                break;
            }
            int idx = rand() % static_cast<int>(chooseQuestions[currentLevel].size());
            askChooseQuestion(chooseQuestions[currentLevel][idx]);
        } else {
            if (orderIndexes.empty()) {
                cout << "Nincs sorrendezős kérdés, a játék leáll.\n";
                gameOver   = true;
                finalPrize = getSafePrize(currentLevel - 1);
                break;
            }
            if (orderCursor >= orderIndexes.size()) orderCursor = 0;
            OrderQuestion& q = orderQuestions[orderIndexes[orderCursor]];
            ++orderCursor;
            askOrderQuestion(q);
        }

        if (!gameOver) finalPrize = PRIZE_LADDER[currentLevel];
    }

    clearScreen();
    printGameResult(walkAway, gameOver, currentLevel, playerName, finalPrize);
    hsTable.add(playerName, finalPrize);
    waitEnter();
}

void Game::askChooseQuestion(ChooseQuestion& q) {
    hiddenResponses[0] = -1;
    hiddenResponses[1] = -1;
    bool audienceThisQuestion = false; // ne mutasson százalékot a következő kérdésnél

    while (!gameOver) {
        clearScreen();
        printLevelHeader(currentLevel);

        // Kérdés kiírása, esetleges 50:50 elrejtésekkel
        q.displayWithHints(
            hiddenResponses[0],
            hiddenResponses[1],
            audienceThisQuestion ? audienceValues : nullptr
        );

        printSeparator();

        cout << "\nLehetőségek:\n";
        cout << "   A / B / C / D\n";
        if (!used5050) {
            cout << "   F - 50:50\n";
        } else {
            cout << "   " << Color::GREY_STRIKE << "F - 50:50" << Color::RESET << "\n";
        }
        if (!usedAudience) {
            cout << "   K - Közönség\n";
        } else {
            cout << "   " << Color::GREY_STRIKE << "K - Közönség" << Color::RESET << "\n";
        }
        cout << "   Q - Megállás\n";
        cout << "\nVálasz: ";

        string input;
        getline(cin, input);

        if (input.empty()) continue;
        normalizeInput(input);

        if (input == "Q") {
            walkAway = true;
            gameOver = true;
            return;
        }

        if (input == "F") {
            if (used5050) {
                cout << "Az 50:50 már fel lett használva.\n\n";
            } else {
                apply5050(q);
            }
            continue;
        }

        if (input == "K") {
            if (usedAudience) {
                cout << "A közönségsegítség már fel lett használva.\n\n";
            } else {
                applyAudience(q);
                audienceThisQuestion = true;
            }
            continue;
        }

        if (input.size() != 1 || input[0] < 'A' || input[0] > 'D') {
            cout << "Érvénytelen válasz, próbáld újra.\n\n";
            continue;
        }

        int selectedIndex = input[0] - 'A'; //ASCII miatt
        if (isHiddenBy5050(selectedIndex, hiddenResponses[0], hiddenResponses[1])) {
            cout << "Ez az opció már el van rejtve az 50:50 miatt.\n\n";
            continue;
        }

        if (q.checkAnswer(input)) {
            cout << Color::BOLD_GREEN << "\nHelyes válasz!\n\n" << Color::RESET;
            return;
        }

        cout << Color::BOLD_RED << "\nRossz válasz." << Color::RESET
             << " A helyes válasz: " << Color::BOLD_YELLOW << q.getCorrectAnswer()
             << Color::RESET << "\n";
        gameOver   = true;
        finalPrize = getSafePrize(currentLevel - 1);
        return;
    }
}

void Game::askOrderQuestion(OrderQuestion& q) {
    while (!gameOver) {
        clearScreen();
        printLevelHeader(currentLevel);
        q.display();
        printSeparator();

        cout << "\nAdd meg a sorrendet 4 betűvel (pl. ABDC)\n";
        cout << "   Q - Megállás\n";
        cout << "\nVálasz: ";

        string input;
        getline(cin, input);
        if (input.empty()) continue;
        normalizeInput(input);

        if (input == "Q") {
            walkAway = true;
            gameOver = true;
            return;
        }

        if (!isValidOrderInput(input)) {
            cout << "A válasz csak A, B, C, D betűket tartalmazhat, ismétlés nélkül.\n\n";
            continue;
        }

        if (q.checkAnswer(input)) {
            cout << Color::BOLD_GREEN << "\nHelyes válasz!\n\n" << Color::RESET;
            return;
        }

        cout << Color::BOLD_RED << "\nRossz válasz." << Color::RESET
             << " A helyes sorrend: " << Color::BOLD_YELLOW << q.getCorrectAnswer()
             << Color::RESET << "\n";
        gameOver   = true;
        finalPrize = getSafePrize(currentLevel - 1);
        return;
    }
}

//! ---------- SEGÍTSÉGEK ----------

void Game::apply5050(ChooseQuestion& q) {
    used5050 = true;

    string correct = q.getCorrectAnswer();
    if (correct.empty()) return;

    int correctIdx = static_cast<int>(std::toupper(static_cast<unsigned char>(correct[0])) - 'A');
    vector<int> wrongIndexes;
    for (int i = 0; i < 4; ++i) {
        if (i != correctIdx) wrongIndexes.push_back(i);
    }
    if (wrongIndexes.size() < 2) return;

    int first = rand() % static_cast<int>(wrongIndexes.size());
    hiddenResponses[0] = wrongIndexes[first];
    wrongIndexes.erase(wrongIndexes.begin() + first);
    int second = rand() % static_cast<int>(wrongIndexes.size());
    hiddenResponses[1] = wrongIndexes[second];
}

void Game::applyAudience(ChooseQuestion& q) {
    usedAudience = true;

    string correct = q.getCorrectAnswer();
    if (correct.empty()) return;

    int correctIdx = static_cast<int>(std::toupper(static_cast<unsigned char>(correct[0])) - 'A');

    audienceValues[0] = audienceValues[1] = audienceValues[2] = audienceValues[3] = 0;
    audienceValues[correctIdx] = 40 + (rand() % 31); // 40-70
    int left = 100 - audienceValues[correctIdx];

    vector<int> wrongIndexes;
    for (int i = 0; i < 4; ++i) {
        if (i != correctIdx) wrongIndexes.push_back(i);
    }

    int first  = rand() % (left + 1);
    int second = rand() % (left - first + 1);
    int third  = left - first - second;
    audienceValues[wrongIndexes[0]] = first;
    audienceValues[wrongIndexes[1]] = second;
    audienceValues[wrongIndexes[2]] = third;
}

//! ---------- JÁTÉKÁLLAPOT ----------

void Game::resetGameState() {
    currentLevel       = 0;
    finalPrize         = 0;
    gameOver           = false;
    walkAway           = false;
    used5050           = false;
    usedAudience       = false;
    hiddenResponses[0] = -1;
    hiddenResponses[1] = -1;
    audienceValues[0]  = audienceValues[1] = audienceValues[2] = audienceValues[3] = 0;
}

string Game::getPlayerName() {
    while (true) {
        cout << "Játékos neve (max 20 karakter): ";
        string name;
        getline(cin, name);

        if (name.empty()) return "Jatekos";

        // UTF-8-tudatos látható hossz számítás
        std::size_t visLen = 0;
        for (unsigned char c : name) {
            if ((c & 0xC0) != 0x80) ++visLen;
        }

        if (visLen > 20) {
            cout << Color::BOLD_RED << "A játékos neve túl hosszú! (max 20 karakter)\n" << Color::RESET;
            continue;
        }
        return name;
    }
}

//! ---------- STATIKUS SEGÉDESZKÖZÖK ----------

string Game::formatPrize(int prize) {
    string number = std::to_string(prize);
    string formatted;
    int count = 0;
    for (int i = static_cast<int>(number.size()) - 1; i >= 0; --i) {
        formatted.insert(formatted.begin(), number[static_cast<std::size_t>(i)]);
        ++count;
        if (count == 3 && i != 0) {
            formatted.insert(formatted.begin(), '.');
            count = 0;
        }
    }
    return formatted;
}

bool Game::isSafeLevel(int index) {
    return index == 4 || index == 9;
}

int Game::getSafePrize(int index) {
    if (index >= 9) return PRIZE_LADDER[9];
    if (index >= 4) return PRIZE_LADDER[4];
    return 0;
}

void Game::printSeparator() {
    cout << "────────────────────────────────────────────────────────────────\n";
}

void Game::waitEnter() {
    cout << "\n[Nyomj Entert a folytatáshoz...]";
    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

void Game::clearScreen() {
    cout << "\033[2J\033[H"; // https://stackoverflow.com/questions/35813318/how-to-refresh-terminal-page-in-c
}
