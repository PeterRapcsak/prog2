#ifndef SERIALIZABLE_H
#define SERIALIZABLE_H

#include <iostream>

struct Serializable {
    virtual void write(std::ostream&) const = 0;
    virtual void read(std::istream&) = 0;
    virtual ~Serializable() {}
};

#endif // SERIALIZABLE_HPP
