/**
 *  \file alakzat2_main.cpp
 *  Demó program az alakzat és rajztábla osztályok kipróbálására.
 *  Felrajzol pár alakzatot, majd elmozgatja kicsit.
 *
 *  ELKESZULT makró értékei:
 *  <= 2 - ld. alakzat_main.cpp. Ebben a főprogramban nincs hozzá kód.
 *  >= 3 - van már Rajztabla osztály, létrehozható, meszüntethető (ez az alapeset)
 *  >= 4 - törlés kivételével minden Rajztabla funkciót kipróbálunk
 *  >= 5 - törlést is próbáljuk
 *  >= 6 - globális rajztabla insertert is kipróbáljuk
 *  >= 7 - szorgalmi: téglalap osztály is van
 */

#define ELKESZULT 6

#include <iostream>
#include "szakasz.h"
#include "kor.h"

#if ELKESZULT >= 1
# include "haromszog.h"
#endif // ELKESZULT

#if ELKESZULT >= 2
# include "ellipszis.h"
#endif // ELKESZULT

#if ELKESZULT >= 3
# include "rajztabla.h"
#endif // ELKESZULT

#if ELKESZULT >= 7
#include "teglalap.h"
#endif // ELKESZULT

using std::cout;
using std::endl;

int main() {

#if ELKESZULT >= 3      // van rajztábla
    Rajztabla tabla;
    cout << "tabla.size: " << tabla.size() << endl;
#endif // ELKESZULT

#if ELKESZULT >= 4      // van rajztábla és ráteszünk alakzatokat
    tabla.felrak(new Szakasz(Pont(20,40), 100, 0, WHITE));
    cout << "tabla.size: " << tabla.size() << endl;
    tabla.felrak(new Kor(Pont(120,140), 50, BLUE));
    cout << "tabla.size: " << tabla.size() << endl;
    tabla.felrak(new Haromszog(Pont(130,150), Pont(230, 150),  Pont(280, 200),  RED));
    cout << "tabla.size: " << tabla.size() << endl;
    tabla.felrak(new Ellipszis(Pont(100,100), 20, 0.5, GREEN));
    cout << "tabla.size: " << tabla.size() << endl;
#endif // ELKESZULT

#if ELKESZULT >= 7      // szorgalmi: téglalap
    tabla.felrak(new Teglalap(Pont(20,40), Pont(60, 100), GREEN));
    cout << "tabla.size: " << tabla.size() << endl;
#endif // ELKESZULT

#if ELKESZULT >= 4      // van rajztábla és kiírjuk mi van rajta.
    cout << "Rajztabla rajzol:" << endl;
    tabla.rajzol();
    Pont delta(100, -10);
    cout << endl << "Rajztabla mozgat delta=" << delta << ":" << endl;
    tabla.mozgat(delta);
#endif // ELKESZULT

#if ELKESZULT >= 6      // Van iserter
    cout << "\nRajztabla inserter:" << endl;
    cout << tabla << endl;
#endif // ELKESZULT

#if ELKESZULT >= 5      // a törlés is megy?
    cout << "\nRajztabla torol:" << endl;
    tabla.torol();
    cout << "tabla.size: " << tabla.size() << endl;
    tabla.felrak(new Kor(Pont(10,20), 50, RED));
    cout << "tabla.size: " << tabla.size() << endl;
    cout << endl << "Rajztabla rajzol:" << endl;
    tabla.rajzol();
#endif // ELKESZULT

#if ELKESZULT < 5
    cout << "Legalabb ELKESZULT == 5-ig oldja meg a feladatokat!\n";
#endif // ELKESZULT

    return 0;
}
