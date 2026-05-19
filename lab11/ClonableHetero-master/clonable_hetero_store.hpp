 /**
 * \file: clonable_hetero_store.hpp
 *
 */

#ifndef CLONABLE_HETERO_STORE_HPP
#define CLONABLE_HETERO_STORE_HPP

#include <vector>
#include <stdexcept>

//! ---------- ALAPÉRTELMEZETT KIVÉTEL ----------
struct DefaultException : std::out_of_range {
    DefaultException(const std::string& msg = "")
        : std::out_of_range(msg)
    {}
};


//! ---------- ALAPÉRTELMEZETT TÁROLÓ ----------
template <typename T>
struct DefaultContainer : std::vector<T> {};


//! ---------- KLÓNOZHATÓ HETEROGÉN TÁROLÓ ----------
template <
    typename Base,                            // Alaposztály
    typename Container = std::vector<Base*>,  // Belső tároló
    typename Exception = std::out_of_range    // Throw kivétel
>
class ClonableHeteroStore {
private:
    Container store;

public:
    typedef typename Container::const_iterator const_iterator;


    //! ---------- KONSTRUKTOROK / DESTRUKTOR ----------
    ClonableHeteroStore()
        : store()
    {}

    ~ClonableHeteroStore() {
        clear();
    }

    ClonableHeteroStore(const ClonableHeteroStore& other)
        : store()
    {
        for (const_iterator it = other.begin(); it != other.end(); ++it) {

            if (*it != NULL)
                add((*it)->clone());

            else {
                try {
                    store.push_back(NULL);
                } catch (std::bad_alloc&) {
                    throw Exception("Memoria allokacios hiba a copy konstruktorban");
                }
            }
        }
    }


    //! ---------- ÉRTÉKADÁS ----------
    ClonableHeteroStore& operator=(const ClonableHeteroStore& other) {

        if (this != &other) {
            clear();

            for (const_iterator it = other.begin(); it != other.end(); ++it) {

                if (*it != NULL)
                    add((*it)->clone());
                else {
                    try {
                        store.push_back(NULL);
                    } catch (std::bad_alloc&) {
                        throw Exception("Memoria allokacios hiba az ertekadasban");
                    }
                }
            }
        }

        return *this;
    }


    //! ---------- ITERÁTOROK ----------
    const_iterator begin() const {
        return store.begin();
    }

    const_iterator end() const {
        return store.end();
    }

    std::size_t size() const {
        return store.size();
    }


    //! ---------- ADATOK KEZELÉSE ----------
    void add(Base* p) {

        try {
            store.push_back(p);
        } catch (std::bad_alloc&) {

            if (p != NULL)
                delete p;

            throw Exception("Nem sikerult az objektumot a taroloba betenni");
        }
    }

    void clear() {

        for (const_iterator it = store.begin(); it != store.end(); ++it) {

            if (*it != NULL)
                delete *it;
        }

        store.clear();
    }
};

#endif // CLONABLE_HETERO_STORE_HPP