/**
 * \file iroda.h
 * Iroda osztály deklarációja
 * Az egyszerűség kedvéért minden tagfüggvényt inline valósítottunk meg.
 *
 */
#ifndef IRODA_H
#define IRODA_H

#include <vector>
#include "memtrace.h"
#include "alkalmazott.h"

/**
 * Iroda osztály.
 * Standard tárolót használ
 */
class Iroda {
    /// saját típus, hogy könnyen le tudjuk cserélni a tárolót
    /// a tároló POINTERT tárol (heterogén gyűjtemény)
    typedef std::vector<Alkalmazott*> tarolo_tipus;
    tarolo_tipus t;
    Iroda(const Iroda&);                    /// így nem elérhető
    Iroda& operator=(const Iroda&);         /// így nem elérhető
public:
    /// Mivel van copy, ezért kell a paraméter nélküli.
    Iroda() {}

    /// Alkalmazottat vesz fel
    /// @param p - alkalmazott pointere
    void felvesz(Alkalmazott *p) {
        t.insert(t.end(), p);  /// insert metódusa minden standard tárolónak van
    }

    /// Meghívjuk a tárolóban levõ összes obj. kiír metódusát
    void kiir(std::ostream& os = std::cout) {
        /// iterátora minden standard tárolónak van
        for (tarolo_tipus::iterator i = t.begin(); i != t.end(); ++i)
            (*i)->kiir(os);  /// figyelje meg az indirekciót!
    }

    /// Megszüntetjük a tárolt objektumokat
    ~Iroda() {
        for (tarolo_tipus::iterator i = t.begin(); i != t.end(); ++i)
            delete *i;      /// figyelje meg az indirekciót!
    }
};

#endif // IRODA_H
