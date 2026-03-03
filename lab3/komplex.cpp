/**
 * \file komplex.cpp
 *
 * Komplex osztályt megvalósító függvények definíciója.
 *
 * Folytassa a megvalósítást sorrendben a komplex.h fájlban levő feladatoknak megfelelően!
 *
 */


#include <iostream>         // Valószínű, hogy a kiíráshoz majd kell
#include <iomanip>          // ... és ez is.
#include <cmath>            // az sqrt miatt kell.

#include "komplex.h"        // Ebben van a Komplex osztály, és néhány globális függvény deklarációja

///using namespace std;  // ha nagyon kell, akkor csak itt nyissuk ki a névteret, a headerben soha!

/// Előre elkészített tagfüggvények

/// 0, 1 és 2 paraméteres konstruktor
/// Figyelje meg, hogy a default argumentumokat CSAK a deklarációnál
/// adtuk meg!
Komplex::Komplex(double r, double im) {
    re = r; // ha nincs névütközés, nem kell kiírni a this-t
    this->im = im;
}

///---- Egy példa a konstans tagfüggvényre, ehhez hasonlóan kell
///     elkészíteni a gettereket az 1. feladatban (ELKESZULT=1)
/// Abszolút érték lekérdezése
/// @return - abszolút érték
double Komplex::abs() const { return sqrt(re*re + im*im); }

#if ELKESZULT >= 3
// 3. feladathoz (ELKESZULT 3)
// összehasonlítás
/// Egyenlőtlenség vizsgálat
/// @param rhs_k - jobb oldali operandus (Komplex)
/// @return hamis - ha a képzetes és a valós rész is azonos, egyébként false
bool Komplex::operator!=(const Komplex& rhs_k) const {  // visszavezetjük az egyenlőség vizsgálatra
    return !(*this == rhs_k);
}
#endif

#if ELKESZULT >= 6
// 6. feladathoz (ELKESZULT 6)
// a += művelet viszont módosítja önmagát!
/// Komplex + double
/// @param rhs_d - jobb oldali operandus (double)
/// @return eredeti (bal oldali) objektum ref., amihez hozzáadtuk rhd_d-t
Komplex& Komplex::operator+=(double rhs_d) {
    re += rhs_d;
    return *this;
}
#endif

// ------------------------ Eddig elkészítettük -------------------------
// TODO: A hiányzó tag- és globális függvények itt következnek

// 3. Feladat

double Komplex::getRe() const {
    return re;
}

double Komplex::getIm() const {
    return im;
}

// 5. Feladat

void Komplex::setRe(double r) {
    re = r;
}

void Komplex::setIm(double i) {
    im = i;
}


// egyenlőség vizsgálás
bool Komplex::operator==(const Komplex& rhs_k) const {
    return re == rhs_k.re && im == rhs_k.im;
}

// Komplex + Komplex
Komplex Komplex::operator+(const Komplex& rhs_k) const {
    return Komplex(re + rhs_k.re, im + rhs_k.im);
}

// Komplex + re
Komplex Komplex::operator+(double rhs_d) const {
    return Komplex(re + rhs_d, im);
}

Komplex operator+(double lhs_d, const Komplex& rhs_k) {
    return Komplex(lhs_d + rhs_k.getRe(), rhs_k.getIm());
}

Komplex& Komplex::operator+=(const Komplex& rhs_k) {
    re += rhs_k.re;
    im += rhs_k.im;
    return *this;
}
