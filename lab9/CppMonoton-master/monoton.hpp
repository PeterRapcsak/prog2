/*
 * \file monoton.hpp
 *
 * Generikus algoritmus: monoton sorozat eldöntése.
 */

#ifndef MONOTON_HPP
#define MONOTON_HPP

/// @tparam InputIterator - iterátor típusa
/// @tparam Predicate     - kétoperandusú predikátum típusa
/// @param first - sorozat elejére mutató iterátor (zárt)
/// @param last  - sorozat végét jelző iterátor (nyílt)
/// @param pred  - kétoperandusú predikátum: pred(kovetkezo, elozo) vizsgálja az elemek viszonyát
/// @return - igaz, ha a sorozat monoton a predikátum szerint (üres vagy egyelemű sorozat mindig igaz)
template <typename InputIterator, typename Predicate>
bool monoton(InputIterator first, InputIterator last, Predicate pred) {
    if (first == last) return true;     // üres sorozat -> monoton

    InputIterator prev = first; // előző elem iterátora
    InputIterator curr = first; // aktuális elem iterátora
    curr++;                     // a második elemre lép

    while (curr != last) {
        if (!pred(*curr, *prev))
            return false;  // feltétel megsértve: nem monoton
        prev = curr;    // léptetés
        curr++;         // curr növelése
    }
    return true;        // minden szomszédos pár jó
}

#endif