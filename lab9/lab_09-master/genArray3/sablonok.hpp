/*
 * \file sablonok.hpp
 */

#ifndef SABLONOK_HPP
#define SABLONOK_HPP

#include <iostream>

#define ELKESZULT 7

// Kiíró függvény
template <typename InputIterator>
void printEach( InputIterator first,            // sorozat eleje iterátor
                InputIterator last,             // sorozat vége iterátor
                std::ostream& os = std::cout,   // kimeneti stream (alapértelmezett: std::cout)
                const char* sep = ", ") {       // elválasztó karaktersorozat (alapértelmezett: ", ")

    if (first == last) {
        os << std::endl; // Üres kezelése
        return;
    }

    os << *first;
    first++;

    while (first != last) {
        os << sep << *first;
        first++;
    }
    os << std::endl;
}

/// Negatív elemek számlálója
template <typename InputIterator>
int szamol_ha_negativ(  InputIterator first,    // sorozat eleje iterátor
                        InputIterator last) {   // sorozat vége iterátor

    int db = 0;
    while (first != last) {
        if (*first < 0) ++db;
        first++;
    }
    return db;
}


/// Nagyobb mint funktor
template <typename T>
class nagyobb_mint {
    T ertek;    // megjegyzett érték
public:

    // Konstruktor -> megjegyzi a paraméterként kapott értéket 
    nagyobb_mint(const T& e) : ertek(e) {}

    // Függvényhívás operátor
    // x - összehasonlítandó érték
        bool operator()(const T& x) const {
        return x > ertek;
    }
};

/// Feltételes számolás generikus függvény
template <typename InputIterator, typename Predicate>
int szamol_ha(  InputIterator first,    // sorozat eleje iterátor
                InputIterator last,     // sorozat vége iterátor
                Predicate pred) {       // egyoperandusú predikátum függvény/funktor

    int db = 0;
    while (first != last) {
        if (pred(*first)) ++db;
        first++;
    }
    return db;
}

#endif
