/**
 *  \file ellipszis.h
 *  Ellipszis osztály deklarációja és definiciója.
 */
#ifndef ELLIPSZIS_H
#define ELLIPSZIS_H

#include "kor.h"

/// Elődeklaráció: az Ellipszis osztály
class Ellipszis;

/// Globális << operátor a Ellipszis adatainak kiírásához
/// os - ostream
/// el - ellipszis
std::ostream& operator<<(std::ostream& os, const Ellipszis& el);

/// Ellipszis osztály.
class Ellipszis : public Kor {
    double e;       /// excentricitás értéke  [ e = c/a, e = sqrt(1-b^2/a^2) ]
public:
    /// konstruktor 2 pontból
    /// p0 - kezdőpont
    /// r  - sugár
    /// e  - excentricitás
    /// sz - szín és átlátszóság
    Ellipszis(const Pont& p0, int r, double e, Szin sz)
    : Kor(p0, r, sz), e(e) {}

    /// Ellipszis excentricitásának lekérdezése
    double gete() const { return e; }

    /// Eltipszist rajzol
    void rajzol() const	{ std::cout << "Rajzol: " << *this << std::endl; }
};

/// Globális << operátor a Ellipszis adatainak kiírásához
/// os - ostream
/// el - ellipszis
inline std::ostream& operator<<(std::ostream& os, const Ellipszis& el) {
    return os << "Ellipszis " << (Alakzat&)el << ",r=" << el.getr() << ",e=" << el.gete();
}

#endif // ELLIPSZIS_H

