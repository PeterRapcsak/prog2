/*======================================================================
    highscore.cpp - Dicsőséglista kezelése
----------------------------------------------------------------------
    FELADAT:
     - HighScoreEntry: egyetlen dicsőséglista bejegyzés
         - játékos neve
         - eredmény dátuma
         - elért nyeremény
         
     - HighScoreTable: teljes dicsőséglista kezelése
         - CSV-ből betöltés
         - CSV-be mentés
         - új eredmény hozzáadása
         - lista törlése
         - rendezett kiírás konzolra

     - CSV formátum:
         Nev;Nyeremeny;Datum

     - A nyeremény szebb kiírása ezres tagolással
     - UTF-8 karakterek kezelése a táblázatos kiírásnál
======================================================================*/

#include "highscore.h"
#include "colors.h"

#include <algorithm> // sort

#include <ctime>     // time, localtime, strftime
#include <iomanip>  // setw, setfill

#include <fstream>   // ifstream, ofstream
#include <iostream>  // konzol kiírás
#include <sstream>   // stringstream

// UTF-8 karakterek kezelése 
#include <codecvt> // codecvt_utf8
#include <locale>  // wstring_convert

using std::string;
using std::vector;
using std::cout;

namespace {

// "Majd kelleni fog" dolgok
static const string SEP = "────────────────────────────────────────────────────────────────";

//! ---------- SEGÉDFÜGGVÉNYEK ----------

// CÉL: Nyeremény szebb kiírása ezres tagolással
// Példa: 1500000 -> 1.500.000
string formatPrize(int prize) {
    string number = std::to_string(prize);
    string formatted;

    int count = 0;
    int size = static_cast<int>(number.size());

    // Hátulról előre haladunk, és közben a formatted végére pakolunk
    for (int i = size - 1; i >= 0; --i) {

        // Az aktuális számjegyet hozzáadjuk a végéhez
        formatted += number[i];
        ++count;

        // Ha megvolt 3 számjegy és még nem értünk a szám elejére, jöhet a pont
        if (count == 3 && i != 0) {
            formatted += '.';
            count = 0;
        }
    }

    // Mivel hátulról építettük fel, a végén meg kell fordítani
    std::reverse(formatted.begin(), formatted.end());

    return formatted;
}

// CÉL: UTF-8 string látható hosszának kiszámolása
// MEGJEGYZÉS:
//      Az ékezetes karakter több byte is lehet.
std::size_t visibleLength(const string& text) {
    std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
    // https://stackoverflow.com/questions/42739129/where-to-put-stdwstring-convertstdcodecvt-utf8wchar-t

    std::wstring wideText = converter.from_bytes(text);
    
    // A wide string size() már karaktereket számol, nem byteokat
    return wideText.size();
}

// CÉL: String kiegészítése szóközökkel a kívánt szélességre
// MEGJEGYZÉS:
//      Táblázatos kiírásnál kell, hogy az oszlopok ne csússzanak szét.
string visiblePad(const string& text, std::size_t width) {
    string result = text;
    std::size_t length = visibleLength(text);

    // Addig rakunk mögé szóközt, ameddig el nem érjük a kívánt szélességet
    while (length < width) {
        result += ' ';
        ++length;
    }

    return result;
}

// CÉL: Aktuális dátum lekérése stringként
// Példa: 2026/05/09
// Reference: https://stackoverflow.com/questions/997946/how-can-i-get-current-time-and-date-in-c
string currentDate() {
    std::time_t t = std::time(nullptr); // aktuális idő lekérése
    std::tm* now = std::localtime(&t);  // átalakítás helyi időre

    // Ha valamiért nem sikerült lekérni az időt, üres stringet adunk vissza
    if (now == nullptr) {
        return "";
    }

    std::stringstream ss;

    // tm_year: 1900 óta eltelt évek száma, ezért kell +1900
    ss << (now->tm_year + 1900) << "/"

       // tm_mon: 0-tól indul, tehát január = 0, ezért kell +1
       // setw és setfill: a hónap mindig 2 számjegy, tehát 1 -> 01
       << std::setw(2) << std::setfill('0') << (now->tm_mon + 1) << "/"

       // tm_mday: hónap napja, ez már normálisan 1-től indul
       << std::setw(2) << std::setfill('0') << now->tm_mday;

    return ss.str();
}

} // namespace


//! ---------- BEJEGYZÉS ----------

// CÉL: Egyetlen dicsőséglista bejegyzés létrehozása
HighScoreEntry::HighScoreEntry(
    const string& name,  // játékos neve
    const string& date,  // eredmény dátuma
    int prize)           // elért nyeremény
    : name(name),
      date(date),
      prize(prize)
{}

// CÉL: Játékos nevének lekérése
const string& HighScoreEntry::getName() const {
    return name;
}

// CÉL: Dátum lekérése
const string& HighScoreEntry::getDate() const {
    return date;
}

// CÉL: Nyeremény lekérése
int HighScoreEntry::getPrize() const {
    return prize;
}

