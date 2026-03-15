#include "memtrace.h"
#include "vektor.h"
#include "gtest_lite.h"

// statikus adattagok definialasa
size_t Vektor::defSize = 11;
double Vektor::defValue = 41;

// masolo konstruktor
Vektor::Vektor(const Vektor& rhs) : nElements(rhs.nElements) {
    pVec = new double[nElements];

    for (size_t i = 0; i < nElements; i++)
        pVec[i] = rhs.pVec[i];
}

// destruktor
Vektor::~Vektor() {
    delete[] pVec;
}

// ertekadas operator
Vektor& Vektor::operator=(const Vektor& rhs) {
    if (this != &rhs) {

        delete[] pVec;

        nElements = rhs.nElements;
        pVec = new double[nElements];

        for (size_t i = 0; i < nElements; i++)
            pVec[i] = rhs.pVec[i];
    }
    return *this;
}

// indexoperator
double& Vektor::operator[](size_t idx) {

    if (idx >= nElements)
        throw "T0R2E4";

    return pVec[idx];
}

// konstans indexoperator
const double& Vektor::operator[](size_t idx) const {

    if (idx >= nElements)
        throw "T0R2E4";

    return pVec[idx];
}

// szorzas: valos * vektor
Vektor operator*(double val, const Vektor& vec) {
    Vektor result(vec.size());

    for (size_t i = 0; i < vec.size(); i++)
        result[i] = val * vec[i];
        
    return result;
}