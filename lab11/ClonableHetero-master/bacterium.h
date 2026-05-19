/**
 *  \file: bacterium.h
 *
 *  Bacterium alaposztály és leszármazottjai
 */

#ifndef BACTERIUM_H
#define BACTERIUM_H

#include <string>

/// Bacterium osztály.
class Bacterium {
    std::string name;
public:
    /// @param n - baktérium neve
    Bacterium(const char *n = "") : name(n) {}

    /// show
    /// @param os - adatfolyam amire kiírja a nevét
    virtual void show(std::ostream& os) const {
        os << "Bakterium " << name;
    };
    virtual ~Bacterium() {}
};

/// Salmonella osztály.
/// Bacterium virtuális alaposztályból származik
/// a többszörös öröklési úton való elérésből adódó problémák kiküszöbölése miatt.
class Salmonella : virtual public Bacterium {
    std::string species;     // faj neve
public:
    /// @param n - salmonella fajtája
    Salmonella(const char *sp) :Bacterium("Salmonella"), species(sp) {}

    /// show
    /// @param os - adatfolyam amire kiírja a nevét és fajtáját
    void show(std::ostream& os) const {
        Bacterium::show(os);
        os << " Fajta: " << species;
    }
};

/// Streptococcus osztály.
/// Bacterium virtuális alaposztályból származik
/// a többszörös öröklési úton való elérésből adódó problémák kiküszöbölése miatt.
class  Streptococcus : virtual public Bacterium {
    char group;     // csoport neve
public:
    /// @param n - Streptococus baci csoportja
    Streptococcus(char grp) :Bacterium("Streptococcus"), group(grp) {}

    /// show
    /// @param os - adatfolyam amire kiírja a nevét és csoportját
    void show(std::ostream& os) const {
        Bacterium::show(os);
        os << " Csoport: " << group;
    }
};

/// Funktor a kiíráshoz
/// Konstruktor paraméterben megadott adatfolyamra ír
class BacteriumShow {
    std::ostream& os;
public:
    BacteriumShow(std::ostream& os = std::cout) :os(os) {}
    void operator()(Bacterium *p) {
        p->show(os);
        os << std::endl;
    }
};
#endif // BACTERIUM_H
