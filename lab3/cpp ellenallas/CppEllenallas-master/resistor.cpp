/**
 * \file resistor.cpp
 *
 * Ohmikus ellenallast modellezo osztaly megvalositasa
 */

#include "resistor.h"

// Statikus adattag definialasa (66 ohm by default)
#ifndef NO_STATIC
double Resistor::defR = 66;
#endif

// Alapértelmezett konstruktor
Resistor::Resistor() {
#ifndef NO_STATIC
    R = defR;
#else
    R = 66;
#endif
}

// Konstruktor ellenallas ertekkel
Resistor::Resistor(double r) : R(r) {
}

#ifndef NO_STATIC
// Alapértelmezett ellenallas ertek beallitasa
void Resistor::setDef(double r) {
    defR = r;
}
#endif

// Soros kapcsolas operator
Resistor Resistor::operator+(const Resistor& r) const {
    return Resistor(R + r.R);
}

// Parhuzamos kapcsolas operator
Resistor Resistor::operator%(const Resistor& r) const {
    return Resistor(1.0 / (1.0/R + 1.0/r.R));
}

// n darab ellenallas soros kapcsolasa
Resistor operator*(int n, const Resistor& r) {
    if (n <= 0) {
        throw "T0R2E4";
    }
    return Resistor(n * r.getR());
}