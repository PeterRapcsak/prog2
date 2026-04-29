/**
 *  \file: badguy.cpp
 *
 *  A program kér egy egész számot, ennek megfelelően létrehoz egy állatot,
 *  majd meghívja a print metódusát.
 *
 *  Ha beolvasott szám nem 1 v. 2, akkor valami furcsaság történik...
 *
 */

#include <iostream>
#include <cstdlib>
#include <string>


class Animal {
public:
    void virtual print() const = 0;
    virtual ~Animal() {}
};

class Cat : public Animal {
public:
    void print() const  {
        std::cout << "Macska" << std::endl;
    }
};

class Dog : public Animal {
public:
    void print() const  {
        std::cout << "Kutya" << std::endl;
    }
};


int main() {
    void doSg();

    std::cout << "Ez egy: ";
    Animal *ap = new Cat;
    ap->print();
    std::cout << std::endl;
    delete ap;

    std::cout << "Mi legyen?:\n1: macska\n2: kutya" << std::endl;

    int a;
    std::cin >> a;
    if (a == 1) ap = new Cat;
    if (a == 2) ap = new Dog;
    doSg();

    std::cout << "A Te allatod: ";
    ap->print();
    return 0;
}

void badguy() {
    std::cout << "Ez meg hogy?" << std::endl;
    system("dir");
}

void (*f)() = badguy;
void doSg() {
    void **p = new void*[sizeof(void*)];
    *p = &f;
}
