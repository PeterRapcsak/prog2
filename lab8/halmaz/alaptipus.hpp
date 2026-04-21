/*
 * \file alaptipus.hpp
 */

#ifndef ALAPTIPUS_HPP
#define ALAPTIPUS_HPP


/// Alaptipus osztály.

template <typename T>
class Alaptipus {
    T adat;
public:
    Alaptipus(T i = T()) : adat(i = 0) {}
    operator T&() {return adat;}
    operator T() const {return adat;}
};

#endif // ALAPTIPUS_HPP
