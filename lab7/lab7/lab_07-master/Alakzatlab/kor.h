/**
 *  \file kor.h
 *  Kor osztály deklarációja
 */
#ifndef KOR_H
#define KOR_H

#include "alakzat.h"

/// Kor osztály.
/// Az alaposztály adatait sugárral bővítjük
class Kor : public Alakzat {
    int r;  /// sugár
public:
    /// Konstruktor
    /// @param p0 - kezdőpont
    /// @param r - sugár
    /// @param sz - szín és átlátszóság
    Kor(const Pont& p0, int r, Szin sz)
                    :Alakzat(p0, sz), r(r)  /// ős osztály inic.
                    {}

    /// Sugár lekérdezése
    /// @return kör sugara
    int getr() const { return r; }

    /// kört rajzol
    void rajzol() const;
};

/// Globális << operátor a Kor adatainak kiírásához
/// @param os - ostream
/// @param k - kor
/// @return os - ostream
std::ostream& operator<<(std::ostream& os, const Kor& k);

#endif // KOR_H