// CÉL: Csökkenő nyeremény szerinti összehasonlítás
bool HighScoreEntry::operator>(const HighScoreEntry& rhs) const {
    return prize > rhs.prize;
}


//! ---------- LISTA ----------

/*
    CÉL: Dicsőséglista tábla létrehozása
    MEGJEGYZÉS:
        A konstruktor azonnal be is tölti a CSV-t.
*/
HighScoreTable::HighScoreTable(const string& filename)
    : filename(filename)
{
    load();
}

// CÉL: Bejegyzések beolvasása CSV fájlból
void HighScoreTable::load() {
    entries.clear();

    std::ifstream inputFile(filename); // input file stream
    if (!inputFile.is_open()) {
        return; // nem nyitható meg -> üres lista
    }

    string line;
    std::getline(inputFile, line); // első sort kihagyjuk

    while (std::getline(inputFile, line)) {
        std::stringstream ss(line);
        string field;
        vector<string> fields;

        // Pontosvesszők alapján darabolás (honnan, hová, meddig)
        while (std::getline(ss, field, ';')) {
            fields.push_back(field); // egyesével a vektor végére
        }

        if (fields.size() != 3) {
            continue; // hibás sor (nem 3 adat) -> kihagyjuk
        }

        // Nyeremény szám konverzió
        int prize = 0;
        try {
            prize = std::stoi(fields[1]);
        } catch (...) { // MINDENT elkapunk
            continue; // nem számmá alakítható -> kihagyjuk
        }

        // fields[0] - név
        // fields[1] - nyeremény
        // fields[2] - dátum
        entries.emplace_back(fields[0], fields[2], prize);
        // Nem hívja a copy konstruktort, az entries-ben hozza létre az új rekordot
        // igen tudom majdnem teljesen ugyanaz mint a push_back() de marginally gyorsabb
    }
}

// CÉL: Az összes bejegyzés visszaírása CSV fájlba
void HighScoreTable::save() const {
    std::ofstream file(filename);

    // Ha valamiért nem írható a fájl, akkor nem omlik össze a program
    if (!file.is_open()) {
        return;
    }

    // Fejléc sor
    file << "Nev;Nyeremeny;Datum\n";

    // Minden rekordot újra kiírunk
    // Ez direkt teljes felülírás, nem csak hozzáfűzés
    for (std::size_t i = 0; i < entries.size(); ++i) {
        file << entries[i].getName()  << ';'
             << entries[i].getPrize() << ';'
             << entries[i].getDate()  << '\n';
    }
}

/*
    CÉL: Új eredmény hozzáadása az aktuális dátummal
    BE: name  - játékos neve
        prize - elért nyeremény
*/
void HighScoreTable::add(const string& name, int prize) {

    // MIVEL name -> const, ezért temp változó kell
    string temp = name;
    if (name == "" || name == " " || name == "\t") temp = "Játékos";

    // Új rekord felvétele memóriába
    entries.emplace_back(temp, currentDate(), prize);

    // Azonnal mentjük, hogy kilépéskor se vesszen el
    save();
}

// CÉL: Dicsőséglista törlése memóriából és fájlból
void HighScoreTable::reset() {
    entries.clear();

    // Üres lista mentése -> csak a fejléc marad a CSV-ben
    save();
}

// CÉL: Dicsőséglista rendezett kiírása a konzolra
void HighScoreTable::display() const {

    // Másolatot rendezünk, hogy az eredeti entries sorrend ne változzon
    vector<HighScoreEntry> sorted = entries;

    // https://stackoverflow.com/questions/5122804/how-to-sort-with-a-lambda
    std::sort(sorted.begin(), sorted.end(),
              [](const HighScoreEntry& a, const HighScoreEntry& b) {
                  return a.getPrize() > b.getPrize();
              });

    //? HEADER
    cout << Color::BOLD_YELLOW << "                     ─── DICSŐSÉGLISTA ───\n" << Color::RESET;
    cout << SEP << "\n";

    cout << "  " << visiblePad("#",         5)
                << visiblePad("Játékos",   22)
                << visiblePad("Nyeremény", 16)
                << "Dátum\n";

    cout << SEP << "\n";

    // Ha még nincs eredmény, akkor is szép táblát írunk ki
    if (sorted.empty()) {
        cout << "  Még nincs mentett eredmény.\n";
        cout << SEP << "\n";
        return;
    }

    //? TÁBLA
    for (std::size_t i = 0; i < sorted.size(); ++i) {

        // A név beíráskor már max 20 karakterre van limitálva
        string name = sorted[i].getName();

        // Nyeremény szöveg, pld: 1.500.000 Ft
        string prizeText = formatPrize(sorted[i].getPrize()) + " Ft";

        cout << "  " << visiblePad(std::to_string(i + 1) + ".", 5)
                    << visiblePad(name, 22)
                    << Color::BOLD_GREEN
                    << visiblePad(prizeText, 16)
                    << Color::RESET
                    << sorted[i].getDate()
                    << "\n";
    }

    cout << SEP << "\n";
}
