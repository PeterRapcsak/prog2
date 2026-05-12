/**
*    @file main.cpp
*    Nem kell beadni!
*/
#include <iostream>
#include <string>
#include "memtrace.h"
#include "gtest_lite.h"
#include "allapotgep.hpp"
#include "lyszamlalo.hpp"
#include "komment.hpp"
#include "split.hpp"

/*
ELKESZULT makro
    == 0 - a demó program fut le az Lyszaml-val
    == 1 - kommentes tesztek
    == 2 - komment és split állapotgépes tesztek   */
#define ELKESZULT 0
#if ELKESZULT == 0

int main() {
    Lyszaml::runtests();

    std::cout << "Input olvasasa EOF-ig:" << std::endl;
    Lyszaml ly;
    char ch;
    while (std::cin >> std::noskipws >> ch)
        ly(ch);
    std::cout << std::endl << ly.get() << std::endl;

    return 0;
}
#elif ELKESZULT > 0

template <typename T>
void runstr(T& allapotgep, std::string str) {
    for(char c : str)
        allapotgep(c);
}

int main() {
    GTINIT(std::cin);
#if ELKESZULT > 0
    TEST(Komment, nincs) {
        Komment k;
        runstr(k, "/alma*/ **\\/valami");
        EXPECT_STREQ("/alma*/ **\\/valami", k.get().c_str());
        runstr(k, "c++");
        EXPECT_STREQ("/alma*/ **\\/valamic++", k.get().c_str());
    } END
    TEST(Komment, egyszeru) {
        Komment k;
        runstr(k, "akarmi/*geza*/valami");
        EXPECT_STREQ("akarmivalami", k.get().c_str());
        runstr(k, "c++");
        EXPECT_STREQ("akarmivalamic++", k.get().c_str());
    } END
    TEST(Komment, latin) {
        Komment k;
        runstr(k, "/*c++**/*Ad maiorem Dei gloriam/**:)(*/ Alea iacta est /**/Et tu mi fili, Brute?");
        EXPECT_STREQ("*Ad maiorem Dei gloriam Alea iacta est Et tu mi fili, Brute?", k.get().c_str());
    } END
#endif

#if ELKESZULT > 1
    TEST(Split, gyumolcsok) {
        Split split(';');
        runstr(split, "alma;banan;korte;");
        EXPECT_STREQ("alma",  split.get()[0].c_str());
        EXPECT_STREQ("banan", split.get()[1].c_str());
        EXPECT_STREQ("korte", split.get()[2].c_str());
        EXPECT_EQ(3u, split.get().size());
        runstr(split, "szilva");
        EXPECT_EQ(4u, split.get().size());
        split(';');
        EXPECT_STREQ("szilva", split.get()[3].c_str());
    } END

    TEST(Split, gyumolcsok2.0) {
        Split split('/');
        runstr(split, "alma//banan///korte/");
        EXPECT_STREQ("alma",  split.get()[0].c_str());
        EXPECT_STREQ("banan", split.get()[1].c_str());
        EXPECT_STREQ("korte", split.get()[2].c_str());
    } END
#endif

    GTEND(std::cerr);
    return 0;
}
#endif
