/**
 *  \file szakasz.h
 *  Szakasz osztály deklarációja
 */
#ifndef SZAKASZ_H
#define SZAKASZ_H

#include "alakzat.h"

/// Szakasz osztály.
/// Az alaposztály adatait egy újabb ponttal bővítjük
class Szakasz : public Alakzat {
    Pont pv;    /// szakasz végpontja az origójához (p0) relatívan
public:
    /// konstruktor 2 pontból
    /// @param p0 - kezdőpont
    /// @param pv - végpont
    /// @param sz - szín és átlátszóság
    Szakasz(const Pont& p0, const Pont& pv, Szin sz)
                     : Alakzat(p0, sz), pv(pv-p0) { }

    /// konstruktor 1 pontból és 2 távolságból
    /// @param p0 - kezdõpont
    /// @param dx - x irányú távolság
    /// @param dy - y irányú távolság
    /// @param sz - szín és átlátszóság
    Szakasz(const Pont& p0, int dx, int dy, Szin sz)
                     : Alakzat(p0, sz), pv(getp0()+Pont(dx, dy)) { }

    /// Szakasz végpontjának lekérdezése
    /// @return szakasz végpontja
    Pont getpv() const { return getp0()+pv; }

    /// szakaszt rajzol
    void rajzol() const;	    // átdefiniáljuk a virt. fv-t.
};

/// Globális << operátor a Szakasz adatainak kiírásához
/// @param os - ostream
/// @param sz - szakasz
/// @return os - ostream
std::ostream& operator<<(std::ostream& os, const Szakasz& sz);

#endif // SZAKASZ_H
