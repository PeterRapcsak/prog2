/*======================================================================
    filemanager.cpp - CSV fájlok betöltése
----------------------------------------------------------------------
    FELADAT:
     - kerdesek.csv/sorkerdesek.csv olvasása
     - File létezésének ellenőrzése
     - ";" csekkolás és hibás sorok kezelése
======================================================================*/

#include <fstream>
#include "filemanager.h"

using std::vector;
using std::string;

// ChooseQuestion-ökből álló vektor a CSV-ből
vector<ChooseQuestion> FileManager::loadChooseQuestions(const string& /*filename*/) {
    return {}; // TODO
}

// OrderQuestion-ökből álló vektor a CSV-ből
vector<OrderQuestion> FileManager::loadOrderQuestions(const string& /*filename*/) {
    return {}; // TODO
}

// A file létezésének ellenőrzése
bool FileManager::fileExists(const string& path) {
    std::ifstream f(path);
    return f.good(); // .good() = csekkolja hogy a filestream helyes/használható-e
}
