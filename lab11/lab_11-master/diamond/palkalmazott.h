/**
 * \file palkalmazott.h
 * Perzisztens viselkedésre képes Alkalmazott alaposztály deklarációja
 * Az egyszerűség kedvéért minden tagfüggvényt inline valósítottunk meg.
 *
 */

#ifndef PALKALMAZOTT_H
#define PALKALMAZOTT_H
#include <string>
#include <stdexcept>

#include "alkalmazott.h"
#include "serializable.h"

/*
 *  Perzisztens Alkalmazott
 */
struct PAlkalmazott : public Alkalmazott, public Serializable {
    PAlkalmazott(const std::string& n, double fiz) : Alkalmazott(n, fiz) {}
    PAlkalmazott(const Alkalmazott& alk) :Alkalmazott(alk) {}
    void write(std::ostream& os) const {
        os << "PAlkalmazott" << std::endl;
        os << getNev() << std::endl;
        os << getFiz() << std::endl;
    }
    void read(std:: istream& is) {
        std::string tmp;
        (is >> tmp).ignore(1);
        if (tmp != "PAlkalmazott") throw std::out_of_range("PAlkalmazott_R: "+tmp);
        std::string n;
        getline(is, n);
        setNev(n);
        double f;
        (is >> f).ignore(1);
        setFiz(f);
    }
};

#endif // PALKALMAZOTT_H
