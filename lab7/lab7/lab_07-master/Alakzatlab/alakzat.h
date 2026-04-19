/**
 *  \file alakzat.h
 *  Alakzat alaposztály deklarációja
 */
#ifndef ALAKZAT_H
#define ALAKZAT_H

#include <iostream>         // ostream miatt kell
#include "memtrace.h"
#include "pont.h"
#include "szin.h"

/// Alakzat abszrakt osztály
class Alakzat {
     Pont p0;                   /// Alakzat origója, vagy bázis pontja.
     /// Minden további adatot ehhez relatívan tárolunk. Geometriai értelemben nem feltétlenül origó.
     Szin sz;                   /// alakzat színe
public:
    /// Konstruktor
    /// @param p0 - kezdőpont
    /// @param sz - szín és átlátszóság
    Alakzat(const Pont& p0, const Szin& sz) :p0(p0), sz(sz) {}

    /// Pozíció lekérdezése
    /// @return alakzat origója
    const Pont& getp0() const { return p0; }

    /// Szin lekérdezése
    /// @return alakzat színe
    const Szin& getsz() const { return sz; }

    /// mozgat: origóját eltolja
    /// @param d - eltolás vektora
    void mozgat(const Pont& d);

    /// rajzol
    virtual void rajzol() const = 0;

    /// Destruktor virtuális, hogy lehessen dinamikus adattagja
    /// a származtatottnak
    virtual ~Alakzat() {}
};

/// Globális << operátor az Alakzat adatainak kiírásához
/// @param os - ostream
/// @param a - alakzat
/// @return os - ostream
std::ostream& operator<<(std::ostream& os, const Alakzat& a);

#endif // ALAKZAT_H
