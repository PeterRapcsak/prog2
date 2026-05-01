/*
 * \file sablonok.hpp
 *
 * 9. labor - 1. feladatcsoport
 * Generikus algoritmusok sablonok.
 */

#ifndef SABLONOK_HPP
#define SABLONOK_HPP

#include <iostream>

#define ELKESZULT 7

/// Kiíró generikus függvény
/// Egy adatsorozat iterátorokkal megadott elemeit szeparátorral elválasztva
/// írja ki a megadott stream-re. Az utolsó elem után soremelést ír.
/// @param first - sorozat elejére mutató iterátor (zárt)
/// @param last  - sorozat végét jelző iterátor (nyílt)
/// @param os    - kimeneti stream (alapértelmezett: std::cout)
/// @param sep   - elválasztó karaktersorozat (alapértelmezett: ", ")
template <typename InputIterator>
void printEach(InputIterator first, InputIterator last,
               std::ostream& os = std::cout,
               const char* sep = ", ") {
    if (first == last) {
        os << std::endl;
        return;
    }
    os << *first;
    ++first;
    while (first != last) {
        os << sep << *first;
        ++first;
    }
    os << std::endl;
}

/// Negatív elemek számlálója
/// Megszámolja, hány negatív elem van az iterátorokkal megadott sorozatban.
/// Feltételezi, hogy az adatra értelmezett az adat < 0 művelet.
/// @param first - sorozat elejére mutató iterátor
/// @param last  - sorozat végét jelző iterátor
/// @return - negatív elemek száma
template <typename InputIterator>
int szamol_ha_negativ(InputIterator first, InputIterator last) {
    int db = 0;
    while (first != last) {
        if (*first < 0) ++db;
        ++first;
    }
    return db;
}

/// Nagyobb mint funktor
/// Konstruktorban megjegyzi az értéket, a függvényhívás operátor igaz értéket
/// ad, ha a paraméter nagyobb a megjegyzett értéknél.
/// @tparam T - összehasonlítandó típus
template <typename T>
class nagyobb_mint {
    T ertek;    // megjegyzett érték
public:
    /// Konstruktor: megjegyzi az értéket
    /// @param e - megjegyezendő érték
    nagyobb_mint(const T& e) : ertek(e) {}

    /// Függvényhívás operátor
    /// @param x - összehasonlítandó érték
    /// @return  - igaz, ha x > ertek
    bool operator()(const T& x) const {
        return x > ertek;
    }
};

/// Feltételes számolás generikus függvény
/// Megszámolja, hány elem tesz eleget a predikátumnak az iterátorokkal megadott sorozatban.
/// @param first - sorozat elejére mutató iterátor
/// @param last  - sorozat végét jelző iterátor
/// @param pred  - egyoperandusú predikátum függvény/funktor
/// @return - predikátumot kielégítő elemek száma
template <typename InputIterator, typename Predicate>
int szamol_ha(InputIterator first, InputIterator last, Predicate pred) {
    int db = 0;
    while (first != last) {
        if (pred(*first)) ++db;
        ++first;
    }
    return db;
}

#endif
