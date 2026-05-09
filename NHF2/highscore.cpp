/*======================================================================
    highscore.cpp - Dicsőséglista kezelése
----------------------------------------------------------------------
    FELADAT:
     - CSV-ből betöltés (dicsoseglista.csv)
     - Új rekord hozzáadása (név, nyeremény) + automatikus dátum
     - Teljes lista visszaírása CSV-be (felülírja a régit)
     - Rendezés csökkenő nyeremény szerint
     - Konzolra kiírás Unicode szeparátorral és színezett nyereménnyel
======================================================================*/

#include "highscore.h"
#include "colors.h"
#include <algorithm> // std::sort
#include <fstream>   // ifstream, ofstream
#include <iostream>  // std::cout
#include <sstream>   // stringstream
#include <ctime>     // time, localtime, strftime

namespace {

/*
    CÉL: Egész szám ezres tagolással formázott stringgé alakítása
    BE: prize - összeg Ft-ban (pl. 1500000)
    KI: Formázott string (pl. "1.500.000")
*/
std::string formatPrize(int prize) {
    std::string n = std::to_string(prize);
    std::string r;
    int cnt = 0;
    for (int i = static_cast<int>(n.size()) - 1; i >= 0; --i) {
        r.insert(r.begin(), n[static_cast<std::size_t>(i)]);
        if (++cnt == 3 && i != 0) { r.insert(r.begin(), '.'); cnt = 0; } // ezreselválasztó
    }
    return r;
}

/*
    CÉL: String kiegészítése szóközökkel a kívánt látható szélességre (UTF-8 tudatos)
    BE: s     - a kiegészítendő string
        width - a kívánt látható szélesség karakterekben
    KI: String szóközökkel kitöltve a megadott szélességre
    MEGJEGYZÉS: (c & 0xC0) != 0x80 -> c nem UTF-8 folytatóbájt -> látható karakter kezdete
*/
std::string visiblePad(const std::string& s, std::size_t width) {
    std::size_t visLen = 0;
    for (unsigned char c : s) {
        if ((c & 0xC0) != 0x80) ++visLen; // csak a "kezdőbájtokat" számoljuk
    }
    std::string result = s;
    while (visLen < width) { result += ' '; ++visLen; } // szóközök hozzáadása
    return result;
}

} // namespace

//! ---------- BEJEGYZÉS OSZTÁLY ----------

/*
    CÉL: Dicsőséglista bejegyzés létrehozása
    BE: name  - játékos neve
        date  - dátum stringként (pl. "2026/05/09")
        prize - elért nyeremény Ft-ban
*/
HighScoreEntry::HighScoreEntry(const std::string& name, const std::string& date, int prize)
    : name(name), date(date), prize(prize) {}

const std::string& HighScoreEntry::getName()  const { return name;  }
const std::string& HighScoreEntry::getDate()  const { return date;  }
int                HighScoreEntry::getPrize() const { return prize; }

/*
    CÉL: Csökkenő nyeremény szerinti összehasonlítás (std::sort-hoz)
    BE: rhs - a másik bejegyzés
    KI: true ha ez a bejegyzés nagyobb nyereményű mint rhs
*/
bool HighScoreEntry::operator>(const HighScoreEntry& rhs) const {
    return prize > rhs.prize;
}

//! ---------- DICSŐSÉGLISTA TÁBLA ----------

/*
    CÉL: Tábla létrehozása – konstruktor azonnal betölti a fájlt
    BE: filename - CSV fájl neve (alapértelmezett: "dicsoseglista.csv")
*/
HighScoreTable::HighScoreTable(const std::string& filename)
    : filename(filename)
{
    load(); // konstruktorban rögtön betöltjük, hogy mindig aktuális legyen
}

/*
    CÉL: Bejegyzések beolvasása CSV fájlból
    CSV formátum: "Nev;Nyeremeny;Datum" (első sor fejléc – kihagyjuk)
    MEGJEGYZÉS: Hibás sorokat (hiányzó mező, nem számmá alakítható nyeremény) kihagyjuk
*/
void HighScoreTable::load() {
    entries.clear();

    std::ifstream in(filename);
    if (!in.is_open()) return; // nincs fájl -> üres lista, nem hiba

    std::string line;
    std::getline(in, line); // fejléc sor kihagyása

    while (std::getline(in, line)) {
        std::stringstream ss(line);
        std::string name, prizeStr, date;

        // Pontosvessző alapján darabolás
        if (!std::getline(ss, name,     ';')) continue; // hiányzó mező -> sor kihagyása
        if (!std::getline(ss, prizeStr, ';')) continue;
        if (!std::getline(ss, date,     ';')) continue;

        int prize = 0;
        try { prize = std::stoi(prizeStr); } catch (...) { continue; } // nem szám -> kihagyjuk

        entries.emplace_back(name, date, prize);
        // emplace_back: közvetlenül a vektorban konstruálja meg az objektumot
        // (nem hívja a copy konstruktort, marginálisan gyorsabb a push_back-nél)
    }
}

