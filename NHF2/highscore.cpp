/*======================================================================
    highscore.cpp - Dicsőséglista implementáció
----------------------------------------------------------------------
    CÉL:
     - HighScoreEntry és HighScoreTable metódusok
     - Betöltés / mentés / rendezett kiírás
======================================================================*/

#include "highscore.h"
#include <algorithm> // std::sort
#include <fstream>   // ifstream, ofstream
#include <iostream>  // std::cout
#include <sstream>   // stringstream
#include <ctime>     // time, localtime, strftime

//! ---------- BEJEGYZÉS OSZTÁLY ----------

/*
    CÉL: Dicsőséglista bejegyzés létrehozása
    BE:
     - name  : játékos neve
    - date: dátum szöveg
    - prize: elért nyeremény összege
*/
HighScoreEntry::HighScoreEntry(const std::string& name, const std::string& date, int prize)
    : name(name), date(date), prize(prize) {}

// CÉL: Játékos nevének lekérése
const std::string& HighScoreEntry::getName() const {
    return name;
}

// CÉL: Dátum szöveg lekérése
const std::string& HighScoreEntry::getDate() const {
    return date;
}

// CÉL: Nyeremény összegének lekérése
int HighScoreEntry::getPrize() const {
    return prize;
}

/*
    CÉL: Összehasonlítás rendezéshez
    KI: true ha ez a bejegyzés nagyobb nyereményű mint rhs
*/
bool HighScoreEntry::operator>(const HighScoreEntry& rhs) const {
    return prize > rhs.prize;
}

//! ---------- DICSŐSÉGLISTA TÁBLA ----------

/*
    CÉL: Dicsőséglista tábla létrehozása
    BE: filename - CSV file neve
*/
HighScoreTable::HighScoreTable(const std::string& filename)
    : filename(filename)
{
    load(); // konstruktorban rögtön betöltjük
}

/*
    CÉL: Bejegyzések beolvasása CSV fileból
    FORMÁTUM: Nev;Nyeremeny;Datum
*/
void HighScoreTable::load() {
    entries.clear();

    std::ifstream in(filename);
    if (!in.is_open()) {
        return; // nincs file -> üres lista
    }

    std::string line;
    std::getline(in, line); // első sor (fejléc) kihagyása

    while (std::getline(in, line)) {
        std::stringstream ss(line);
        std::string name;
        std::string prizeStr;
        std::string date;

        // Pontosvessző alapján darabolás
        if (!std::getline(ss, name, ';'))     continue;
        if (!std::getline(ss, prizeStr, ';')) continue;
        if (!std::getline(ss, date, ';'))     continue;

        int prize = 0;
        try {
            prize = std::stoi(prizeStr);
        } catch (...) {
            continue; // hibás szám -> sor kihagyása
        }

        entries.emplace_back(name, date, prize);
    }
}

/*
    CÉL: Bejegyzések teljes visszaírása CSV fileba
*/
void HighScoreTable::save() const {
    std::ofstream out(filename);
    if (!out.is_open()) {
        return; // nem írható file
    }

    out << "Nev;Nyeremeny;Datum\n"; // fejléc

    for (std::size_t i = 0; i < entries.size(); ++i) {
        out << entries[i].getName()  << ';'
            << entries[i].getPrize() << ';'
            << entries[i].getDate()  << "\n";
    }
}

/*
    CÉL: Új eredmény hozzáadása és mentése
    BE:
     - name  : játékos neve
     - prize : elért nyeremény
*/
void HighScoreTable::add(const std::string& name, int prize) {
    std::time_t now   = std::time(nullptr);
    std::tm* tmNow    = std::localtime(&now);

    char dateBuffer[16] = {0};
    if (tmNow != nullptr) {
        std::strftime(dateBuffer, sizeof(dateBuffer), "%Y/%m/%d", tmNow);
    }

    entries.emplace_back(name, dateBuffer, prize);
    save(); // azonnal mentjük fileba
}

/*
    CÉL: Dicsőséglista kiírása csökkenő nyeremény szerint
*/
void HighScoreTable::display() const {
    // Másolatot rendezünk, az eredeti vektor sorrendje marad
    std::vector<HighScoreEntry> sorted = entries;
    std::sort(sorted.begin(), sorted.end(),
              [](const HighScoreEntry& a, const HighScoreEntry& b) {
                  return a.getPrize() > b.getPrize(); // csökkenő sorrend
              });

    std::cout << "\n\t- DICSŐSÉGLISTA -\n";
    std::cout << "----------------------------------------------------------------\n";
    std::cout << " #   Játékos               Nyeremény       Dátum\n";
    std::cout << "----------------------------------------------------------------\n";

    for (std::size_t i = 0; i < sorted.size(); ++i) {
        std::cout << " " << (i + 1) << ". "
                  << sorted[i].getName()  << " - "
                  << sorted[i].getPrize() << " Ft ("
                  << sorted[i].getDate()  << ")\n";
    }

    std::cout << "----------------------------------------------------------------\n";
}
