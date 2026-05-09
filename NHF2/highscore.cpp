/*======================================================================
    highscore.cpp - Dicsőséglista kezelése
----------------------------------------------------------------------
    FELADAT:
     - CSV-ből betöltés (dicsoseglista.csv)
     - Új rekord hozzáadása (név, nyeremény) + számított dátum
     - CSV-be mentés
     - Rendezés csökkenő nyeremény szerint
======================================================================*/

#include "highscore.h"
#include "colors.h"
#include <algorithm> // std::sort
#include <fstream>   // ifstream, ofstream
#include <iostream>  // std::cout
#include <sstream>   // stringstream
#include <ctime>     // time, localtime, strftime

namespace {

// CÉL: Szám formázása ezres elválasztóval (pl. 5000000 -> 5.000.000)
std::string formatPrize(int prize) {
    std::string n = std::to_string(prize);
    std::string r;
    int cnt = 0;
    for (int i = static_cast<int>(n.size()) - 1; i >= 0; --i) {
        r.insert(r.begin(), n[static_cast<std::size_t>(i)]);
        if (++cnt == 3 && i != 0) { r.insert(r.begin(), '.'); cnt = 0; }
    }
    return r;
}

// CÉL: Szöveg kiegészítése szóközökkel a kívánt látható szélességre (UTF-8 tudatos)
std::string visiblePad(const std::string& s, std::size_t width) {
    std::size_t visLen = 0;
    for (unsigned char c : s) {
        if ((c & 0xC0) != 0x80) ++visLen;
    }
    std::string result = s;
    while (visLen < width) { result += ' '; ++visLen; }
    return result;
}

} // namespace

//! ---------- BEJEGYZÉS OSZTÁLY ----------

HighScoreEntry::HighScoreEntry(const std::string& name, const std::string& date, int prize)
    : name(name), date(date), prize(prize) {}

const std::string& HighScoreEntry::getName() const  { return name;  }
const std::string& HighScoreEntry::getDate() const  { return date;  }
int                HighScoreEntry::getPrize() const  { return prize; }

bool HighScoreEntry::operator>(const HighScoreEntry& rhs) const {
    return prize > rhs.prize;
}

//! ---------- DICSŐSÉGLISTA TÁBLA ----------

HighScoreTable::HighScoreTable(const std::string& filename)
    : filename(filename)
{
    load();
}

void HighScoreTable::load() {
    entries.clear();

    std::ifstream in(filename);
    if (!in.is_open()) return;

    std::string line;
    std::getline(in, line); // fejléc kihagyása

    while (std::getline(in, line)) {
        std::stringstream ss(line);
        std::string name, prizeStr, date;

        if (!std::getline(ss, name,     ';')) continue;
        if (!std::getline(ss, prizeStr, ';')) continue;
        if (!std::getline(ss, date,     ';')) continue;

        int prize = 0;
        try { prize = std::stoi(prizeStr); } catch (...) { continue; }

        entries.emplace_back(name, date, prize);
    }
}

void HighScoreTable::save() const {
    std::ofstream out(filename);
    if (!out.is_open()) return;

    out << "Nev;Nyeremeny;Datum\n";
    for (std::size_t i = 0; i < entries.size(); ++i) {
        out << entries[i].getName()  << ';'
            << entries[i].getPrize() << ';'
            << entries[i].getDate()  << "\n";
    }
}

void HighScoreTable::add(const std::string& name, int prize) {
    std::time_t now = std::time(nullptr);
    std::tm* tmNow  = std::localtime(&now);

    char dateBuffer[16] = {0};
    if (tmNow != nullptr)
        std::strftime(dateBuffer, sizeof(dateBuffer), "%Y/%m/%d", tmNow);

    entries.emplace_back(name, dateBuffer, prize);
    save();
}

void HighScoreTable::reset() {
    entries.clear();
    save();
}

void HighScoreTable::display() const {
    std::vector<HighScoreEntry> sorted = entries;
    std::sort(sorted.begin(), sorted.end(),
              [](const HighScoreEntry& a, const HighScoreEntry& b) {
                  return a.getPrize() > b.getPrize();
              });

    static const std::string SEP =
        "────────────────────────────────────────────────────────────────";

    std::cout << Color::BOLD_YELLOW << "\t             ─── DICSŐSÉGLISTA ───\n" << Color::RESET;
    std::cout << SEP << "\n";
    std::cout << "  " << visiblePad("#",         5)
                      << visiblePad("Játékos",   22)
                      << visiblePad("Nyeremény", 16)
                      << "Dátum\n";
    std::cout << SEP << "\n";

    for (std::size_t i = 0; i < sorted.size(); ++i) {
        std::string name = sorted[i].getName();

        // UTF-8-tudatos csonkítás 20 látható karakterre
        std::size_t visLen = 0, byteEnd = 0;
        for (; byteEnd < name.size(); ++byteEnd) {
            if ((static_cast<unsigned char>(name[byteEnd]) & 0xC0) != 0x80) {
                if (visLen == 20) break;
                ++visLen;
            }
        }
        name = name.substr(0, byteEnd);

        std::string prizeText = formatPrize(sorted[i].getPrize()) + " Ft";

        std::cout << "  " << visiblePad(std::to_string(i + 1) + ".", 5)
                          << visiblePad(name, 22)
                          << Color::BOLD_GREEN << visiblePad(prizeText, 16) << Color::RESET
                          << sorted[i].getDate() << "\n";
    }

    std::cout << SEP << "\n";
}
