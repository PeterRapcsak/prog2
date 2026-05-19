/**
 * \file: clonable_hetero.cpp
 *
 *  Tesztprogram a klónozható heterogén kollekciót tároló
 *  ClonableHeteroStore osztály működésének ellenőrzéséhez
 *
 */

#include <iostream>
#include <sstream>
#include <list>

#include "gtest_lite.h"
#include "memtrace.h"
#include "clonable_hetero_store.hpp"
#include "clonable_testclass.h"
#ifdef BACI_IS_VAN
#include "clonable_bacterium.h"
#endif
#include "bacterium.h"

/// Saját kivételosztály az ellenőrzéshez
struct MyException : std::exception {
    MyException(...) {}
};

/// Saját fix méretű tároló az ellenőrzéshez.
/// Egyszerű adatper az std::vector-ra.
template <typename T, size_t siz>
struct fixVec : public std::vector<T> {
    void push_back(const T& d) {
        if (std::vector<T>::size() >= siz) throw std::bad_alloc();
        std::vector<T>::push_back(d);
    }
};

int main() {

    GTINIT(std::cin);       // Csak C(J)PORTA működéséhez kell

    /// Csak a méreteket ellenőrizzük
    /// Saját tárolóval ellenőrizzük, hogy dob-e kivételt
    TEST(Test1, Sanity) {
        /// Saját tárolóban csak 2 hely van!
        ClonableHeteroStore<ClonableTestClass, fixVec<ClonableTestClass*, 2> > t1;

        /// Egyben példa az unsigned érték literállal történő összehasonlítására, vagy cast-olunk.
        EXPECT_TRUE(0 == t1.size()) << "Nem ures?" << std::endl;

        EXPECT_NO_THROW(t1.add(NULL)) << "Nem jo a meret!" << std::endl;

        /// Itt cast-olunk
        EXPECT_EQ((size_t)1, t1.size());
        EXPECT_NO_THROW(t1.add(NULL));
        EXPECT_EQ((size_t)2, t1.size())  << "Nem jo a meret!" << std::endl;
        EXPECT_THROW(t1.add(NULL), std::out_of_range&);
        t1.clear();
        EXPECT_EQ((size_t)0, t1.size()) << "Nem torlodott?" << std::endl;
    } ENDM

    /// Saját tárolóval ellenőrizzük, hogy dob-e SAJÁT kivételt
    /// Ellenőrizzük, hogy ha kivételt dob előtte felszabadítja-e, ami nem fért be.
    TEST(Test2, Exception) {
        /// Saját tárolóban csak 1 hely van !
        ClonableHeteroStore<ClonableTestClass, fixVec<ClonableTestClass*, 1>, MyException> t1;
        EXPECT_NO_THROW(t1.add(new ClonableTestClass));
        EXPECT_THROW(t1.add(new ClonableTestClass), MyException&);   /// Kivétel esetén fel kell szabadítania

    } ENDMsg("Lehet, hogy hiba eseten nem szabaditotta fel a 'tarolora bizott' objektumot?")

    /// Megnézzük std::list-el is, ami nem indexelhető
    TEST(Test3, List) {
        /// Heterogén tárroló stt::list-tel
        ClonableHeteroStore<ClonableTestClass, std::list<ClonableTestClass*> > t1;
        EXPECT_TRUE(0 == t1.size()) << "Nem ures?" << std::endl;

        EXPECT_NO_THROW(t1.add(NULL)) << "Nem jo a meret!" << std::endl;

        EXPECT_EQ((size_t)1, t1.size());
        EXPECT_NO_THROW(t1.add(NULL));
        EXPECT_EQ((size_t)2, t1.size())  << "Nem jo a meret!" << std::endl;
        t1.clear();                                                 /// törli a tárolót
        EXPECT_EQ((size_t)0, t1.size()) << "Nem torlodott?" << std::endl;
    } ENDM


    /// Megnézzük, hogy jól működik-e tároló valós objektummal.
    /// Megnézzük, hogy van-e esetleg szeletelődés
    TEST(Test4, Heterogen) {
        /// Default tároló (std::vector-ral)
        ClonableHeteroStore<ClonableTestClass> t1;
        t1.add(new ClonableTestClass1(1));
        t1.add(new ClonableTestClass1(2));
        t1.add(new ClonableTestClass2("Hello"));
        t1.add(new ClonableTestClass2("World"));
        t1.add(new ClonableTestClass1(3));
        t1.add(new ClonableTestClass2("Bye"));

        std::stringstream ss;
        TestClassPrint print(ss);                       /// stringstream-be írunk
        std::for_each(t1.begin(), t1.end(), print);     /// ez lett a traverse helyett
        EXPECT_STREQ("1;2;Hello;World;3;Bye;", ss.str().c_str()) <<
            "Valami baj van! Szeletelodes?" << std::endl;
    } ENDM


    /// Megnézzük, hogy jól működik-e a másoló ctor.
    TEST(Test4, Copy) {
        ClonableHeteroStore<ClonableTestClass> t1;      ///
        t1.add(new ClonableTestClass1(1));
        t1.add(new ClonableTestClass1(2));
        t1.add(new ClonableTestClass2("Hello"));
        t1.add(new ClonableTestClass2("World"));
        t1.add(new ClonableTestClass1(3));
        t1.add(new ClonableTestClass2("Bye"));

        ClonableHeteroStore<ClonableTestClass> t2 = t1;
        EXPECT_EQ((size_t)6, t2.size());

        std::stringstream ss;                           /// Ebbe stringstream-be írunk
        std::for_each(t2.begin(), t2.end(), TestClassPrint(ss));    /// Funktor létrehozása a paraméterlistán
        EXPECT_STREQ("1;2;Hello;World;3;Bye;", ss.str().c_str()) <<
            "Valami baj van! Nem jo a masolat!" << std::endl;

        /// Csak a pointereket másolta le, vagy az objektumokat is ?
        ClonableHeteroStore<ClonableTestClass>::const_iterator it1, it2;
        for (it1 = t1.begin(), it2 = t2.begin(); it1 != t1.end(); ++it1,++it2)
            EXPECT_NE(*it1, *it2) << "Lehet, hogy nem deep copy-t csinalt?" << std::endl;
    } ENDM

    /// Megnézzük, hogy jól működik-e a az értékadás.
    /// Ezt listával próbáljuk.
    TEST(Test5, Assign) {
        ClonableHeteroStore<ClonableTestClass, std::list<ClonableTestClass*> > t1;
        t1.add(new ClonableTestClass1(1));
        t1.add(new ClonableTestClass1(2));
        t1.add(new ClonableTestClass2("Hello"));
        t1.add(new ClonableTestClass2("World"));
        t1.add(new ClonableTestClass1(3));
        t1.add(new ClonableTestClass2("Bye"));

        ClonableHeteroStore<ClonableTestClass, std::list<ClonableTestClass*> > t2;
        t2 = t1 = t1;
        EXPECT_EQ((size_t)6, t2.size());

        std::stringstream ss;
        TestClassPrint print(ss);                       /// stringstrem-be írunk
        std::for_each(t2.begin(), t2.end(), print);
        EXPECT_STREQ("1;2;Hello;World;3;Bye;", ss.str().c_str()) <<
            "Valami baj van! Nem jo a masolat!" << std::endl;

        /// Csak a pointereket másolta le, vagy az objektumokat is ?
        ClonableHeteroStore<ClonableTestClass, std::list<ClonableTestClass*> >::const_iterator it1, it2;
        for (it1 = t1.begin(), it2 = t2.begin(); it1 != t1.end(); ++it1,++it2)
            EXPECT_NE(*it1, *it2) << "Lehet, hogy nem deep copy-t csinalt?" << std::endl;
    } ENDM

#ifdef BACI_IS_VAN
    /// ClonableBacterium osztály működésének vizsgálata
    /// Itt még nem klónoz
    TEST(Test6, bacterium) {
        ClonableHeteroStore<ClonableBacterium> petri;
        petri.add(new ClonableSalmonella("bongori"));
        petri.add(new ClonableStreptococcus('A'));

        EXPECT_EQ((size_t)2, petri.size());

        std::stringstream ss;
        BacteriumShow show(ss);                         /// stringstrem-be írunk
        std::for_each(petri.begin(), petri.end(), show);
        EXPECT_STREQ("Bakterium Salmonella Fajta: bongori\n"
                      "Bakterium Streptococcus Csoport: A\n", ss.str().c_str()) <<
            "Valami baj van! Szeletelodes? Virt. alaposztaly konstruktora? " << std::endl;

        petri.add(new ClonableSalmonella("enterica"));
        petri.add(new ClonableStreptococcus('B'));
        std::cout << "petri:\n";
        std::for_each(petri.begin(), petri.end(), BacteriumShow());     /// képernyőre írunk
    } ENDM

    /// Megnézzük, hogy jól működik-e a másoló ctor.
    TEST(Test7, bacterium_copy) {
        ClonableHeteroStore<ClonableBacterium> petri;
        petri.add(new ClonableSalmonella("bongori"));
        petri.add(new ClonableStreptococcus('A'));
        petri.add(new ClonableSalmonella("enterica"));
        petri.add(new ClonableStreptococcus('B'));

        /// Klónozás jön (másoló ctor)
        ClonableHeteroStore<ClonableBacterium> agar = petri;
        EXPECT_EQ((size_t)4, agar.size());

        std::stringstream ss;
        std::for_each(agar.begin(), agar.end(), BacteriumShow(ss));     /// Funktor létrehozása a paraméterlistán
        EXPECT_STREQ(
            "Bakterium Salmonella Fajta: bongori\n"
            "Bakterium Streptococcus Csoport: A\n"
            "Bakterium Salmonella Fajta: enterica\n"
            "Bakterium Streptococcus Csoport: B\n",
            ss.str().c_str()) << "Valami baj van! Nem jo a masolat!" << std::endl;

        ClonableHeteroStore<ClonableBacterium>::const_iterator it1, it2;
        for (it1 = petri.begin(), it2 = agar.begin(); it1 != petri.end(); ++it1,++it2)
            EXPECT_NE(*it1, *it2) << "Lehet, hogy nem deep copy-t csinalt?" << std::endl;
        std::cout << "copy:\n";
        std::for_each(agar.begin(), agar.end(), BacteriumShow());       /// képernyőre írunk
    } ENDM

    TEST(Test8, bacterium_assign) {
        ClonableHeteroStore<ClonableBacterium> petri;
        petri.add(new ClonableSalmonella("bongori"));
        petri.add(new ClonableStreptococcus('A'));
        petri.add(new ClonableSalmonella("enterica"));
        petri.add(new ClonableStreptococcus('B'));

        /// Klónozás jön (többszöros értékadás)
        ClonableHeteroStore<ClonableBacterium> agar;
        agar = agar = petri;
        EXPECT_EQ((size_t)4, agar.size());

        std::stringstream ss;
        BacteriumShow show(ss);
        std::for_each(agar.begin(), agar.end(), show);
        EXPECT_STREQ(
            "Bakterium Salmonella Fajta: bongori\n"
            "Bakterium Streptococcus Csoport: A\n"
            "Bakterium Salmonella Fajta: enterica\n"
            "Bakterium Streptococcus Csoport: B\n",
            ss.str().c_str()) << "Valami baj van! Nem jo a masolat!" << std::endl;

        ClonableHeteroStore<ClonableBacterium>::const_iterator it1, it2;
        for (it1 = petri.begin(), it2 = agar.begin(); it1 != petri.end(); ++it1,++it2)
            EXPECT_NE(*it1, *it2) << "Lehet, hogy nem deep copy-t csinalt?" << std::endl;
        std::cout << "assign:\n";
        std::for_each(agar.begin(), agar.end(), BacteriumShow());
    } ENDM

#endif

/// Itt a vége
    GTEND(std::cerr);       // Csak C(J)PORTA működéséhez kell
    return 0;
}
