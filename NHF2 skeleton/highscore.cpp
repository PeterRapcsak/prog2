/*======================================================================
    highscore.cpp - Dicsőséglista kezelése
----------------------------------------------------------------------
    FELADAT:
     - CSV-ből betöltés (dicsoseglista.csv)
     - Új rekord hozzáadása (név, nyeremény) + számított dátum
     - CSV-be mentés (append)
     - Rendezés csökkenő nyeremény szerint (< operator)
======================================================================*/

#include <ctime>
#include <iostream>
#include "highscore.h"

using std::string;

//* csak név és nyeremény kell, a dátumot majd itt kiszámoljulk
HighScoreEntry::HighScoreEntry(const string& name, int prize) : name(name), prize(prize) {}

const string& HighScoreEntry::getName()  const {return name;}
const string& HighScoreEntry::getDate()  const {return date;}
int           HighScoreEntry::getPrize() const {return prize;}

//* kell majd a rendezéshez
bool HighScoreEntry::operator>(const HighScoreEntry& rhs) const {
    return prize > rhs.prize;
}

//* Konstruktor filename paraméterrel
HighScoreTable::HighScoreTable(const string& filename) : filename(filename) {}

void HighScoreTable::load()  {} // TODO
void HighScoreTable::save() const {} // TODO
void HighScoreTable::add(const string& /*name*/, int /*prize*/) {} // TODO

void HighScoreTable::display() const {
    std::cout << "HighScoreTable::display()"<< std::endl;
}