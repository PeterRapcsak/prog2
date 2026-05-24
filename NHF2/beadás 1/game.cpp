/*======================================================================
    game.cpp - Játékmenet logikája
----------------------------------------------------------------------
    FELADAT:
     - mainMenu() és play() működése
     - 12 szintnyi játék, szintenként kérdés választás
     - Opciók (CHOOSE típusnál):
         - 50:50    | 2 helytelen választ eltüntet (a Question kezeli)
         - közönség | Pseudo-random százalékos eloszlással segít (LEHET HOGY FÉLREVEZET)
         - feladás  | A játék feladása és a nyeremény megtartása
     - Válaszellenőrzés, nyeremenylista, biztos szintek (5., 10.)
     - Játékos neve, aktuális szint, végső nyeremény kezelése
======================================================================*/

//! Változtatások:
//!  - A kérdezést maga a Question ask() metódusa végzi, innen csak meghívjuk
//!  - rng, reseedRng, randomInt, readInt, normalizeInput, printLevelHeader -> Game oszály PUBLIKUS metódusai
//!    igazából tényleg sokkal logikusabb így, valamiért amikor írtam akkor csak simán gondoltam, hogy ezek globális segédfüggvények lesznek...
//!  - pickQuestionType, printGameResult, showHighScoreMenu, runOneLevel -> Game osztály PRIVÁT metódusai
//!  - felesleges itt a namespace akkor.
//!  - isValidOrderInput -> OrderQuestion osztály PRIVÁT metódusa

#include "memtrace.h" // memóriaszivárgás check

#include "game.h"
#include "filemanager.h"
#include "colors.h"

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


//! ---------- KONSTANSOK ----------

const int Game::PRIZE_LADDER[Game::LEVELS] = {
    5000, 10000, 25000, 50000, 100000, 200000,
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

        //? FŐMENÜ KÉPERNYŐ
        //? mivel úgyis csak 1 helyen használjuk, ezért tök fölösleges külön segédfüggvénybe tenni 
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

        //? MENÜ VÁLASZTÁS (1-3, hibás bemenet -> újrakérdés)
        int choice = -1;
        while (choice < 1 || choice > 3) {
            cout << "Valasztas: ";
            choice = readInt();
            if (choice < 1 || choice > 3) {
                cout << Color::BOLD_RED << "Érvénytelen bemenet!\n" << Color::RESET;
            }
        }

        switch (choice) {

            //? ÚJ JÁTÉK
            case 1: {
                //! Játékmód választás (1-4, hibás bemenet -> újrakérdés)
                //? őt is fölösleges külön írni
                cout << "\nJátékmódok:\n";
                cout << "  " << Color::BOLD_YELLOW << "[1]" << Color::RESET << " Feleletválasztós\n";
                cout << "  " << Color::BOLD_YELLOW << "[2]" << Color::RESET << " Sorrendezős\n";
                cout << "  " << Color::BOLD_YELLOW << "[3]" << Color::RESET << " Vegyes\n";
                cout << "  " << Color::BOLD_YELLOW << "[4]" << Color::RESET << " Vissza\n";

                int mode = -1;
                while (mode < 1 || mode > 4) {
                    cout << "Mód: ";
                    mode = readInt();
                    if (mode < 1 || mode > 4) {
                        cout << Color::BOLD_RED << "Érvénytelen bemenet!\n" << Color::RESET;
                    }
                }

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

            //? DICSŐSÉGLISTA
            case 2:
                showHighScoreMenu();
                break;

            //? KILÉPÉS
            case 3:
                exitMenu = true;
                break;
        }
    }
}

//! ---------- DICSŐSÉGLISTA ALMENÜ ----------
//? Őt is game:: metódus lett, logikusabb így
void Game::showHighScoreMenu() {
    hsTable.load();

    clearScreen();
    hsTable.display();

    cout << "\n  " << Color::BOLD_YELLOW << "[1]" << Color::RESET << " Vissza\n";
    cout << "  "   << Color::BOLD_YELLOW << "[2]" << Color::RESET << " Lista törlése\n";

    while (true) {
        cout << "\nValasztas: ";
        int v = readInt();

        if (v == 1) {
            return;
        }

        if (v == 2) {
            hsTable.reset();
            cout << Color::BOLD_RED << "A dicsőséglista törölve.\n" << Color::RESET;
            waitEnter();
            return;
        }

        cout << Color::BOLD_RED << "Érvénytelen bemenet!\n" << Color::RESET;
    }
}

//! ---------- JÁTÉKMENET ----------

