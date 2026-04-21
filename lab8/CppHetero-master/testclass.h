/**
 *  \file: bacterium.h
 *
 *  TestClass alaposztály és leszármazottjai
 */

#ifndef TESTCLASS_H
#define TESTCLASS_H

#include <string>

/// TestClass osztály.
struct TestClass {
    virtual void print(std::ostream& os) const {} ;
    virtual ~TestClass() {}
};

/// TestClass1 osztály.
class TestClass1 : public TestClass {
    int adat;
public:
    TestClass1(int a = 0) : adat(a) { }
    void print(std::ostream& os) const {
        os << adat << ';';
    }
};

/// TestClass2 osztály.
class TestClass2 : public TestClass {
    std::string str;
public:
    TestClass2(std::string s = "") : str(s) {}
    void print(std::ostream& os) const {
        os << str << ';';
    }
};

/// Funktor a kiíráshoz
/// Konstruktor paraméterben megadott adatfolyamra ír
class TestClassPrint {
    std::ostream& os;
public:
    TestClassPrint(std::ostream& os = std::cout) :os(os) {}
    void operator()(TestClass *p) {
        p->print(os);
    }
};

#endif // TESTCLASS_H
