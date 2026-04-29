/**
 * \file alkalmazott.h
 * Alkalmazott alaposztály deklarációja
 * Az egyszerűség kedvéért minden tagfüggvényt inline valósítottunk meg.
 *
 */

#ifndef ALKALMAZOTT_H
#define ALKALMAZOTT_H

#include <iostream>
#include <string>
#include "memtrace.h"


/// header fájlban ne használjon using direktívát, mert a hatása
/// rejtve marad

/**
 * Alkalmazott
 */
class Alkalmazott {
    std::string   nev;          // nev
    double   fizetes;           // fizetés
public:
    Alkalmazott() :fizetes(0) {
        std::cout << "Baj van: Az Alkalmazott default konstruktora hivodott!" << std::endl
                  << "         Javitsa a kodot!" << std::endl;
    }

    Alkalmazott(const  std::string& n, double fiz) :nev(n), fizetes(fiz) {
#if TESTLEVEL <= 1
        std::cout << 'A';
#endif
    }

    /// Név lekérdezése
    /// @return - nev
    const std::string& getNev() const { return nev; }

    /// Fizetés lekérdezése
    /// @return - fizetés
    double getFiz() const { return fizetes; }

    /// Név beállítása
    /// @param nev - név
    void setNev(const std::string& n) { nev = n; }


    /// Fizetés beállítása
    /// @param fiz - fizetés
    void setFiz(double fiz) { fizetes = fiz; }

    /// Adatok kiírása
    /// @param os - output adatfolyam
    virtual void kiir(std::ostream& os = std::cout) const {
        os << nev << " fizetese: " << fizetes << std::endl;
    }

    virtual ~Alkalmazott() {
#if TESTLEVEL <= 1
        std::cout << "Megszunik: ";
        kiir(std::cout);
#endif
    }
};
#endif // ALKALMAZOTT_H

