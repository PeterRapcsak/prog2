/**
 * \file diamond_test.cpp
 * Egyszerű program a rombusz (diamond) öröklés problémának bemutatására
 *
 */

#include "elkeszult.h"

/**
 * Feladatok:
 * 1. Értse meg a programot, majd szüntesse meg a rombusz öröklésből
 *    származó adattöbbszörözést!
 * 2. Az Iroda osztály az std::vector osztályt használja tárolásra.
 *    A tarolo_tipus módosításával ez könnyen lecserélhető pl. std::list,
 *    vagy std::deque osztályra. Miért?
 * 3. Cserélje le először listára, majd sorra!
 * 4. Készítse ez irodistak perzisztens változatát a pirodistak.h- fájlban
 *
 *
 */

/// Ellenőrizzük, hogy az elkeszult.h-ban definiálta-e az ELKESZULT makrót
#ifndef ELKESZULT
#define ELKESZULT 0     /// ha mégse definiálta
#endif

#if ELKESZULT < 0       /// ha hibásan definiálta
#undef ELKESZULT
#define ELKESZULT 0
#endif
/// ------- Ellenőrző rész vége ----------

#include <iostream>
#include "iroda.h"
#include "alkalmazott.h"
#include "irodistak.h"
#include "memtrace.h"
#include "gtest_lite.h"



using std::cin;
using std::cout;
using std::endl;

int main() {
  GTINIT(std::cin);       // Csak C(J)PORTA működéséhez kell
  try {
    Iroda munkahely;

    munkahely.felvesz(new Alkalmazott("Lusta Dick", 100));      // 1. obj létrehozása
    munkahely.felvesz(new HatIdeju("Grabovszki", 300, 1000));   // 2. obj
    munkahely.felvesz(new CsopVez("Mr. Gatto", 500, 1));        // 3. onj
    HatIdCsV *gore =                                    /// miért nem Alkalmazott* ?
                    new HatIdCsV("Mr. Tejfel", 600, 2, 1000);   // 4. obj
#if ELKESZULT == 0
    /// ha nincs virtualis öröklés
    munkahely.felvesz(dynamic_cast<CsopVez*>(gore));            // 5. obj.
    HatIdCsVezH *saf = new HatIdCsVezH("Safranek", 700, 1000, *gore);
    munkahely.felvesz(dynamic_cast<CsopVez*>(saf));            /// miért kell a cast ?
#else
    /// ha van virtualis öröklés
    munkahely.felvesz(gore);                                   /// miért nem kell már a cast ?
    munkahely.felvesz(new HatIdCsVezH("Safranek", 700, 1000, *gore));   // 5. obj.
#endif
    cout << endl;
    munkahely.kiir();
#if ELKESZULT >= 2
    void perzTest();
    perzTest();
#endif
  } catch (std::exception& e) {
    std::cout << "*** Kivetel: *** " << e.what() << std::endl;
  }
  // Itt a vége
  std::cout << "ELKESZULT = " << ELKESZULT << std::endl;
  if (ELKESZULT < 4 && !gtest_lite::test.fail())
    FAIL() << "\nLegalabb az elso negy feladatot oldja meg!" << std::endl;

  if (ELKESZULT >= 6 && !gtest_lite::test.fail())
    std::cout << "Szuper! Mind kesz" << std::endl;

  GTEND(std::cerr);       // Csak C(J)PORTA működéséhez kell
  return 0;
}
