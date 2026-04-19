/**
 *  \file alakzat_main.cpp
 *  Demo program az alakzat osztályok kipróbálására.
 *  Felrajzol pár alakzatot, majd elmozgatja kicsit.
 *
 *  ELKESZULT makró értékei:
 *  <= 0 - alap, nincs újabb osztály kész
 *  >= 1 - elkészült a háromszög osztály
 *  >= 2 - ellipszis osztályt is kipróbáljuk
 */

#define ELKESZULT 0

#include <iostream>
#include "szakasz.h"
#include "kor.h"

#if ELKESZULT >= 1
# include "haromszog.h"
#endif // ELKESZULT

#if ELKESZULT >= 2
# include "ellipszis.h"
#endif // ELKESZULT

using std::cout;
using std::endl;

int main() {

    const int N = 100;              // maximum 100 alakzatunk lehet
    Alakzat *idom[N];
    int db = 0;                     // most nincs egy se

    /// Létrehozunk egy szakaszt egy pontból és dx, dy távolságokból
    std::cout << "\n";
    Szakasz sz1(Pont(20,40), 100, 0, WHITE);
    std::cout << sz1 << std::endl;  // kiírjuk az adatait
    sz1.rajzol();                   // "felrajzoljuk"
    idom[db++] = &sz1;			    // nem kell cast, mert kompatibilis

    /// Létrehozunk egy kört a középpontból és a sugarából
    std::cout << "\n";
    Kor k1(Pont(120,140), 50, BLUE);
    std::cout << k1 << std::endl;   // kiírjuk az adatait
    k1.rajzol();                    // "felrajzoljuk"
    idom[db++] = &k1;

#if ELKESZULT >= 1
    /// Létrehozunk egy háromszöget három pontból
    std::cout << "\n";
    Haromszog h1(Pont(130,150), Pont(230, 150),  Pont(280, 200),  RED);
    std::cout << h1 << std::endl;   // kiírjuk az adatait
    h1.rajzol();                    // "felrajzoljuk"
    idom[db++] = &h1;
#endif // ELKESZULT

#if ELKESZULT >= 2
    /// Létrehozunk egy ellipszist középpontból, sugárból és excentricitásból
    std::cout << "\n";
    Ellipszis e1(Pont(100,100), 20, 0.5, GREEN);
    std::cout << e1 << std::endl;   // kiírjuk az adatait
    e1.rajzol();                    // "felrajzoljuk"
    idom[db++] = &e1;
#endif // ELKESZULT

    cout << "\nMost kirajzoljuk alaposztaly pointerrel:" << endl;
    for (int i = 0; i < db; i++)  // végigmegyünk az összes alakzaton
        idom[i]->rajzol();        // kirajzoljuk

    cout << "\nMost mozgatunk (x+100, y+100) (pointerrel):" << endl;
    for (int i = 0; i < db; i++)  // az összes alakzatra
        idom[i]->mozgat(Pont(100, 100));  // kicsit mozdítunk rajta

    return 0;
}
