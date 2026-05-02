/*======================================================================
    highscore.h - Header file
----------------------------------------------------------------------
    CÉL:
     - Dicsőséglista osztályok deklarációja
======================================================================*/

#ifndef HIGHSCORE_H
#define HIGHSCORE_H

#include <string>
#include <vector>

//! ---------- DICSŐSÉGLISTA ----------

// CÉL: Egy dicsőséglista bejegyzés tárolása
class HighScoreEntry {
private:
    std::string name;   // Játékos neve
    std::string date;   // Eredmény dátuma (pl. "2026-04-29")
    int prize;          // Nyeremény összege

public:
    // CÉL: Név, dátum és nyeremény eltárolása
    HighScoreEntry(const std::string& name, int prize);

    // CÉL: Játékos nevének lekérése
    const std::string& getName() const;

    // CÉL: Dátum szöveg lekérése
    // *Ha c++11 felett vagyunk akkor "std::chrono::system_clock::now()" lenne a legkönnyebb
    const std::string& getDate() const;

    // CÉL: Nyeremény lekérése
    int getPrize() const;

    // CÉL: Nyeremény szerinti összehasonlítás rendezéshez
    bool operator>(const HighScoreEntry& rhs) const;
};

// CÉL: Dicsőséglista betöltés / mentés / kiírás
class HighScoreTable {
private:
    std::vector<HighScoreEntry> entries;
    std::string filename;

public:
    // CÉL: Dicsőséglista objektum létrehozása CSV fájlhoz
    explicit HighScoreTable(const std::string& filename = "dicsoseglista.csv");

    // CÉL: Bejegyzések beolvasása fájlból
    void load();

    // CÉL: Bejegyzések teljes visszaírása fájlba
    void save() const;

    // CÉL: Új eredmény hozzáadása és mentése
    void add(const std::string& name, int prize);

    // CÉL: Dicsőséglista RENDEZETT kiírása
    void display() const;
};

#endif