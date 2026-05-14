/**
 * \file pstring.h
 *
 * Szerializálható String osztály (PString = Persistent String)
 * A PString osztály a String osztályból és a Serializable interfészből örökli a tagfüggvényeit.
 */

#ifndef PSTRING_H
#define PSTRING_H

#include "string5.h"
#include "serializable.h"

/**
 * PString osztály.
 * A String osztályból és a Serializable interfészből örökli a tagfüggvényeit.
 * Képes az adatait szerializálni (write) és deszerializálni (read).
 */
class PString : public String, public Serializable {
public:
    /// Konstruktor egy nullával lezárt char sorozatból (default és C-sztring)
    /// @param p - pointer egy C sztringre (alapértéke: "")
    PString(const char *p = "") : String(p) {}

    /// Konstruktor egy karakterből
    /// @param ch - karakter
    PString(char ch) : String(ch) {}

    /// Másoló konstruktor String-ből
    /// @param s - String, amiből létrehozzuk az új PString-et
    PString(const String& s) : String(s) {}

    /// Másoló konstruktor PString-ből
    /// @param ps - PString, amiből létrehozzuk az új PString-et
    PString(const PString& ps) : String(ps) {}

    /// write metódus: a sztringet szerializálja az adatfolyamra
    /// Először a hosszt írja ki (size_t, bináris formában),
    /// majd az adatokat (karakterek, beleértve az \n-t is).
    /// @param os - kimeneti adatfolyam
    virtual void write(std::ostream& os) const {
        size_t len = this->size();
        os.write(reinterpret_cast<const char*>(&len), sizeof(size_t));
        os.write(this->c_str(), len);
    }

    /// read metódus: a sztringet deszerializálja az adatfolyamról
    /// Először a hosszt olvassa be (size_t, bináris formában),
    /// majd az adatokat (karakterek, beleértve az \n-t is).
    /// @param is - bemeneti adatfolyam
    virtual void read(std::istream& is) {
        size_t len;
        is.read(reinterpret_cast<char*>(&len), sizeof(size_t));
        char *buffer = new char[len + 1];
        is.read(buffer, len);
        buffer[len] = '\0';
        *this = PString(buffer);
        delete[] buffer;
    }

    /// Destruktor
    virtual ~PString() {}
};

#endif // PSTRING_H