/*
    CÉL: Az összes bejegyzés teljes visszaírása CSV fájlba (felülírja a régit)
    MEGJEGYZÉS: Mindig az összes rekordot írjuk ki újra, nem csak az újakat –
                ezzel garantáljuk, hogy törlés után a fájl is frissül
*/
void HighScoreTable::save() const {
    std::ofstream out(filename);
    if (!out.is_open()) return; // nem írható fájl -> csendben kihagyjuk

    out << "Nev;Nyeremeny;Datum\n"; // fejléc sor

    for (std::size_t i = 0; i < entries.size(); ++i) {
        out << entries[i].getName()  << ';'
            << entries[i].getPrize() << ';'
            << entries[i].getDate()  << "\n";
    }
}

/*
    CÉL: Új eredmény hozzáadása az aktuális dátummal és azonnali mentése
    BE: name  - játékos neve
        prize - elért nyeremény Ft-ban
*/
void HighScoreTable::add(const std::string& name, int prize) {
    std::time_t now = std::time(nullptr);
    std::tm* tmNow  = std::localtime(&now); // helyi idő struktúra

    char dateBuffer[16] = {0};
    if (tmNow != nullptr)
        std::strftime(dateBuffer, sizeof(dateBuffer), "%Y/%m/%d", tmNow); // pl. "2026/05/09"

    entries.emplace_back(name, dateBuffer, prize);
    save(); // azonnal fájlba írjuk, hogy kilépéskor se vesszen el az eredmény
}

// CÉL: Lista törlése memóriából és fájlból (csak a fejléc marad a CSV-ben)
void HighScoreTable::reset() {
    entries.clear();
    save(); // üres vektorral hívja a save()-t -> csak a fejléc íródik ki
}

/*
    CÉL: Dicsőséglista rendezett kiírása a konzolra (csökkenő nyeremény szerint)
    MEGJEGYZÉS: Másolatot rendezünk, az eredeti entries[] sorrend változatlan marad
*/
void HighScoreTable::display() const {
    std::vector<HighScoreEntry> sorted = entries;       // másolat a rendezéshez
    std::sort(sorted.begin(), sorted.end(),
              [](const HighScoreEntry& a, const HighScoreEntry& b) {
                  return a.getPrize() > b.getPrize();   // csökkenő sorrend
              });

    static const std::string SEP =
        "────────────────────────────────────────────────────────────────";

    // Fejléc kiírása
    std::cout << Color::BOLD_YELLOW << "\t─── DICSŐSÉGLISTA ───\n" << Color::RESET;
    std::cout << SEP << "\n";
    std::cout << "  " << visiblePad("#",         5)   // sorszám oszlop (pl. "1.   ")
                      << visiblePad("Játékos",   22)  // névoszlop (max 20 karakter + 2 padding)
                      << visiblePad("Nyeremény", 16)  // nyeremény oszlop (pl. "5.000.000 Ft")
                      << "Dátum\n";
    std::cout << SEP << "\n";

    for (std::size_t i = 0; i < sorted.size(); ++i) {
        std::string name = sorted[i].getName();

        // UTF-8-tudatos csonkítás 20 látható karakterre
        // (a dicsőséglistában max 20 karaktert tárolunk, de a getPlayerName() is 20-ban limitál)
        std::size_t visLen = 0, byteEnd = 0;
        for (; byteEnd < name.size(); ++byteEnd) {
            if ((static_cast<unsigned char>(name[byteEnd]) & 0xC0) != 0x80) {
                if (visLen == 20) break; // 20. látható karakter után leállunk
                ++visLen;
            }
        }
        name = name.substr(0, byteEnd);

        std::string prizeText = formatPrize(sorted[i].getPrize()) + " Ft";

        std::cout << "  " << visiblePad(std::to_string(i + 1) + ".", 5)  // sorszám
                          << visiblePad(name, 22)                          // név (paddelt)
                          << Color::BOLD_GREEN << visiblePad(prizeText, 16) << Color::RESET // zöld nyeremény
                          << sorted[i].getDate() << "\n";
    }

    std::cout << SEP << "\n";
}
