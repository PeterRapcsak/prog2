/**
 * \file generikus1_teszt.cpp
 *
 * Egyszerű példa osztálysablonra és függvénysablonra.
 *
 */

/**
 * ELKESZULT == 0 estén az előre elkészített Integer osztályt próbálja ki pár esetre.
 * ELKESZULT == 1 estén az Ön által elkészítendő Alaptípus osztálysablont próbálja ki.
 * ELKESZULT >= 2 esetén pedig egy egyszerű függvénysablont kell elkészítenie.
*/
#define ELKESZULT 0

#include <iostream>
#include "integer.h"

#if ELKESZULT > 0
# include "alaptipus.hpp"
#endif // ELKESZULT > 0

/// Korábban használt makró, ami kiírja a paramétereit, majd változtatás nélkül kigenerálja azokat.
/// A kiírást egy tabulátorral kezdi és soremeléssel zárja.
/// Használatával elérhető, hogy a paraméterként adott utasítások, deklarációk a végrehajtás előtt
/// kiíródjanak a képernyőre. Így hatásuk jól megfigyelhető.
#define _(...)  std::cout << "\t" << #__VA_ARGS__ << std::endl; __VA_ARGS__


/// Ebből a függvényből kell sablont készíteni ELKESZULT >= 2 esetén (l. útmutató)
void kiir(Integer* first, Integer* last, std::ostream& os = std::cout) {
    while (first != last)
        os << *first++ << ' ';
    os << std::endl;
}

int main() {
#if ELKESZULT >= 0
{ // blokk kezdete így a nevek lokálisak maradnak
    std::cout << "\nELKESZULT >= 0\n";

    /// Csak az Integer osztályt mutatja be.
    /// Értse meg az osztály működését és használatát!
_(  Integer a;                          )
_(  a = 10;                             )

    /// Szedje szét a kifejezést operátoronként külön sorba és
    /// a debug funkcióval kövesse a végrehajtást!
_(  Integer b = a * 2 + 1 * a + 1;      )

    /// Néhány további művelet
_(  std::cout << b / 10 << std::endl;   )

    /// Címképzés is megy:
_(  int *p = &a;                        )
_(  *p = 13;                            )
_(  std::cout << a << std::endl;        )

    /// Konstanssal is megy?:
_(  const Integer a0;                   )
_(  const int *p0 = &a0;                )
_(  std::cout << *p0 << std::endl;      )

    /// Akkor tömböt is tudunk létrehozni?
_(  Integer t[] = { 1, 2, 4, 8, 10} ;   )

_( kiir(t, t+5);                        )
}  // blokk vége
#endif // ELKESZULT >= 0


#if ELKESZULT >= 1
{ // blokk kezdete így a nevek lokálisak maradnak
    std::cout << "\nELKESZULT >= 1\n";

    /// Az Alaptípus sablont példányosítjuk double típusra.
_(  Alaptipus<double> a;                          )
_(  a = 10;                                       )
_(  Alaptipus<double> b = a * 2 + 1 * a + 1;      )
    /// Néhány további művelet
_(  std::cout << b / 10 << std::endl;             )

_(  Alaptipus<double> t[] = { 1.1, 2.2, 4.4, 8.8};)

#if ELKESZULT >= 2
    std::cout << "\nELKESZULT >= 2\n";

_(  kiir(t, t+4);                                 )
#endif // ELKESZULT >= 2
} // blokk vége
#endif // ELKESZULT >= 1

    return 0;
}

