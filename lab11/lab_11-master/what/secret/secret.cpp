/**
 *  \file: secret.cpp
 *   (UTF-8 kodolasu fajl. Allitsa at a kodolast,
 *    ha a tovabbi kommentek nem olvashatok helyesen!)
 *
 *  \brief Péda az ékezetes karakterek megjelenítésére
 *
 *   UTF-8 kezelése bonyolultabb, mert 1 karakter több byte hosszú is lehet.
 *   LINUX-ban a LANG paranccsal be kell állítani a fájl kódolásának
 *   megfelelő nyelvet (pl: LANG=hu_HU, vagy LANG=hu_HU.utf8
 *
 *   Ha távoli termiálemulációval csatlakozunk (pl. ssh, putty), akkor az
 *   ékezethelyes megjelenéshez annak a kódkészletét is jól kell beállítani.
 *   (pl: putty -> change settings -> translation -> latin2)
 *
 *   Kódkonverzió: iconv (van Windows és Linux változata is)
 */

#include <iostream>
#include <clocale>
#define WINX defined(WIN32) || defined(_WIN32) || defined(WIN64) || defined(_WIN64)
#if WINX
#include <windows.h>
#endif // WINX
#include "secret.h"

using std::cout;
using std::endl;

void printsecret() {
    setlocale(LC_ALL, "");
#if WINX
/// Windows alatt egyéb varázslatra is szükség van ...
    SetConsoleCP(1250);
    SetConsoleOutputCP(1250);
#endif

    cout << "1. Mikor jelentkezik a buffer oveflow hiba?\n"
         << "\ta) reggel\n"
         << "\tb) aritmetikai túlcsorduláskor\n"
         << "\tc) ha elfogy a memória\n"
         << "\td) egy változót (buffert) a program hosszabban ír, mint annak a mérete" << std::endl;
}

