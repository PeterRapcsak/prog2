/**
 *  \file kor.cpp
 *  Alakzat osztály tagfüggvényeinek megvalósítása
 */
#include "alakzat.h"

/// mozgat - eltolja az alakzat origóját
/// @param d - eltolás
void Alakzat::mozgat(const Pont& d) {
    Szin tsz = sz; 	/// tényleges rajzolási szín elmentése
    sz = BACKGND;   /// rajzolási szín legyen a háttér színe
    rajzol(); 		/// letörlés az eredeti helyről (rajzolás háttér színnel)
    p0 += d;  	    /// mozgatás: a pozíció változik
    sz = tsz;		/// rajzolási szín a tényleges szín
    rajzol(); 		/// felrajzolása az új pozícióra
}

/// Globális << operátor az Alakzat adatainak kiírásához
std::ostream& operator<<(std::ostream& os, const Alakzat& a) {
    return os << "p0=" << a.getp0() << ",szin=" << a.getsz();
}
