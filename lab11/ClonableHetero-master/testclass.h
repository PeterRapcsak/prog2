/**
 *  \file: testclass.h
 *
 *  TestClass alaposztály és leszármazottjai
 */

#ifndef TESTCLASS_H
#define TESTCLASS_H

#include <string>

/// TestClass osztály.
struct TestClass {
    virtual void print(std::ostream&) const {} ;
    virtual ~TestClass() {}
};

/// TestClass1 osztály.
/// TestClass virtuális alaposztályból származik
/// a többszörös öröklési úton való elérésből adódó problémák kiküszöbölése miatt.
class TestClass1 : virtual public TestClass {
    int adat;
public:
    TestClass1(int a = 0) : adat(a) { }
    void print(std::ostream& os) const {
        os << adat << ';';
    }
};

/// TestClass2 osztály.
/// TestClass virtuális alaposztályból származik
/// a többszörös öröklési úton való elérésből adódó problémák kiküszöbölése miatt.
class TestClass2 : virtual public TestClass {
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
