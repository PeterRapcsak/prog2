#ifndef UTILS_H
#define UTILS_H

#include <cstddef>
#include <iostream> 
#include <limits>
#include <random>
#include <string>
#include "memtrace.h"

#include "colors.h"

using std::string;
using std::cout;
using std::endl;
using std::cin;

//! ---------- KONSTANSOK ----------

static const int LEVELS = 12; // szintek száma (kérdések száma egy játékban)

extern const int PRIZE_LADDER[LEVELS];

//! ---------- SEGÉDFÜGGVÉNYEK ----------

// CÉL: Egész szám olvasása, nem szám bemenet esetén -1-et ad vissza
int readInt();

// CÉL: Játékmód választás olvasása (1-4), érvénytelen bemenetnél hibaüzenet + újrakérdezés
int selectGameMode();

// Addig vár, amíg a játékos Entert nem nyom
void waitEnter();

void clearScreen();

void printSeparator();

//! ---------- JÁTÉKLOGIKA SEGÉDFÜGGVÉNYEI ----------

/*
    CÉL: Megmondja, hogy az adott szintindex biztos szint-e
    BE: index
    KI: igaz ha 4 (5. kérdés) vagy 9 (10. kérdés)
*/
bool isSafeLevel(int index);

// Ha a játékos veszít, akkor a legutóbbi safe level nyereményét kapja meg
int getSafePrize(int index);

// CÉL: Szint fejlécének kiírása
void printLevelHeader(int level);

//! ---------- SZÖVEGKEZELÉS ----------

// CÉL: Minden input-ot nagybetűsítünk
void normalizeInput(string& input);

// CÉL: UTF-8 string látható hosszának kiszámolása
std::size_t visibleLength(const string& text);

// Visszaadja hogy a játékos nem elrejtett választ választott
bool isHiddenBy5050(int selectedIndex, int hidden0, int hidden1);

// CÉL: Sorrendezős válasz validitás ellenőrzése
bool isValidOrderInput(const string& input);

//! ---------- FORMÁZÁS ----------

/*
    CÉL: Egész szám ezres tagolással formázva
    BE: prize - összeg Ft-ban (pld: 5000000)
    KI: Formázott string (pld: "5.000.000")
*/
string formatPrize(int prize);

//! ---------- RANDOM ----------

// CÉL: Egy központi random generátor
std::mt19937& rng();

// CÉL: Uj játéknál új random seed generálása
void reseedRng();

// CÉL: Random int generálása két érték között
int randomInt(int min, int max);

#endif
