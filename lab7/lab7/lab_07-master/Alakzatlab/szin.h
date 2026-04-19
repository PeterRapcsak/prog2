/**
 *  \file szin.h
 *  Szin deklarációja
 */
#ifndef SZIN_H
#define SZIN_H

#include <iostream>     // ostream miatt kell

/// Szín
enum Szin { BLACK, WHITE, RED, GREEN, BLUE };

/// Háttérszín
const Szin BACKGND = BLACK;

/// Globális << operátor a Szin kiírásához
/// @param os - ostream
/// @param sz - szín
/// @return os - ostream
std::ostream& operator<<(std::ostream& os, Szin sz);
#endif // SZIN_H
