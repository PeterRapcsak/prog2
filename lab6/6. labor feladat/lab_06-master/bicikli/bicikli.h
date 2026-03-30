#ifndef BICIKLI_H
#define BICIKLI_H

#include <iostream>

using namespace std;

class Jarmu {
    double vMax;    // maximalis sebesseg
public:
    // Konstruktor
    Jarmu(double v = 0) : vMax(v) {
        cout << "\tJarmu ctor vMax=" << vMax << endl;
    }

    // Masolo konstruktor
    Jarmu(const Jarmu& j) : vMax(j.vMax) {
        cout << "\tJarmu copy vMax=" << vMax << endl;
    }

    // Virtualis destruktor
    virtual ~Jarmu() {
        cout << "\tJarmu dtor vMax=" << vMax << endl;
    }
};

class Kerek {
    int atmero;     // kerek atmeroje
public:
    // Konstruktor
    Kerek(int a = 0) : atmero(a) {
        cout << "\tKerek ctor" << endl;
    }

    // Masolo konstruktor
    Kerek(const Kerek& k) : atmero(k.atmero) {
        cout << "\tKerek copy" << endl;
    }

    // Virtualis destruktor
    virtual ~Kerek() {
        cout << "\tKerek dtor" << endl;
    }

    // Atmero kiirasa
    void kiir() {
        cout << "atmero=" << atmero << endl;
    }
};

class Szan : public Jarmu {
    int kutyakSzama;    // kutyak szama
public:
    // Konstruktor
    // v - maximalis sebesseg
    // n - kutyak szama
    Szan(double v = 0, int n = 0) : Jarmu(v), kutyakSzama(n) {
        cout << "\tSzan ctor kutyakSzama=" << kutyakSzama << endl;
    }

    // Masolo konstruktor
    Szan(const Szan& s) : Jarmu(s), kutyakSzama(s.kutyakSzama) {
        cout << "\tSzan copy kutyakSzama=" << kutyakSzama << endl;
    }

    // Destruktor
    ~Szan() {
        cout << "\tSzan dtor kutyakSzama=" << kutyakSzama << endl;
    }
};

class Bicikli : public Jarmu {
    Kerek elso;
    Kerek hatso;
public:

    // Konstruktor
    // v - maximalis sebesseg
    // a - kerekek atmeroje
    Bicikli(double v = 0, int a = 0) : Jarmu(v), elso(a), hatso(a) {
        cout << "\tBicikli ctor ";
        elso.kiir();
    }

    // Masolo konstruktor
    // b - masolando Bicikli
    Bicikli(const Bicikli& b) : Jarmu(b), elso(b.elso), hatso(b.hatso) {
        cout << "\tBicikli copy ";
        elso.kiir();
    }

    // Destruktor
    ~Bicikli() {
        cout << "\tBicikli dtor ";
        elso.kiir();
    }
};

#endif
