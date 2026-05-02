#ifndef MONOTON_HPP
#define MONOTON_HPP

/// @tparam InputIterator - iterátor típusa
/// @tparam Feltetel - kétoperandusú predikátum típusa
/// @return Igaz, ha a sorozat monoton a feltetel szerint 
template <typename InputIterator, typename Feltetel>
bool monoton(   InputIterator first,    // sorozat elejére mutató iterátor
                InputIterator last,     // sorozat végét jelző iterátor
                Feltetel pred) {        // vizsgálja az elemek viszonyát

    if (first == last) return true;     // üres sorozat -> monoton

    InputIterator prev = first; // előző elem iterátora
    InputIterator curr = first; // aktuális elem iterátora
    curr++;                     // a második elemre lép

    while (curr != last) {
        if (!pred(*curr, *prev))
            return false;   // feltétel nem teljesül -> nem monoton
        prev = curr;        // léptetés
        curr++;             // curr növelése
    }
    return true; // minden szomszédos pár jó
}

#endif 