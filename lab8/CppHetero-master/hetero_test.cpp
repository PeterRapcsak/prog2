#include <iostream>
#include <sstream>

#include "memtrace/gtest_lite.h"
#include "memtrace/memtrace.h"
#include "hetero_store.hpp"
#include "testclass.h"
#include "bacterium.h"

/// Csak az ellenőrzéshez
struct MyException : std::exception {
    MyException(...) {}
};

int main() {
    GTINIT(std::cin);       // Csak C(J)PORTA működéséhez kell

    /// Csak a méreteket ellenőrizzük
    TEST(Test1, sanity) {
        HeteroStore<TestClass, 2> t1;
        EXPECT_EQ((size_t)0, t1.size()) << "Nem ures?" << std::endl;
        EXPECT_EQ((size_t)2, t1.capacity()) <<  "Nem jo kapacitas erteke!" << std::endl;
        EXPECT_NO_THROW(t1.add(NULL)) << "Nem jo a meret!" << std::endl;
        EXPECT_EQ((size_t)1, t1.size());
        EXPECT_EQ((size_t)2, t1.capacity()) << "Megvaltozott a kapacitas?" << std::endl;
        EXPECT_NO_THROW(t1.add(NULL));
        EXPECT_EQ((size_t)2, t1.size())  << "Nem jo a meret!" << std::endl;
        EXPECT_EQ((size_t)2, t1.capacity()) << "Megvaltozott a kapacitas?" << std::endl;
        EXPECT_THROW(t1.add(NULL), std::out_of_range);

        t1.clear();
        EXPECT_EQ((size_t)0, t1.size()) << "Nem torlodott?" << std::endl;
        EXPECT_EQ((size_t)2, t1.capacity()) << "Megvaltozott a kapacitas?" << std::endl;
    } ENDM

   /// Méreteket ellenőrizzük és azt, hogy törli-e az objektumokat
   TEST(Test2, obj) {
        HeteroStore<TestClass, 1, MyException> t1;
        EXPECT_EQ((size_t)0, t1.size());
        EXPECT_EQ((size_t)1, t1.capacity());
        EXPECT_NO_THROW(t1.add(new TestClass2));
        EXPECT_EQ((size_t)1, t1.size());
        EXPECT_EQ((size_t)1, t1.capacity());
    } ENDMsg("Megszuneskor felszabaditotta a tarolora bizott objektumokat?")

    /// Ellenőrizzuk a kivételt valós objekummal
    TEST(Test3, exception) {
        HeteroStore<TestClass, 1, MyException> t1;
        EXPECT_EQ((size_t)0, t1.size());
        EXPECT_EQ((size_t)1, t1.capacity());
        EXPECT_NO_THROW(t1.add(new TestClass2));
        EXPECT_EQ((size_t)1, t1.size());
        EXPECT_EQ((size_t)1, t1.capacity());
        EXPECT_THROW(t1.add(new TestClass2), MyException);
    } ENDMsg("Torolte azt az objektumot, ami mar nem fert bele?")

    /// Traverse tesztelése és megnézzük, hogy van-e esetleg szeletelődés
    TEST(Test4, heterogen) {
        HeteroStore<TestClass> t1;
        EXPECT_EQ((size_t)100, t1.capacity()) <<  "Nem jo default kapacitas erteke!" << std::endl;
        t1.add(new TestClass1(1));
        t1.add(new TestClass1(2));
        t1.add(new TestClass2("Hello"));
        t1.add(new TestClass2("World"));
        t1.add(new TestClass1(3));
        t1.add(new TestClass2("Bye"));
        std::stringstream ss;

        TestClassPrint print(ss);
        t1.traverse(print);
        EXPECT_TRUE(0 != ss.str().size()) << "Nem irt ki semmit! Traverse mukodik?" << std::endl;
        if (ss.str().size() != 0)
            EXPECT_STREQ("1;2;Hello;World;3;Bye;", ss.str().c_str()) <<
                                                    "Valami baj van! Szeletelodes?" << std::endl;
    } ENDM

    /// Itt egy másik alaposztályból származóaknak hozunk létre tárolót
    TEST(Test5, bacterium) {
        HeteroStore<Bacterium> petri;
        petri.add(new Salmonella("bongori"));
        petri.add(new Salmonella("enterica"));
        petri.add(new Streptococcus('A'));
        petri.add(new Streptococcus('B'));

        EXPECT_EQ((size_t)4, petri.size());
        BacteriumShow show;
        petri.traverse(show);
    } ENDM

    GTEND(std::cerr);       // Csak C(J)PORTA működéséhez kell
    return 0;
}
