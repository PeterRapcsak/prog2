/**
 * \file: clonable_testclass.h
 *
 */

#ifndef CLONABLE_TESTCLASS_H
#define CLONABLE_TESTCLASS_H

#include "testclass.h"
#include "clonable.h"


/// ClonableTestClass osztály.
/// TestClass-ból hozza a funkcionalitást,
/// a Clonable-ből pedig a klónozhatóságot.
struct ClonableTestClass : virtual TestClass, Clonable {
    ClonableTestClass *clone() const { return new ClonableTestClass(*this); }
};

/// ClonableTestClass1 osztály.
/// TestClass1-ből hozza a funkcionalitást,
/// a ClonableTestClass-ból pedig a klónozhatóságot, de ennek és a TestClass1-nek is
/// alaposztálya a TestClass, ami virtuális alap mindkettőben.
struct ClonableTestClass1 : TestClass1, ClonableTestClass {
    ClonableTestClass1(int a = 0) :
        // az öröklési a lánc végén meg kell hívni a virt. alaposztály konstrukorát
        TestClass(), // ez lenne a default, de kiírjuk, hogy látszódjon
        TestClass1(a) { }
    ClonableTestClass1 *clone() const { return new ClonableTestClass1(*this); }
};


/// ClonableTestClass2 osztály.
/// TestClass2-ből hozza a funkcionalitást,
/// a ClonableTestClass-ból pedig a klónozhatóságot, de ennek és a TestClass2-nek is
/// alaposztálya a TestClass, ami virtuális alap mindkettőben.
struct ClonableTestClass2 :  TestClass2, ClonableTestClass {
    ClonableTestClass2(std::string s = "") :
        // az öröklési a lánc végén meg kell hívni a virt. alaposztály konstrukorát
        TestClass(), // ez lenne a default, de kiírjuk, hogy látszódjon
        TestClass2(s) {}
    ClonableTestClass2 *clone() const { return new ClonableTestClass2(*this); }
};

#endif // CLONABLE_TESTCLASS_H
