/**
 *  \file rajztabla.cpp
 *   Ebben a fájlban valósítsa meg a Rajztábla osztály nem inline tagfüggvényeit
 */

#include "rajztabla.h"

/// Alakzatot tesz a rajztáblára
/// ap - pointer az alakzatra
void Rajztabla::felrak(Alakzat *ap) {
    if (db < MAXDB) {
        tabla[db++] = ap;
    }
}

/// Kirajzolja az összes alakzatot
void Rajztabla::rajzol() const {
    for (size_t i = 0; i < db; i++)
        tabla[i]->rajzol();
}

/// Az összes alakzatot elmozdítja
/// d - eltolás vektora
void Rajztabla::mozgat(const Pont& d) const {
    for (size_t i = 0; i < db; i++)
        tabla[i]->mozgat(d);
}

/// letörli a táblát, eltávolítja az objektumokat
void Rajztabla::torol() {
    for (size_t i = 0; i < db; i++)
        delete tabla[i];
    db = 0;
}

/// Globális << operátor a Rajztabla adatainak kiírásához
std::ostream& operator<<(std::ostream& os, const Rajztabla& t) {
    return os << "Tabla size=" << t.size();
}

