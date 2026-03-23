/**
 *
 * \file string5.cpp
 *
 * Itt kell megvalósítania a hiányzó tagfüggvényeket.
 * Segítségül megadtuk a C++ nem OO eszközeinek felhasználásával készített String-kezelő
 * függvényke neveit.
 *
 * Ha valamit INLINE-ként valósít meg, akkor annak a string5.h-ba kell kerülnie,
 * akár kívül akár osztályon belül definiálja. (Az inline függvényeknek minden
 * fordítási egységben elérhetőknek kell lenniük)
 * *
 * A teszteléskor ne felejtse el beállítani a string5.h állományban az ELKESZULT makrót.
 *
 */

#ifdef _MSC_VER
// MSC ne adjon figyelmeztető üzenetet a C sztringkezelő függvényeire
  #define _CRT_SECURE_NO_WARNINGS
#endif

#include <iostream>             // Kiíratáshoz
#include <cstring>              // Sztringműveletekhez

#include "memtrace.h"           // a standard headerek után kell lennie
#include "string5.h"


// Konstruktor: char karakterbol
String::String(char ch) : len(1) {

    pData = new char[2];
    pData[0] = ch;
    pData[1] = '\0';
}

// Konstruktor: C-sztringbol
String::String(const char* p) : len(strlen(p)) {

    pData = new char[len + 1];

    strcpy(pData, p);

}

// Masolo konstruktor
String::String(const String& rhs) : len(rhs.len) {

    pData = new char[len + 1];

    strcpy(pData, rhs.pData);
}

// Destruktor
String::~String() {
    delete[] pData;
}

// Ertekadas operator
String& String::operator=(const String& rhs) {

    if (this != &rhs) {

        delete[] pData;
        len = rhs.len;
        pData = new char[len + 1];
        strcpy(pData, rhs.pData);
    }
    return *this;
}

// Indexeles operator
char& String::operator[](size_t idx) {

    if (idx >= len)
        throw "T0R2E4";
        
    return pData[idx];
}

// Konstans indexeles operator
const char& String::operator[](size_t idx) const {

    if (idx >= len)
        throw "T0R2E4";

    return pData[idx];
}

// String + String
String String::operator+(const String& rhs) const {
    String result;
    delete[] result.pData;

    result.len = len + rhs.len;
    result.pData = new char[result.len + 1];

    strcpy(result.pData, pData);
    strcat(result.pData, rhs.pData);

    return result;
}

// String + char
String String::operator+(char ch) const {
    return *this + String(ch);
}

// char + String (global)
String operator+(char ch, const String& str) {
    return String(ch) + str;
}

// Inserter operator
std::ostream& operator<<(std::ostream& os, const String& str) {
    os << str.c_str();
    return os;
}
