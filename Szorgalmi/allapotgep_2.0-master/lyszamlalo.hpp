/**
*    @file lyszamlalo.hpp
*    Nem kell beadni! Példa megoldás
*/

#ifndef LYSZAML_HPP
#define LYSZAML_HPP

#include <string>
#include "memtrace.h"
#include "allapotgep.hpp"

/**
 * Állapotok és input csoportok kódolása
 * Ún. scoped enum típust használunk (az enum nem osztály)
 *
 * https://en.cppreference.com/w/cpp/language/enum#Scoped_enumerations
 * hogy több enum esetén lehessen azonos név alatt elem
 */
enum class LyAllapot { alap, l_jott, ll_jott};
enum class LyInput   { l, y, egyeb };

class Lyszaml : public Allapotgep<LyAllapot, LyInput, char> {
    Allapotgep::AllTabla tab;
    int sz = 0;     // számláló

    /// Akciók és állapotátmenetek megvalósítása
    struct Novel1 : public Nop {
        Lyszaml& allapotgep;   // állapotgép referenciája, hogy elérje a számlálót
        Novel1(LyAllapot all, Lyszaml& allapotgep) : Nop(all), allapotgep(allapotgep) {}
        void akcio(char ch) { allapotgep.sz += 1; }
    };
    struct Novel2 : public Nop {
        Lyszaml& allapotgep;
        Novel2(LyAllapot all, Lyszaml& allapotgep) : Nop(all), allapotgep(allapotgep) {}
        void akcio(char ch) { allapotgep.sz += 2; }
    };
public:
    Lyszaml();
    LyInput input(char ch) const {
        if (ch == 'l') return LyInput::l;
        if (ch == 'y') return LyInput::y;
        return LyInput::egyeb;
    }
    int get() const {
        return sz;
    }
    void str(std::string str) {
        for(char c : str)
            (*this)(c);
    }
    static void runtests();
};

#endif
