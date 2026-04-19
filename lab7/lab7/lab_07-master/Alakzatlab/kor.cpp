/**
 *  \file kor.cpp
 *  Kor osztály tagfüggvényeinek megvalósítása
 */
#include "kor.h"

/// Kört rajzoló rajzol fv.
void Kor::rajzol() const {
    std::cout << "Rajzol: " << *this << std::endl;
}

/// Globális << operátor a Kor adatainak kiírásához
std::ostream& operator<<(std::ostream& os, const Kor& k) {
    return os << "Kor " << (Alakzat&)k << ",r=" << k.getr();
}