void Game::play(int mode) {
    resetGameState();
    reseedRng();
    clearScreen();

    playerName = getPlayerName();

    //? Változtatás: kivittem innen a logikát egy runOneLevel() metódusba, hogy ne legyen ilyen hosszú a play() (+ messze sokkal áttekinthetőbb)
    // A sorrendezős kérdések összekeverése
    // Vektorban int-ek, kell cast mert különben size_t lenne
    int orderCount = static_cast<int>(orderQuestions.size());

    vector<int> orderQuestionIndexes;

    for (int i = 0; i < orderCount; i++) {
        orderQuestionIndexes.push_back(i);
    }

    std::shuffle(orderQuestionIndexes.begin(), orderQuestionIndexes.end(), rng());

    std::size_t nextOrderQuestion = 0;

    // Végigmegyünk a szinteken
    for (currentLevel = 0; currentLevel < LEVELS && !gameOver; ++currentLevel) {
        bool ok = runOneLevel(mode, orderQuestionIndexes, nextOrderQuestion);

        if (!ok) {
            break; // a runOneLevel már beállította a gameOver / finalPrize-t
        }

        // Ha a kérdés után még nem lett vége a játéknak,
        // akkor a játékos elérte az aktuális szint nyereményét
        finalPrize = PRIZE_LADDER[currentLevel];
    }

    // Játék vége: képernyő törlése, eredmény kiírása, pontszám mentése
    clearScreen();
    printGameResult();

    hsTable.add(playerName, finalPrize); // Dátumot ott számolunk

    waitEnter();
}

//! ---------- EGY SZINT LEJÁTSZÁSA ----------

/*
    CÉL: Egy szint lebonyolítása, akármilyen kérdéstípussal
    KI: true  = sikeresen átment a szinten
        false = vége a játéknak (rossz válasz, megállás, nincs kérdés)
*/
bool Game::runOneLevel(
    int mode,
    vector<int>& orderQuestionIndexes,
    std::size_t& nextOrderQuestion) {

    QuestionType questionType = pickQuestionType(mode, currentLevel);

    // A polymorph hívás miatt elég egy pointer az ősosztályhoz
    Question* current = nullptr;

    switch (questionType) {

        //? FELELETVÁLASZTÓS KÉRDÉS
        case ChooseQuestionType: {

            // Egyik leghosszabb változónév amit valaha adtam
            // Megadja hogy van-e választós kérdés egy adott szinten
            bool noChooseQuestionOnThisLevel = chooseQuestions[currentLevel].empty();

            if (noChooseQuestionOnThisLevel) {
                cout << "Nincs elég feleletválasztós kérdés ezen a szinten, a játék leáll.\n";
                gameOver = true;
                finalPrize = getSafePrize(currentLevel - 1);
                return false;
            }

            // size_t -> int , ebből random indexet generálunk
            int questionCount = static_cast<int>(chooseQuestions[currentLevel].size());
            int randomIndex = randomInt(0, questionCount - 1);

            current = &chooseQuestions[currentLevel][randomIndex];
            break;
        }

        //? SORRENDEZŐS KÉRDÉS
        case OrderQuestionType: {

            // Megadja hogy van-e sorrendezős kérdés (Nem kell szintet csekkolni)
            bool noOrderQuestions = orderQuestionIndexes.empty();

            if (noOrderQuestions) {
                cout << "Nincs sorrendezős kérdés, a játék leáll.\n";
                gameOver = true;
                finalPrize = getSafePrize(currentLevel - 1);
                return false;
            }

            // Ha elfogytak a sorrendezős kérdések, kezdjük újra az elejéről
            if (nextOrderQuestion >= orderQuestionIndexes.size()) {
                nextOrderQuestion = 0;
            }

            // Egyesével léptetjük őket a MÁR kevert index listán
            int questionIndex = orderQuestionIndexes[nextOrderQuestion];
            ++nextOrderQuestion;

            current = &orderQuestions[questionIndex];
            break;
        }
    }

    // ÉS itt a varázslat - polymorph hívás
    // A Question döntse el, hogy hogy kérdezzen, mi csak az eredményt nézzük
    Question::AskResult result = current->ask(currentLevel, used5050, usedAudience);

    if (result == Question::WALK_AWAY) {
        walkAway = true;
        gameOver = true;
        return false;
    }

    if (result == Question::WRONG) {
        gameOver = true;
        finalPrize = getSafePrize(currentLevel - 1); // -1 mert ugye elbukta a szintet
        return false;
    }

    return true; // CORRECT
}

//! ---------- KÉRDÉSTÍPUS VÁLASZTÁS ----------

Game::QuestionType Game::pickQuestionType(int mode, int level) const {
    if (mode == 1) return ChooseQuestionType; // Feleletválasztós
    if (mode == 2) return OrderQuestionType;  // Sorrendezős

    if (!hasOrder)  return ChooseQuestionType; // Feleletválasztós
    if (!hasChoose) return OrderQuestionType;  // Sorrendezős

    // Vegyes módban a 3. 7. és 11. kérdés sorrendezős, a többi feleletválasztós
    // (azért nem minden 3. kérdés, mert akkor a 12. kérdés is sorrendezős lenne, ami azért tényleg elég nehéz lenne)
    return (level + 1 == 3 || level + 1 == 7 || level + 1 == 11)
           ? OrderQuestionType
           : ChooseQuestionType;
}

