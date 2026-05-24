
#ifndef UTILS_H
#define UTILS_H

#include <iostream> 
#include <limits>
#include <random>
#include "memtrace.h"

#include "colors.h"

using std::string;
using std::cout;
using std::endl;
using std::cin;

//! ---------- KONSTANSOK ----------

static const int LEVELS = 12;          // szintek száma (kérdések száma egy játékban)

extern const int PRIZE_LADDER[LEVELS];

//! ---------- SEGÉDFÜGGVÉNYEK ----------


// CÉL: Játékmód választás olvasása (1-4), érvénytelen bemenetnél hibaüzenet + újrakérdezés
int selectGameMode();


// CÉL: Egész szám olvasása, nem szám bemenet esetén -1-et ad vissza
int readInt();
/*
    CÉL: Megmondja, hogy az adott szintindex biztos szint-e
    BE: index
    KI: igaz ha 4 (5. kérdés) vagy 9 (10. kérdés)
*/
bool isSafeLevel(int index);

// Ha a játékos veszít, akkor a legutóbbi safe level nyereményét kapja meg
int getSafePrize(int index);

void printSeparator();

// Addig vár, amíg a játékos Entert nem nyom
void waitEnter();

void clearScreen();

/*
    CÉL: Egész szám ezres tagolással formázva
    BE: prize - összeg Ft-ban (pld: 5000000)
    KI: Formázott string (pld: "5.000.000")
*/
string formatPrize(int prize);

// CÉL: Egy központi random generátor
std::mt19937& rng();

// CÉL: Uj játéknál új random seed generálása
void reseedRng();

// CÉL: Random int generálása két érték között
int randomInt(int min, int max);

// CÉL: Szint fejlécének kiírása
void printLevelHeader(int level);

// CÉL: Minden input-ot nagybetűsítünk
void normalizeInput(string& input);


// Visszaadja hogy a játékos nem elrejtett választ választott
bool isHiddenBy5050(int selectedIndex, int hidden0, int hidden1);



// CÉL: Sorrendezős válasz validitás ellenőrzése
bool isValidOrderInput(const string& input);




#endif