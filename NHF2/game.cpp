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

static const string SEP =
    "────────────────────────────────────────────────────────────────";

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

// CÉL: Egész szám olvasása; nem-numerikus bemenet esetén -1-et ad vissza
int readInt() {
    int value;
    if (cin >> value) {
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return value;
    }
    cin.clear();
    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return -1;
}

// CÉL: Főmenü választás olvasása (1-3), érvénytelen bemenetnél piros hibaüzenet + újrakérdezés
int readMenuChoice() {
    while (true) {
        cout << "Valasztas: ";
        int v = readInt();
        if (v >= 1 && v <= 3) return v;
        cout << Color::BOLD_RED << "Érvénytelen bemenet!\n" << Color::RESET;
    }
}

// CÉL: Játékmód választás olvasása (1-4), érvénytelen bemenetnél piros hibaüzenet + újrakérdezés
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

void normalizeInput(string& input) {
    for (std::size_t i = 0; i < input.size(); ++i) {
        input[i] = static_cast<char>(std::toupper(static_cast<unsigned char>(input[i])));
    }
}

bool isValidOrderInput(const string& input) {
    if (input.size() != 4) return false;
    bool seen[4] = { false, false, false, false };
    for (int i = 0; i < 4; ++i) {
        if (input[i] < 'A' || input[i] > 'D') return false;
        int idx = input[i] - 'A';
        if (seen[idx]) return false;
        seen[idx] = true;
    }
    return true;
}

vector<int> buildShuffledOrderIndexes(std::size_t count) {
    vector<int> indexes(count);
    for (std::size_t i = 0; i < count; ++i) indexes[i] = static_cast<int>(i);
    std::mt19937 rng(static_cast<unsigned int>(time(nullptr)));
    std::shuffle(indexes.begin(), indexes.end(), rng);
    return indexes;
}

void printLevelHeader(int level) {
    cout << SEP << "\n";
    cout << "  " << Color::BOLD_YELLOW << (level + 1) << ". KÉRDÉS" << Color::RESET
         << "  |  Tét: " << Color::BOLD_GREEN
         << Game::formatPrize(Game::PRIZE_LADDER[level]) << " Ft"
         << Color::RESET << "\n";
    cout << SEP << "\n";
}

int pickQuestionType(int mode, bool hasChoose, bool hasOrder, int level) {
    if (mode == 1) return 1;
    if (mode == 2) return 2;
    if (!hasChoose) return 2;
    if (!hasOrder)  return 1;
    return ((level + 1) % 3 == 0) ? 2 : 1;
}

void printGameResult(bool walkAway, bool gameOver, int currentLevel,
                     const string& playerName, int finalPrize) {
    if (walkAway) {
        cout << "\nMegálltál. Nyerményed: "
             << Color::BOLD_GREEN << Game::formatPrize(finalPrize) << " Ft"
             << Color::RESET << "\n";
        return;
    }
    if (!gameOver && currentLevel >= Game::LEVELS) {
        cout << "\n" << Color::BOLD_YELLOW << "Gratulálok, " << playerName << "!\n" << Color::RESET;
        cout << "Megnyerted a főnyereményt: "
             << Color::BOLD_GREEN << Game::formatPrize(finalPrize) << " Ft"
             << Color::RESET << "\n";
        return;
    }
    cout << "\nJáték vége. Nyerményed: "
         << Color::BOLD_GREEN << Game::formatPrize(finalPrize) << " Ft"
         << Color::RESET << "\n";
}

bool isHiddenBy5050(int selectedIndex, int hidden0, int hidden1) {
    return selectedIndex == hidden0 || selectedIndex == hidden1;
}

} // namespace

//! ---------- KONSTANSOK ----------

const int Game::PRIZE_LADDER[Game::LEVELS] = {
    5000, 10000, 20000, 50000, 100000, 200000,
    300000, 500000, 800000, 1500000, 3000000, 5000000
};

//! ---------- KONSTRUKTOR ----------

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
    srand(static_cast<unsigned int>(time(nullptr)));
    hiddenResponses[0] = -1;
    hiddenResponses[1] = -1;
    audienceValues[0]  = 0;
    audienceValues[1]  = 0;
    audienceValues[2]  = 0;
    audienceValues[3]  = 0;
}

//! ---------- KÉRDÉSEK BETÖLTÉSE ----------

void Game::loadQuestions(const string& chooseFile, const string& orderFile) {
    vector<ChooseQuestion> choose = FileManager::loadChooseQuestions(chooseFile);
    vector<OrderQuestion>  order  = FileManager::loadOrderQuestions(orderFile);

    for (int i = 0; i < LEVELS; ++i) chooseQuestions[i].clear();

    for (std::size_t i = 0; i < choose.size(); ++i) {
        int diff = choose[i].getDifficulty();
        if (diff >= 1 && diff <= LEVELS)
            chooseQuestions[diff - 1].push_back(choose[i]);
    }

    orderQuestions = order;
    hasChoose = !choose.empty();
    hasOrder  = !orderQuestions.empty();
}

//! ---------- FŐMENÜ ----------

void Game::mainMenu() {
    bool exitMenu = false;

    while (!exitMenu) {
        printMainMenuScreen();
        int choice = readMenuChoice();

        switch (choice) {
            case 1: {
                int mode = selectGameMode();
                if (mode == 4) break;
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
            case 2: {
                hsTable.load();
                clearScreen();
                hsTable.display();
                cout << "\n  " << Color::BOLD_YELLOW << "[1]" << Color::RESET << " Vissza\n";
                cout << "  "   << Color::BOLD_YELLOW << "[2]" << Color::RESET << " Lista törlése\n";
                while (true) {
                    cout << "\nValasztas: ";
                    int v = readInt();
                    if (v == 1) break;
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
        q.displayWithHints(hiddenResponses[0], hiddenResponses[1], nullptr);
        if (audienceThisQuestion) {
            cout << Color::CYAN << "  Közönség: ";
            for (int i = 0; i < 4; ++i) {
                cout << static_cast<char>('A' + i) << ": " << audienceValues[i] << "%";
                if (i < 3) cout << "  ";
            }
            cout << Color::RESET << "\n";
        }
        printSeparator();

        cout << "\nVálaszod:\n";
        cout << "    A / B / C / D\n";
        if (!used5050) {
            cout << "    F - 50:50\n";
        } else {
            cout << "    " << Color::GREY_STRIKE << "F - 50:50" << Color::RESET << "\n";
        }
        if (!usedAudience) {
            cout << "    K - Közönség\n";
        } else {
            cout << "    " << Color::GREY_STRIKE << "K - Közönség" << Color::RESET << "\n";
        }
        cout << "    Q - Megállás\n";
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

        int selectedIndex = input[0] - 'A';
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
        cout << "    Q - Megállás\n";
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
        cout << "Játékos neve (max. 20 karakter): ";
        string name;
        getline(cin, name);

        if (name.empty()) return "Jatekos";

        // UTF-8-tudatos látható hossz számítás
        std::size_t visLen = 0;
        for (unsigned char c : name) {
            if ((c & 0xC0) != 0x80) ++visLen;
        }

        if (visLen > 20) {
            cout << Color::BOLD_RED << "A játékos neve túl hosszú! (max. 20 karakter)\n" << Color::RESET;
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
    cout << "\033[2J\033[H";
}
