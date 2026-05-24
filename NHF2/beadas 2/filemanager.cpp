/*======================================================================
    filemanager.cpp - CSV fájlok betöltése
----------------------------------------------------------------------
    FELADAT:
     - kerdesek.csv/sorkerdesek.csv olvasása
     - File létezésének ellenőrzése
     - ";" csekkolás és hibás sorok kezelése
======================================================================*/

#include "memtrace.h" // memóriaszivárgás check

#include "filemanager.h"
#include <fstream>	// file olvasás
#include <sstream>	// string darabolás (könnyen)

// Egyesével importáljuk, hogy ne kelljen mindenhol std::-t írni
// ÉS nincs namespace clutter
using std::string;
using std::vector;
using std::ifstream;
using std::stringstream;

//! ---------- CSV BETÖLTÉS ----------

/*
    CÉL: Feleletválasztós kérdések betöltése CSV fileból
    CSV formátum: "Nehézség;Kérdés;A;B;C;D;Válasz;Kategória"
    BE: filename - a file neve
    KI: A betöltött ChooseQuestion objektumok vektora (üres ha hiba)
*/
vector<ChooseQuestion> FileManager::loadChooseQuestions(const string& filename) {
    vector<ChooseQuestion> result;
	
    ifstream inputFile(filename); // input file stream
    if (!inputFile.is_open()) {
        return result; // nem nyitható meg -> üres vektor
    }

    string line;
    getline(inputFile, line); // első sort kihagyjuk

    while (getline(inputFile, line)) {
        stringstream ss(line);
        string field;
        vector<string> fields;

        // Pontosvesszők alapján darabolás (honnan, hová, meddig)
        while (getline(ss, field, ';')) {
            fields.push_back(field); // egyesével a vektor végére
        }

        if (fields.size() != 8) {
            continue; // hibás sor (nem 8 adat) -> kihagyjuk
        }

        // Nehézségi szint szám konverzió
        int diff = 0;
        try {
            diff = stoi(fields[0]);
        } catch (...) { // MINDENT elkapunk 
            continue; // nem számmá alakítható -> kihagyjuk
        }

        // fields[2..5] = A, B, C, D válaszok
        vector<string> answers = { fields[2], fields[3], fields[4], fields[5] };

        // fields[1] - kérdés
        // fields[7] - kategória,
        // fields[6] - helyes válasz
        result.emplace_back(diff, fields[1], fields[7], fields[6], answers);
        // Nem hívja a copy konstruktort, a result-ban hozza létre az uj rekordot
        // igen tudom majdnem teljesen ugyanaz mint a push_back() de marginally gyrosabb
    }

    return result;
}


/*
    CÉL: Sorrendezős kérdések betöltése CSV fileból
    CSV formátum: "Sorkérdés;A;B;C;D;Válasz;Kategória"
    BE: filename - a file neve
    KI: A betöltött OrderQuestion objektumok vektora (üres ha hiba)
*/
vector<OrderQuestion> FileManager::loadOrderQuestions(const string& filename) {
    vector<OrderQuestion> result;
    ifstream inputFile(filename); // input file stream
    if (!inputFile.is_open()) {
        return result; // nem nyitható meg -> üres vektor
    }

    string line;
    getline(inputFile, line); // első sort kihagyjuk

    while (getline(inputFile, line)) {
        stringstream ss(line);
        string field;
        vector<string> fields;

        // Pontosvesszők alapján darabolás
        while (getline(ss, field, ';')) {
            fields.push_back(field); // egyesével a vektor végére
        }

        if (fields.size() != 7) {
            continue; // hibás sor (nem 7 adat) -> kihagyjuk
        }

        // fields[1..4] = A, B, C, D elemek (ezeket kell sorba rendezni)
        vector<string> answers = { fields[1], fields[2], fields[3], fields[4] };

        // fields[0] - kérdés
        // fields[6] - kategória
        // fields[5] - helyes sorrend
        result.emplace_back(fields[0], fields[6], fields[5], answers);
    }

    return result;
}


//CÉL: Megnézni hogy egy file létezik-e
bool FileManager::fileExists(const string& path) {
    ifstream inputFile(path);
    return inputFile.good(); // .good() = csekkolja hogy a filestream helyes/használható-e
}
