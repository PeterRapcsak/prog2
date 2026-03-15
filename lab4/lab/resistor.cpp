#include "resistor.h"

double Resistor::defR = 66;

// alapertelmezett konstruktor
Resistor::Resistor() : R(defR) {
    Pr("ctor0");
}

// ellenallas ertekkel
Resistor::Resistor(double r) : R(r) {
    Pr("ctor1");
}

// alapertelmezett ellenallas ertek beallitasa
void Resistor::setDef(double r) {
    defR = r;
}

// soros kapcsolas operator
Resistor Resistor::operator+(const Resistor& r) const {
    return Resistor(R + r.R);
}

// parhuzamos kapcsolas operator
Resistor Resistor::operator%(const Resistor& r) const {
    return Resistor(1.0 / (1.0/R + 1.0/r.R));
}

//! #######################################################
#if ELKESZULT >= 2

// masolo konstruktor
Resistor::Resistor(const Resistor& rhs) : R(rhs.R) {
    Pr("copy");
}
#endif

//! #######################################################
#if ELKESZULT >= 3

// destruktor
Resistor::~Resistor() {
    Pr("dtor");
}
#endif

//! #######################################################
#if ELKESZULT >= 4

// ertekadas operator
Resistor& Resistor::operator=(const Resistor& rhs) {
    Pr("assign");
    R = rhs.R;
    return *this;
}
#endif

//! #######################################################
#if ELKESZULT >= 5

// ellenallason folyo aram
double Resistor::getI(double u) const {
    return u / R;
}
#endif

//! #######################################################
#if ELKESZULT >= 6

// ellenallason feszultseg
double Resistor::getU(double i) const {
    return R * i;
}
#endif

// n darab ellenallas soros kapcsolasa
Resistor operator*(int n, const Resistor& r) {
    if (n <= 0) {
        throw "T0R2E4";
    }
    return Resistor(n * r.getR());
}

// inserter
std::ostream& operator<<(std::ostream& os, const Resistor& rhs) {
    os << rhs.getR();
    return os;
}
