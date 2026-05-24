/*======================================================================
    highscore.h - Dicsőséglista osztályok deklarációja
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
======================================================================*/

#ifndef HIGHSCORE_H
#define HIGHSCORE_H

#include <string>
#include <vector>

//! ---------- BEJEGYZÉS ----------

// CÉL: Egyetlen dicsőséglista rekord tárolása
class HighScoreEntry {
private:
    std::string name;  // játékos neve, max 20 látható karakter
    std::string date;  // eredmény dátuma, pld: "2026/05/09"
    int prize;         // elért nyeremény Ft-ban

public:

    // CÉL: Egyetlen dicsőséglista bejegyzés létrehozása
    HighScoreEntry(
        const std::string& name,  // játékos neve
        const std::string& date,  // eredmény dátuma
        int prize);               // elért nyeremény

    // CÉL: Játékos nevének lekérése
    const std::string& getName() const;

    // CÉL: Dátum lekérése
    const std::string& getDate() const;

    // CÉL: Nyeremény lekérése
    int getPrize() const;

    // CÉL: Csökkenő nyeremény szerinti összehasonlítás
    bool operator>(const HighScoreEntry& rhs) const;
};


//! ---------- LISTA ----------

// CÉL: Dicsőséglista betöltése, mentése, kiírása és törlése
class HighScoreTable {
private:
    std::vector<HighScoreEntry> entries; // betöltött dicsőséglista bejegyzések
    std::string filename;                // CSV fájl neve

public:
    /*
        CÉL: Dicsőséglista tábla létrehozása
        MEGJEGYZÉS:
            A konstruktor azonnal be is tölti a CSV-t
    */
    explicit HighScoreTable(const std::string& filename = "dicsoseglista.csv"); // default value

    // CÉL: Bejegyzések beolvasása CSV fájlból
    void load();

    // CÉL: Az összes bejegyzés visszaírása CSV fájlba
    void save() const;

    /*
        CÉL: Új eredmény hozzáadása (aktuális dátumot itt szamolunk majd)
        BE: name  - játékos neve
            prize - elért nyeremény
    */
    void add(const std::string& name, int prize);

    // CÉL: A dicsőséglista elemeinek száma getter
    std::size_t HighScoreTable::size() const {}

    // CÉL: Dicsőséglista törlése memóriából és fájlból
    void reset();

    // CÉL: Dicsőséglista rendezett kiírása a konzolra
    void display() const;
};

#endif