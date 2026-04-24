#ifndef MYSET_HPP
#define MYSET_HPP

#include <cstddef>
#include "point.h"

#ifndef ELKESZULT
#define ELKESZULT 4
#endif

// Összehasonlító operátor
// lhs és rhs mivel nincs this. globalis == operator
inline bool operator==(const Point& lhs, const Point& rhs) {
    return lhs.getX() == rhs.getX() && lhs.getY() == rhs.getY();
}

// Általános összehasonlító sablon
// alapból operator==-t használ
template <typename T>
bool equal(const T& lhs, const T& rhs) {
    return lhs == rhs;
}

// X szerinti összehasonlítás
inline bool Xequal(const Point& lhs, const Point& rhs) {
    return lhs.getX() == rhs.getX();
}

#if ELKESZULT == 4

template <>
inline bool equal<Point>(const Point& lhs, const Point& rhs) {
    return lhs.getX() == rhs.getX();
}

#endif

// Fix méretű halmaz.
// A harmadik sablonparaméterrel az összehasonlítás cserélhető.
template <typename T, std::size_t N = 10, bool (*Cmp)(const T&, const T&) = equal<T> >
class Set {
    T tarolo[N];
    std::size_t db;
public:
    Set() : db(0) {}

    // Méret visszaadás
    std::size_t size() const {
        return db;
    }

    // Elemkereső
    bool isElement(const T& elem) const {
        for (std::size_t i = 0; i < db; ++i)
            if (Cmp(tarolo[i], elem))
                return true;
        return false;
    }

    // Beszúró (ha még nincs benne, beteszi; ha megtelt, const char* kivételt dob)
    void insert(const T& elem) {
        if (isElement(elem))
            return;
        if (db >= N)
            throw "Nincs hely a halmazban";
        tarolo[db++] = elem;
    }
};

#endif // MYSET_HPP
