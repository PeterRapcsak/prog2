#include <cstring>
#include <cctype>
#include "nem_oo.h"

/**
 * \file nem_oo_teszt.cpp
 * (UTF-8 kodolasu fajl. Allitsa at a kodolast,
 *  ha a tovabbi kommentek nem olvashatok helyesen!)
 *
 * FELADATOK:
 *  1. Ebben a fájlban valósítsa meg a nem_oo.h állományban deklarált függvényeket!
 *
 * Ügyeljen a helyes névtér használatra!
 *   Tipp: Használja a scope operátort, vagy nyisson névteret!
 */

namespace sajat {

// stringbol szamot csinal, base szamrendszerben
int atoi(const char *p, int base) {
    int result = 0;

    // vegigmegyunk a karaktereken amig nem whitespace vagy vege
    while (*p != '\0' && !std::isspace(*p)) {
        int digit;

        // szamjegy 0-9
        if (*p >= '0' && *p <= '9') {
            digit = *p - '0';

        // nagybetu A-Z
        } else if (*p >= 'A' && *p <= 'Z') {
            digit = *p - 'A' + 10;

        // kisbetu a-z
        } else if (*p >= 'a' && *p <= 'z') {
            digit = *p - 'a' + 10;
        } else {
            // ervenytelen karakter
            throw "T0R2E4";
        }

        // ha a szamjegy tul nagy a szamrendszerhez
        if (digit >= base) {
            throw "T0R2E4";
        }

        // horner: result = result * alap + uj szamjegy
        result = result * base + digit;
        p++;
    }
    return result;
}

// ket stringet osszefuz, dinamikusan foglal memoriat
char *strcat(const char *p1, const char *p2) {
    size_t len1 = std::strlen(p1);
    size_t len2 = std::strlen(p2);

    // uj memoria a ket string + lezaro nulla
    char *result = new char[len1 + len2 + 1];
    std::strcpy(result, p1);       // elso string masolasa
    std::strcpy(result + len1, p2); // masodik hozzafuzese
    return result;
}

// egymas utani ismetlodo karaktereket kiszedi
// helyben modosit, visszaadja az uj veg utani pointert
char *unique(char *first, char *last) { 
    // ures tartomany
    if (first == last) {
        return last;
    }
    char *result = first;
    // vegigmegyunk az elemeken
    while (++first != last) {
        // ha kulonbozik az elozotol, megtartjuk
        if (*result != *first) {
            *(++result) = *first;
        }
    }
    // uj veg utani pointer
    return ++result;
}

}