#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include "memtrace.h"

const char *str(std::stringstream& ss) {
        return ss.str().c_str();
}

void bug1() {
    std::stringstream ss;
    ss << "Hello";
    std::cout << ss.str() << std::endl;
    std::cout << str(ss) << std::endl;
}

void feltolt(std::vector<const char*>& v, std::string s) {
    v.push_back(s.c_str());
}

void kiir(std::vector<const char*>& v) {
    for (size_t i = 0; i < v.size(); i++) {
        std::cout << v[i] << std::endl;
    }
}

void bug2() {
    std::vector<const char*> v;
    feltolt(v, "Hello");
    feltolt(v, "World");
    kiir(v);
}


struct Mat {
     int **p;
     int siz;
     Mat(int n = 0) :siz(n) {
        p = new int*[siz];
        for (int i = 0; i < siz; i++)
            p[i] = new int[siz];
     }
     size_t size() const { return siz; }
     ~Mat() {
        for (int i = 0; i < siz; i++)
            delete[] p[i];
        delete[] *p;
     }
};

void bug3() {
    Mat m;
    std::cout << "size:" << m.size() << std::endl;
}


void prmenu() {
#ifdef MEMTRACE
    std::cout << "\n\tMEMTRACE bekapcsolva\n";
#else
    std::cout << "\n\tMEMTRACE nincs bekapcsolva\n";
#endif
    std::cout << "Melyik teszteset legynen? (1,2,3): ";
}

int main() {
    int i;
    prmenu();
    while (std::cin >> i) {
        switch (i) {
          case 1:
            std::cout << "TEST1\n";
            bug1();
          break;

          case 2:
            std::cout << "TEST2\n";
            bug2();
          break;

          case 3:
            std::cout << "TEST3\n";
            bug3();
          break;
        }
        prmenu();
    }
}
