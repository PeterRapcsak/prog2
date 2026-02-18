// Fájl: nagyobb_main.cpp
// Egyszerű demo program.
// Bekér két egész számot és kiírja a nagyobbat.

#include <iostream>
#include <iomanip>
#include "fuggvenyeim.h"
#include <cmath>
using namespace std;

double polinom(double x, double egyutthatok[], int fokszam){
    int count = 0;
    double eredmeny = 0;
    
     for (int i = fokszam - 1; i >= 0; i--){
          eredmeny += egyutthatok[count] * pow(x, i);
          count++;
     }


    return eredmeny;
}



int main() {

     double lista[] = {3,2,1};
     cout << "polinom(x=1)(x^2 + 2x + 3) = " << polinom(1, lista, 3) << endl;



     cout << "Kerek ket egesz szamot:";

     int i, j;
     cin >> i >> j;                    // i és j értékének beolvasása
     int k  = max(i,  j);
     cout << "A nagyobb: " << k  << endl; // nagyobb kiírása
}
