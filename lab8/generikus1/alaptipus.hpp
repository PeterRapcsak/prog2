/*
 * \file alaptipus.hpp
 */

#ifndef ALAPTIPUS_HPP
#define ALAPTIPUS_HPP


/// Alaptipus osztaly.
/// A konverzios operatorok miatt az osztaly peldanyai ugy hasznalhatok,
/// mint az eredeti alaptipus valtozoi.
template <typename T>
class Alaptipus {
    T adat;
public:
    Alaptipus(T i = 0) : adat(i) {}      /// konstruktor es egyben konverzios operator
    operator T&() { return adat; }       /// cast
    operator T() const { return adat; }  /// const cast
};

#endif // ALAPTIPUS_HPP