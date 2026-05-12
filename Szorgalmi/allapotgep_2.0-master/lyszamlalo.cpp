/**
*    @file lyszamlalo.cpp
*    Nem kell beadni! Példa megoldás
*/

#include <string>
#include "memtrace.h"
#include "lyszamlalo.hpp"
#include "gtest_lite.h"

/// Default konstruktor
/// A kezdő állapot(alap) és az állapotátmenet tábla megadása és beállítása
Lyszaml::Lyszaml() : Allapotgep(LyAllapot::alap, tab), sz(0) {
    tab[LyAllapot::alap] =   {{LyInput::l, new Nop(LyAllapot::l_jott)},
                              {LyInput::y, new Nop(LyAllapot::alap)},
                              {LyInput::egyeb, new Nop(LyAllapot::alap)}};
    tab[LyAllapot::l_jott] = {{LyInput::l, new Nop(LyAllapot::ll_jott)},
                              {LyInput::y, new Novel1(LyAllapot::alap, *this)},
                              {LyInput::egyeb, new Nop(LyAllapot::alap)}};
    tab[LyAllapot::ll_jott]= {{LyInput::l, new Nop(LyAllapot::ll_jott)},
                              {LyInput::y, new Novel2(LyAllapot::alap, *this)},
                              {LyInput::egyeb, new Nop(LyAllapot::alap)}};
}

/// példa tesztek a demó programhoz
void Lyszaml::runtests() {
    TEST(Lyszaml Test, alma) {
            Lyszaml szaml;
            szaml.str("alma");
            EXPECT_EQ(0, szaml.get());
        } END

    TEST(Lyszaml Test, ayya) {
            Lyszaml szaml;
            szaml.str("ayya");
            EXPECT_EQ(0, szaml.get());
        } END

    TEST(Lyszaml Test, bagoly) {
            Lyszaml szaml;
            szaml.str("bagoly");
            EXPECT_EQ(1, szaml.get());
        } END

    TEST(Lyszaml Test, gally) {
            Lyszaml szaml;
            szaml.str("gally");
            EXPECT_EQ(2, szaml.get());
        } END
}
