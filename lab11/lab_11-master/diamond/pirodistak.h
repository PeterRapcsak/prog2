/**
 * \file pirodistak.h
 * Minden származtatott irodista osztály perzisztens változatának deklarációját ebbe a fájlba tegye!
 * Az egyszerűség kedvéért minden tagfüggvényt inline valósítson meg!
 *
 */
#ifndef PIRODISTAK_H
#define PIRODISTAK_H

#include <stdexcept>
#include <string>
#include "palkalmazott.h"
#include "irodistak.h"

/**
 * Perzisztens csoportvezető
 */
struct PCsopVez : public CsopVez, public Serializable {

    // Konstruktor normál adatokból
    PCsopVez(const std::string& n, double fiz, csop_t cs)
        : Alkalmazott(n, fiz),
          CsopVez(n, fiz, cs)
    {}

    // Konstruktor sima CsopVez objektumból
    PCsopVez(const CsopVez& csv)
        : Alkalmazott(csv),
          CsopVez(csv)
    {}

    // Kiírás streambe
    void write(std::ostream& os) const {
        os << "PCsopVez" << std::endl;
        os << getNev()   << std::endl;
        os << getFiz()   << std::endl;
        os << getCs()    << std::endl;
    }

    // Beolvasás streamből
    void read(std::istream& is) {
        std::string tmp;
        (is >> tmp).ignore(1);

        // Típusnév ellenőrzése
        if (tmp != "PCsopVez")
            throw std::out_of_range("PCsopVez_R: " + tmp);

        std::string n;
        getline(is, n);
        setNev(n);

        double f;
        (is >> f).ignore(1);
        setFiz(f);

        csop_t cs;
        (is >> cs).ignore(1);
        setCs(cs);
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


/**
 * Perzisztens Határozott idejű csoportvezető
 */
struct PHatIdCsV : public HatIdCsV, public Serializable {

    // Konstruktor normál adatokból
    PHatIdCsV(const std::string& n, double fiz, csop_t cs, time_t t)
        : Alkalmazott(n, fiz),
          HatIdCsV(n, fiz, cs, t)
    {}

    // Konstruktor sima HatIdCsV objektumból
    PHatIdCsV(const HatIdCsV& hicv)
        : Alkalmazott(hicv),
          HatIdCsV(hicv)
    {}

    // Kiírás streambe
    void write(std::ostream& os) const {
        os << "PHatIdCsV" << std::endl;
        os << getNev()   << std::endl;
        os << getFiz()   << std::endl;
        os << getCs()    << std::endl;
        os << getIdo()   << std::endl;
    }

    // Beolvasás streamből
    void read(std::istream& is) {
        std::string tmp;
        (is >> tmp).ignore(1);

        // Típusnév ellenőrzése
        if (tmp != "PHatIdCsV")
            throw std::out_of_range("PHatIdCsV_R: " + tmp);

        std::string n;
        getline(is, n);
        setNev(n);

        double f;
        (is >> f).ignore(1);
        setFiz(f);

        csop_t cs;
        (is >> cs).ignore(1);
        setCs(cs);

        time_t t;
        (is >> t).ignore(1);
        setIdo(t);
    }
};


/**
 * Perzisztens Határozott idejű csoportvezető-helyettes
 */
struct PHatIdCsVezH : public HatIdCsVezH, public Serializable {

    // Konstruktor normál adatokból
    PHatIdCsVezH(const std::string& n, double fiz, time_t t, CsopVez& kit)
        : Alkalmazott(n, fiz),
          HatIdCsVezH(n, fiz, t, kit)
    {}

    // Konstruktor sima HatIdCsVezH objektumból
    PHatIdCsVezH(const HatIdCsVezH& hicvh)
        : Alkalmazott(hicvh),
          HatIdCsVezH(hicvh)
    {}

    // Kiírás streambe
    void write(std::ostream& os) const {
        os << "PHatIdCsVezH" << std::endl;
        os << getNev()   << std::endl;
        os << getFiz()   << std::endl;
        os << getCs()    << std::endl;
        os << getIdo()   << std::endl;
    }

    // Beolvasás streamből
    void read(std::istream& is) {
        std::string tmp;
        (is >> tmp).ignore(1);

        // Típusnév ellenőrzése
        if (tmp != "PHatIdCsVezH")
            throw std::out_of_range("PHatIdCsVezH_R: " + tmp);

        std::string n;
        getline(is, n);
        setNev(n);

        double f;
        (is >> f).ignore(1);
        setFiz(f);

        csop_t cs;
        (is >> cs).ignore(1);
        setCs(cs);

        time_t t;
        (is >> t).ignore(1);
        setIdo(t);
    }
};



#endif // DIAMOND_PIRODISTAK_H
