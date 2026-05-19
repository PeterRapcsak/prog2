/**
 * \file pirodistak.h
 * Minden származtatott irodista osztály perzisztens változatának deklarációját ebbe a fájlba tegye!
 * Az egyszerűség kedvéért minden tagfüggvényt inline valósítson meg!
 *
 */
#ifndef PIRODISTAK_H
#define PIRODISTAK_H

#include <stdexcept>
#include "palkalmazott.h"
#include "irodistak.h"
/**
 * Perzisztens csoportvezető
 */
struct PHatIdeju : public HatIdeju, public Serializable {

    // Konstruktor normál adatokból
    PHatIdeju(const std::string& n, double fiz, time_t t)
        : Alkalmazott(n, fiz),
          HatIdeju(n, fiz, t)
    {}

    // Konstruktor sima HatIdeju objektumból
    PHatIdeju(const HatIdeju& hi)
        : Alkalmazott(hi),
          HatIdeju(hi)
    {}

    // Kiírás streambe
    void write(std::ostream& os) const {
        os << "PHatIdeju" << std::endl;
        os << getNev()   << std::endl;
        os << getFiz()   << std::endl;
        os << getIdo()   << std::endl;
    }

    // Beolvasás streamből
    void read(std::istream& is) {
        std::string tmp;
        (is >> tmp).ignore(1);

        // Típusnév ellenőrzése
        if (tmp != "PHatIdeju")
            throw std::out_of_range("PHatIdeju_R: " + tmp);

        std::string n;
        getline(is, n);
        setNev(n);

        double f;
        (is >> f).ignore(1);
        setFiz(f);

        time_t t;
        (is >> t).ignore(1);
        setIdo(t);
    }
};


/**
 * Perzisztens Határozott idejű alkalmazott
 */
struct PHatIdeju : public HatIdeju, public Serializable {

    // Konstruktor normál adatokból
    PHatIdeju(const std::string& n, double fiz, time_t t)
        : Alkalmazott(n, fiz),
          HatIdeju(n, fiz, t)
    {}

    // Konstruktor sima HatIdeju objektumból
    PHatIdeju(const HatIdeju& hi)
        : Alkalmazott(hi),
          HatIdeju(hi)
    {}

    // Kiírás streambe
    void write(std::ostream& os) const {
        os << "PHatIdeju" << std::endl;
        os << getNev()   << std::endl;
        os << getFiz()   << std::endl;
        os << getIdo()   << std::endl;
    }

    // Beolvasás streamből
    void read(std::istream& is) {
        std::string tmp;
        (is >> tmp).ignore(1);

        // Típusnév ellenőrzése
        if (tmp != "PHatIdeju")
            throw std::out_of_range("PHatIdeju_R: " + tmp);

        std::string n;
        getline(is, n);
        setNev(n);

        double f;
        (is >> f).ignore(1);
        setFiz(f);

        time_t t;
        (is >> t).ignore(1);
        setIdo(t);
    }
};



#endif // IRODISTAK_H
