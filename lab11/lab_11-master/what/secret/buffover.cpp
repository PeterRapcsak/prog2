/**
 *  \file: buffover.cpp
 *   (UTF-8 kodolasu fajl. Allitsa at a kodolast,
 *    ha a tovabbi kommentek nem olvashatok helyesen!)
 *
 *  \brief Buffer overflow hiba bemutatása
 *
 *  A helyes jelszó: titok
 *  Próbálja ki helyes és helytelen jelszóval,
 *  majd 7-10 betű hosszúságú tetszőleges jelszóval is!
 *
 */

#include <iostream>
#include <cstring>
#include "secret.h"

using std::cin;
using std::cout;
using std::endl;

char buff[6];
bool pass = false;

int main(void)
{
    cout << "Jelszo: ";
    cin >> buff;

    if (strcmp(buff, "titok") != 0) {
        cout << "*** Hibas jelszo ***" << endl;
    } else {
        cout << "Helyes jelszo" << endl;
        pass = true;
    }

    if (pass) {
        cout << ">> Hozzaferhet a titokhoz: <<" << endl;
        printsecret();  // kiírja a titkos információt
    }
    return 0;
}