//! ---------- EREDMÉNY KIÍRÁS ----------

// CÉL: Kiírja a játék kimenetelét (megállt, nyert, veszített) és a nyereményt
void Game::printGameResult() const {

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

//! ---------- JÁTÉKÁLLAPOT ----------

void Game::resetGameState() {
    currentLevel = 0;
    finalPrize   = 0;
    gameOver     = false;
    walkAway     = false;
    used5050     = false;
    usedAudience = false;
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

//! ---------- STATIKUS SEGÉDESZKÖZÖK ----------

// CÉL: Központi random generátor
std::mt19937& Game::rng() {

    // random_device -> random seed
    // static miatt nem kell ujraindítani minden rng()-nél
    static std::mt19937 generator(std::random_device{}()); // https://stackoverflow.com/questions/39288595/why-not-just-use-stdrandom-device

    // Referenciát adunk vissza, hogy mindig ugyanazt a generátort használjuk
    // NE generáljunk mindig új seedet
    return generator;
}

// CÉL: Uj játéknál új random seed generálása
void Game::reseedRng() {
    // de menő hogy std::mt19937-nek van .seed metódusa :D
    rng().seed(std::random_device{}());
}

// CÉL: Random int generálása két érték között
int Game::randomInt(int min, int max) {

    // Mindkét határ benne van, tehát pld: randomInt(0, 3) -> 0, 1, 2 vagy 3
    std::uniform_int_distribution<int> distribution(min, max);
    // Seed-et használó C++ kód minta:
    // https://en.cppreference.com/cpp/numeric/random/uniform_int_distribution

    // Az előbbi random generátorral kérünk egy számot ebből az eloszlásból
    return distribution(rng());
}

// CÉL: Egész szám olvasása, nem szám bemenet esetén -1
int Game::readInt() {
    int value;

    // Buffer ürítések miatt ilyen komplikált
    if (cin >> value) {
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Ez egy buffer ami kiignorál minden maradék inputot
        // https://stackoverflow.com/questions/25020129/cin-ignorenumeric-limitsstreamsizemax-n

        return value;
    }

    cin.clear(); // cin hibás állapotának clear-elése
    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // ugyanúgy ignorálás
    return -1;
}

// CÉL: Minden input-ot nagybetűsítünk
void Game::normalizeInput(string& input) {
    for (size_t i = 0; i < input.size(); i++) {
        input[i] = toupper(input[i]); // Végigmegyünk a bemeneten és toupper minden karakterre
    }
}

// CÉL: A nyereményt szebben írja ki
// Példa: 1500000 -> 1.500.000
string Game::formatPrize(int prize) {
    string number = std::to_string(prize);
    string formatted;

    int size = static_cast<int>(number.size());
    int count = 0;

    // Hátulról, előre 3-asával egy "." beszúrása
    for (int i = size - 1; i >= 0; --i) {

        formatted.insert(formatted.begin(), number[i]); // Elejére szúr, de mi hátulról megyünk végig
        ++count;

        if (count == 3 && i != 0) {
            formatted.insert(formatted.begin(), '.');
            count = 0;
        }
    }
    return formatted;
}

// Biztonságos szintek: 5. és a 10. szint
bool Game::isSafeLevel(int index) {
    return index == 4 || index == 9;
}

// Ha a játékos veszít, akkor a legutóbbi safe level nyereményét kapja meg
int Game::getSafePrize(int index) {
    if (index >= 9) return PRIZE_LADDER[9];
    if (index >= 4) return PRIZE_LADDER[4];
    return 0;
}

void Game::printSeparator() {
    cout << "────────────────────────────────────────────────────────────────\n";
}

// Addig vár, amíg a játékos Entert nem nyom
void Game::waitEnter() {
    cout << "\n[Nyomj Entert a folytatáshoz...]";

    string temp;
    getline(cin, temp); // Beolvasunk valamit amit nem használunk, de addigis vár a program
}

void Game::clearScreen() {
    cout << "\033[2J\033[H"; // https://stackoverflow.com/questions/35813318/how-to-refresh-terminal-page-in-c
}

// CÉL: Szint fejlécének kiírása
void Game::printLevelHeader(int level) {
    int questionNumber = level + 1;
    int prize = PRIZE_LADDER[level];

    printSeparator();

    cout << "  "
         << Color::BOLD_YELLOW << questionNumber << ". KÉRDÉS"
         << Color::RESET;

    cout << "  |  Tét: "
         << Color::BOLD_GREEN << formatPrize(prize) << " Ft"
         << Color::RESET << "\n";

    printSeparator();
}
