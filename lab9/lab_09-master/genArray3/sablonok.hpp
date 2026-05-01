/*
 * \file sablonok.hpp
 *
 * 9. labor - 2. feladatcsoport
 * Generikus algoritmusok és az Array sablon kiegészítései.
 */

#ifndef SABLONOK_HPP
#define SABLONOK_HPP

#include <iostream>

#define ELKESZULT 17

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

/// Generikus tömb kiíró függvénysablon
/// Kiírja a tömb méretét, kapacitását, majd az adatait vesszővel elválasztva.
/// Feltételezi, hogy a generikus tömbnek van: ForwardIteratora, size() és capacity() tagfüggvénye.
/// @tparam T   - sablon paraméter: iterátoros tömb típus
/// @param txt  - kiírandó fejléc szöveg
/// @param arr  - konkrét tömb (const referencia)
/// @param os   - kimeneti stream (alapértelmezett: std::cout)
template <class T>
void PrintArray(const char *txt, const T& arr, std::ostream& os = std::cout) {
    os << txt << " size=" << arr.size()
         << " capacity=" << arr.capacity() << std::endl;
    os << "\t data=";
    // kiírjuk az adatokat vesszővel elválasztva, a printEach sablonnal
    printEach(arr.begin(), arr.end(), os, ",");
}

/// forEach generikus algoritmus
/// Két iterátorral megadott adathalmaz minden elemére meghívja a paraméterként kapott függvényt.
/// @tparam InputIterator - iterátor típusa
/// @tparam Func          - meghívandó függvény/funktor típusa
/// @param first - sorozat elejére mutató iterátor
/// @param last  - sorozat végét jelző iterátor
/// @param func  - meghívandó függvény/funktor
/// @return - maga a meghívott függvény/funktor (így kiolvasható az összegyűjtött eredmény)
template <typename InputIterator, class Func>
Func forEach(InputIterator first, InputIterator last, Func func) {
    while (first != last) {
        func(*first);
        ++first;
    }
    return func;
}

/// Stream kiíró funktor
/// Konstruktorban megjegyzi a stream-et és az elválasztót,
/// a függvényhívás operátor kiírja a paramétert a stream-re, majd az elválasztót.
/// @tparam T - kiírandó érték típusa
template <typename T>
class ostreamFunctor {
    std::ostream& os;   // kimeneti stream
    const char* sep;    // elválasztó karaktersorozat
public:
    /// Konstruktor
    /// @param os  - kimeneti stream referenciája
    /// @param sep - elválasztó karaktersorozat (alapértelmezett: "")
    ostreamFunctor(std::ostream& os, const char* sep = "") : os(os), sep(sep) {}

    /// Függvényhívás operátor: kiírja az értéket és az elválasztót
    /// @param x - kiírandó érték
    void operator()(const T& x) {
        os << x << sep;
    }
};

#endif
