/**
 * \file rstring.cpp
 *
 * RString osztály implemntációja
 * Valósítsd meg az osztály metódusait, a leírást a header fájlban találod.
 * 
 * Ezt a fájlt be kell adni (fel kell tölteni) a megoldással.
 */

#include "memtrace.h"
#include "rstring.h"
#include <cstring>


const char* NEPTUN = "T0R2E4";

//! ------------ CTOR + DTOR ------------
RString::RString(size_t capacity) {
    if (capacity <= 0) {
        throw NEPTUN;
    }

    cap = capacity;
    data = new char[cap];
    data[0] = '\0';
    // Ez így eddig egy sima C-s string
}

RString::RString(const char* str) {
    if (str == 0) {
        throw NEPTUN;
    }

    // Lezáró \0 miatt +1 kell
    cap = std::strlen(str) + 1;
    data = new char[cap];
    
    std::strcpy(data, str);
}

RString::RString(const char* str, size_t capacity) {
    if (str == 0 || capacity <= 0 || std::strlen(str) + 1 > capacity) {
        throw NEPTUN;
    }

    cap = capacity;
    data = new char[cap];
    
    std::strcpy(data, str);
    // Ugyanaz mint az előbb
}

RString::RString(const RString& other) {

    cap = other.cap;
    data = new char[cap];
    
    std::strcpy(data, other.data);
    // Copy ctor, másik kapacitásával.
}

RString::~RString() {
    delete[] data;
}

//! ------------ GETTEREK ------------
size_t RString::size() const {
    return std::strlen(data);
}
 
size_t RString::capacity() const {
    return cap;
}

RString::operator char*() const {
    return data;
}

//! ------------ OPERÁTOROK ------------
RString& RString::operator=(const char* rhs) {
    if (rhs == 0 || std::strlen(rhs) + 1 > cap) {
        throw NEPTUN;
    }
    
    std::strcpy(data, rhs);
    return *this;
}

RString& RString::operator=(const RString& rhs) {
    if (this == &rhs) {
        return *this;
    }

    char* newData = new char[rhs.cap];

    std::strcpy(newData, rhs.data);

    // Átvitel a lhs-ra
    delete[] data;
    
    data = newData;
    cap = rhs.cap;

    return *this;
}
