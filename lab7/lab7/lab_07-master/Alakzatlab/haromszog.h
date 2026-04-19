/**
 *  \file haromszog.h
 *  Haromszog osztály deklarációja
 */
#ifndef HAROMSZOG_H
#define HAROMSZOG_H

#include "alakzat.h"

/// Haromszog osztály.
class Haromszog : public Alakzat {
    Pont pb;    /// 1. csúcspont
    Pont pc;    /// 2. csúcspont
public:
    /// konstruktor 3 pontból
    /// pa - 1. csúcspont (ez lesz az origó)
    /// pb - 2. csúcspont
    /// pc - 3. csúcspont
    /// sz - szín és átlátszóság
    Haromszog(const Pont& pa, const Pont& pb, const Pont& pc, Szin sz)
    : Alakzat(pa, sz), pb(pb-pa), pc(pc-pa) { }

    /// Második csúcspont lekérdezése
    Pont getpb() const { return getp0()+pb; }

    /// Harmadik csúcspont lekérdezése
    Pont getpc() const { return getp0()+pc; }

    /// háromszöget rajzol
    void rajzol() const;
};

/// Globális << operátor a Haromszog adatainak kiírásához
///  os - ostream
///  h - haromszog
std::ostream& operator<<(std::ostream& os, const Haromszog& h);

#endif // HAROMSZOG_H

