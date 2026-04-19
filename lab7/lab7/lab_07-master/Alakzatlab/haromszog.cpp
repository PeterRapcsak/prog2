/**
 *  \file haromszog.cpp
 *  Haromszog osztály tagfüggvényeinek megvalósítása
 */
#include "haromszog.h"

/// Háromszöget rajzoló rajzol fv.
void Haromszog::rajzol() const {
    std::cout << "Rajzol: " << *this << std::endl;
}

/// Globális << operátor a Haromszog adatainak kiírásához
std::ostream& operator<<(std::ostream& os, const Haromszog& h) {
    return os << "Haromszog " << (Alakzat&)h << ",pb=" << h.getpb() << ",pc=" << h.getpc();
}

