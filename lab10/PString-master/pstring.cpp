#include "pstring.h"
#include <iostream>
#include <cstddef>


//! ---------- KONSTRUKTOROK ----------

PString::PString(const char *p) : String(p) {}

PString::PString(char ch) : String(ch) {}

PString::PString(const String& s) : String(s) {}

PString::PString(const PString& ps) : String(ps) {}


//! ---------- KIÍRÁS ----------

// CÉL: A sztring bináris mentése
void PString::write(std::ostream& os) const {
    std::size_t len = this->size(); // String mérete

    // Hossz kiírása binárisan
    os.write((const char *)&len, sizeof len);

    // Szöveg karaktereinek kiírása
    os.write(this->c_str(), len);
}


//! ---------- BEOLVASÁS ----------

// CÉL: A sztring visszaolvasása bináris streamből
void PString::read(std::istream& is) {
    std::size_t len = 0;

    // Hossz beolvasása binárisan
    is.read((char *)&len, sizeof len);

    // Ide olvassuk be a szöveg karaktereit
    std::string buf(len, '\0');

    // Pontosan len darab karakter beolvasása
    is.read(&buf[0], len);

    // A beolvasott szövegből új PString lesz
    *this = PString(buf.c_str());
}


//! ---------- DESTRUKTOR ----------

// CÉL: Virtuális destruktor megvalósítása
PString::~PString() {}