/*======================================================================
    highscore.h - Dicsőséglista osztályok deklarációja
----------------------------------------------------------------------
    FELADAT:
     - HighScoreEntry: egyetlen bejegyzés (név, dátum, nyeremény)
     - HighScoreTable: lista kezelése – betöltés, mentés, megjelenítés
======================================================================*/

#ifndef HIGHSCORE_H
#define HIGHSCORE_H

#include <string>
#include <vector>

//! ---------- BEJEGYZÉS ----------

// CÉL: Egyetlen dicsőséglista rekord tárolása
class HighScoreEntry {
private:
    std::string name;  // játékos neve (max. 20 látható karakter)
    std::string date;  // eredmény dátuma (pl. "2026/05/09")
    int prize;         // elért nyeremény Ft-ban

public:
    /*
        CÉL: Bejegyzés létrehozása
        BE: name  - játékos neve
            date  - dátum string
            prize - nyeremény összege
    */
    HighScoreEntry(const std::string& name, const std::string& date, int prize);

    const std::string& getName()  const; // játékos neve lekérése
    const std::string& getDate()  const; // dátum lekérése
    int                getPrize() const; // nyeremény lekérése

    // CÉL: Csökkenő rendezéshez szükséges összehasonlítás (std::sort-hoz)
    bool operator>(const HighScoreEntry& rhs) const;
};

//! ---------- LISTA ----------

// CÉL: Dicsőséglista betöltése, mentése, megjelenítése és törlése
class HighScoreTable {
private:
    std::vector<HighScoreEntry> entries; // betöltött bejegyzések
    std::string filename;                // CSV fájl neve

public:
    /*
        CÉL: Tábla létrehozása – konstruktor azonnal betölti a CSV-t
        BE: filename - CSV fájl neve (alapértelmezett: "dicsoseglista.csv")
    */
    explicit HighScoreTable(const std::string& filename = "dicsoseglista.csv");

    // CÉL: Bejegyzések beolvasása CSV fájlból (hibás sorokat kihagyja)
    void load();

    // CÉL: Az összes bejegyzés visszaírása CSV fájlba (teljes felülírás)
    void save() const;

    /*
        CÉL: Új eredmény hozzáadása az aktuális dátummal és azonnali mentése
        BE: name  - játékos neve
            prize - elért nyeremény Ft-ban
    */
    void add(const std::string& name, int prize);

    // CÉL: Lista rendezett kiírása a konzolra (csökkenő nyeremény szerint)
    void display() const;

    // CÉL: Lista törlése memóriából és fájlból (csak a fejléc marad a CSV-ben)
    void reset();
};

#endif
