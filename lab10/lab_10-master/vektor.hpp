/**
 * \file vektor.hpp
 *
 * Előadáson bemutatott generikus tömbből Vektort készítünk.
 *
 */
#ifndef GEN_VEKTOR
#define GEN_VEKTOR

#include <stdexcept>
#include "gen_array_iter3.hpp"

template<typename T, size_t maxsiz = 6>
class Vektor : public Array<T, maxsiz> {
public:
    /// minden tagfüggvény jó, nem kell átdefiniálni
    /// csak a konstruktorokat kell megfelelően paraméterezve továbbhívni

	/// default és konstans értékkel feltöltő konstruktor
	/// @param n - méret
	/// @param value - érték, amivel feltölt
	explicit Vektor(size_t n = 0, const T& value = T())
    : Array<T, maxsiz>(n, value) {}      // alaposztály konstruktora

/// ----------------------------------------------------
/// A további tagfüggvényeket Önnek kell megvalósítani
///-----------------------------------------------------
    /// konstruktor sorozatból
    /// @param first - sorozat elejére mutat
    /// @param last - utolsó elem után
    template <class InputIterator>
    Vektor(InputIterator first, InputIterator last)
    : Array<T, maxsiz>(first, last) {}

    /// push_back
    /// vektor végéhez adatot ad
    /// @param val - adat
    void push_back(const T& val) {
        this->at(this->size()) = val;
    }

    /// back
    /// vektor utolsó adatának elérése
    /// @return referencia az utolsó adat
    T& back() {
        if (this->empty()) throw std::out_of_range("Vektor.back(): ures vektor");
        
        size_t idx = this->size() - 1;
        return this->at(idx);
    }

    /// pop_back
    /// vektor utolsó adatának eldobása
    void pop_back() {
        if (this->empty()) throw std::out_of_range("Vektor.pop_back(): ures vektor");
        
        size_t sz = this->size();
        this->setsize(sz - 1);
    }

    /// empty
    /// @return true, ha nincs adat
    bool empty() const {
        return this->size() == 0;
    }

    /// A továbbiakban egyszerűbben hivatkozhassunk az iterator-ra, mint típusra
    typedef typename Array<T, maxsiz>::iterator iterator;

    /// törli az adott pozíción levő elemet
    /// @param pos - a törlendő elemre mutató iterátor
	/// @return az első nem törölt elemre mutató iterátor, ha végéig törölt, akkor end()
    iterator erase(iterator pos) {
        return erase(pos, pos+1);
    }

    /// törli az adott intervallumba eső elemeket
    /// @param first - a törlendő intervalum eleje
    /// @param last - a törlendő intervallum vége
	/// @return az első nem törölt elemre mutató iterátor, ha végéig törölt, akkor end()
    iterator erase(iterator first, iterator last) {
        // Nem implementáljuk, mert az ELKESZULT >= 5 szinthez nem kell
        throw std::runtime_error("Vektor.erase(): nem implementalt");
    }

 }; // Vektor sablon vége

#endif
