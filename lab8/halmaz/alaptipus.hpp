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
    Alaptipus(T i = 0) : adat(i) {} // 0 alapérték
    operator T&() { return adat; }
    operator T() const { return adat; }
};

#endif // ALAPTIPUS_HPP
