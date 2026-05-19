/**
 * \file irodistak.h
 * Minden származtatott osztály deklarációját ebbe a fájlba tettük.
 * Az egyszerűség kedvéért minden tagfüggvényt inline valósítottunk meg.
 *
 */

#ifndef IRODISTAK_H
#define IRODISTAK_H

#include <iostream>
#include <string>

#include "alkalmazott.h"


// Csoport azonosító típusa
typedef int csop_t;


/**
 * Csoportvezető
 */
class CsopVez : virtual public Alkalmazott {
    csop_t csoport; // csoport azonosító

public:

    // Konstruktor normál adatokból
    CsopVez(const std::string& n, double f, csop_t cs)
        : Alkalmazott(n, f), // Alkalmazott rész
          csoport(cs)        // csoport 
    {}

    // Csoport lekérdezése
    csop_t getCs() const {
        return csoport;
    }

    // Csoport beállítása
    void setCs(csop_t cs) {
        csoport = cs;
    }

    // Kiírás streambe
    void kiir(std::ostream& os = std::cout) const {
        os << "CsopVez: ";
        Alkalmazott::kiir(os);
    }
};


/**
 * Határozott idejű alkalmazott
 */
class HatIdeju : virtual public Alkalmazott {
protected:
    time_t ido; // szerződés lejárati ideje

public:

    // Konstruktor normál adatokból
    HatIdeju(const std::string& n, double f, time_t t)
        : Alkalmazott(n, f), // Alkalmazott rész init
          ido(t)             // idő inicializálása
    {}

    // Lejárati idő lekérdezése
    time_t getIdo() const {
        return ido;
    }

    // Lejárati idő beállítása
    void setIdo(time_t t) {
        ido = t;
    }

    // Kiírás streambe
    void kiir(std::ostream& os = std::cout) const {
        os << "HatIdeju: ";
        Alkalmazott::kiir(os);
    }
};


/**
 * Határozott idejű csoportvezető
 */
class HatIdCsV : public CsopVez, public HatIdeju {
public:

    // Konstruktor normál adatokból
    HatIdCsV(const std::string& n, double f, csop_t cs, time_t t)
        : Alkalmazott(n, f), // virtuális Alkalmazott alaposztály

          // Itt direkt f * 2 van, hogy látszódjon:
          // a CsopVez is kapna saját fizetést, ha nem lenne virtual öröklés
          CsopVez(n, f * 2, cs),

          // Itt direkt f * 10 van, hogy látszódjon:
          // a HatIdeju is kapna saját fizetést, ha nem lenne virtual öröklés
          HatIdeju(n, f * 10, t)
    {}

    // Kiírás streambe
    void kiir(std::ostream& os = std::cout) const {
        os << "HatIdCsV:" << std::endl;

        os << "   ";
        CsopVez::kiir(os);

        os << "   ";
        HatIdeju::kiir(os);
    }
};


/**
 * Határozott idejű csoportvezető helyettes
 */
class HatIdCsVezH : public HatIdCsV {
public:

    // Konstruktor normál adatokból
    HatIdCsVezH(const std::string& n, double f, time_t t, CsopVez& kit)
        : Alkalmazott(n, f), // virtuális Alkalmazott alaposztály init

          // A csoportot attól a csoportvezetőtől vesszük át, akit helyettesít
          HatIdCsV(n, f, kit.getCs(), t)
    {}

    // Kiírás streambe
    void kiir(std::ostream& os = std::cout) const {
        os << "HatIdCsVezH: ";
        HatIdCsV::kiir(os);
    }
};


#endif // IRODISTAK_H