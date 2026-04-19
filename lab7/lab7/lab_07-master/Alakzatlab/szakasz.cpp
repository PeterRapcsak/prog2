/**
 *  \file szakasz.cpp
 *  Szakasz osztály tagfüggvényeinek megvalósítása
 */
#include "szakasz.h"

/// Szakaszt rajzoló rajzol fv.
void Szakasz::rajzol() const {
    std::cout << "Rajzol: " << *this << std::endl;
}

/// Globális << operátor a Szakasz adatainak kiírásához
std::ostream& operator<<(std::ostream& os, const Szakasz& sz) {
    return os << "Szakasz " << (Alakzat&)sz << ",pv=" << sz.getpv();
}
