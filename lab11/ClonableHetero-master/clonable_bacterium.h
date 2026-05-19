/**
 * \file: clonable_bacterium.h
 *
 */

#ifndef CLONABLE_BACTERIUM_H
#define CLONABLE_BACTERIUM_H

#include "bacterium.h"
#include "clonable.h"

/**
 * A ClonableBacterium, ClonableSalmonella, ClonableStreptococcus
 * osztályoknak kompatibilisnek kell lennie a Bacterium és Clonable osztályokkal.
 * A funkciókat tekintve meg kell egyezni a Bacterium, Salmonella, Streptococcus
 * osztályok funkcióival.
 */


//! ---------- KLÓNOZHATÓ BACTERIUM ----------
struct ClonableBacterium : virtual Bacterium, Clonable {

    ClonableBacterium(const char* n = "")
        : Bacterium(n)
    {}

    ClonableBacterium* clone() const {
        return new ClonableBacterium(*this);
    }
};


//! ---------- KLÓNOZHATÓ SALMONELLA ----------
struct ClonableSalmonella : Salmonella, ClonableBacterium {

    ClonableSalmonella(const char* sp = "")
        : Bacterium("Salmonella"),
          Salmonella(sp)
    {}

    ClonableSalmonella* clone() const {
        return new ClonableSalmonella(*this);
    }
};


//! ---------- KLÓNOZHATÓ STREPTOCOCCUS ----------
struct ClonableStreptococcus : Streptococcus, ClonableBacterium {

    ClonableStreptococcus(char grp = 'A')
        : Bacterium("Streptococcus"),
          Streptococcus(grp)
    {}

    ClonableStreptococcus* clone() const {
        return new ClonableStreptococcus(*this);
    }
};


#endif // CLONABLE_BACTERIUM_H
