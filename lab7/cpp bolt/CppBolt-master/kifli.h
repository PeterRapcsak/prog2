#ifndef KIFLI_H
#define KIFLI_H

/**
 * \file kifli.h
 *
 * Kifli osztály deklarációja és inline függvényei
 */

#include "aru.h"

/// Kifli osztály.
class Kifli :public Aru {
    String spec;        ///< kifli fajtája
  public:
    /// Konstruktor beállítja az attribútumokat (ősosztályét is)
    /// ar - kifli egységára
    /// spec - kifli fajtája
    Kifli(double ar, const char *spec = "")
        :Aru("Kifli", "db", ar), spec(spec) {}

    /// kiírás egy stream-re
    /// os - output stream referencia
    std::ostream& print(std::ostream& os) const {
        return os << megnevezes << " "
                  << spec << "; "
                  << ar << "Ft/" << egyseg;
    }
};

#endif // KIFLI_H
