
#include <iostream> 
#include <limits>
#include <random>
#include "memtrace.h"

#include "colors.h"
#include "utils.h"

using std::string;
using std::cout;
using std::endl;
using std::cin;

//! ---------- KONSTANSOK ----------

const int PRIZE_LADDER[LEVELS] = {
    5000, 10000, 25000, 50000, 100000, 200000,
    300000, 500000, 800000, 1500000, 3000000, 5000000
};

//! ---------- SEGÉDFÜGGVÉNYEK ----------


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


// CÉL: Egész szám olvasása, nem szám bemenet esetén -1-et ad vissza
int readInt() {
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


/*
    CÉL: Megmondja, hogy az adott szintindex biztos szint-e
    BE: index
    KI: igaz ha 4 (5. kérdés) vagy 9 (10. kérdés)
*/
bool isSafeLevel(int index) {
    return index == 4 || index == 9;
}

// Ha a játékos veszít, akkor a legutóbbi safe level nyereményét kapja meg
int getSafePrize(int index) {
    if (index >= 9) return PRIZE_LADDER[9];
    if (index >= 4) return PRIZE_LADDER[4];
    return 0;
}

void printSeparator() {
    cout << "────────────────────────────────────────────────────────────────\n";
}

// Addig vár, amíg a játékos Entert nem nyom
void waitEnter() {
    cout << "\n[Nyomj Entert a folytatáshoz...]";

    string temp;
    getline(cin, temp); // Beolvasunk valamit amit nem használunk, de addigis vár a program
}

void clearScreen() {
    cout << "\033[2J\033[H"; // https://stackoverflow.com/questions/35813318/how-to-refresh-terminal-page-in-c
}

/*
    CÉL: Egész szám ezres tagolással formázva
    BE: prize - összeg Ft-ban (pld: 5000000)
    KI: Formázott string (pld: "5.000.000")
*/
string formatPrize(int prize) {
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

// CÉL: Egy központi random generátor
std::mt19937& rng() {

    // random_device -> random seed
    // static miatt nem kell ujraindítani minden rng()-nél
    static std::mt19937 generator(std::random_device{}()); // https://stackoverflow.com/questions/39288595/why-not-just-use-stdrandom-device

    // Referenciát adunk vissza, hogy mindig ugyanazt a generátort használjuk
    // NE generáljunk mindig új seedet
    return generator;
}

// CÉL: Uj játéknál új random seed generálása
void reseedRng() {
    // de menő hogy std::mt19937-nek van .seed metódusa :D
    rng().seed(std::random_device{}());
}


// CÉL: Random int generálása két érték között
int randomInt(int min, int max) {

    // Mindkét határ benne van, tehát pld: randomInt(0, 3) -> 0, 1, 2 vagy 3
    std::uniform_int_distribution<int> distribution(min, max);
    // Seed-et használó C++ kód minta: 
    // https://en.cppreference.com/cpp/numeric/random/uniform_int_distribution

    // Az előbbi random generátorral kérünk egy számot ebből az eloszlásból
    return distribution(rng());
}

// CÉL: Szint fejlécének kiírása
void printLevelHeader(int level) {
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

// CÉL: Minden input-ot nagybetűsítünk
void normalizeInput(string& input) {
    for (size_t i = 0; i < input.size(); i++) { 
        input[i] = toupper(input[i]); // Végigmegyünk a bemeneten és toupper minden karakterre
    }
}

// Visszaadja hogy a játékos nem elrejtett választ választott
bool isHiddenBy5050(int selectedIndex, int hidden0, int hidden1) {
    return selectedIndex == hidden0 || selectedIndex == hidden1;
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
