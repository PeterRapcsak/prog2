/**
 *  \file: clonable.h
 *
 *  Clonable absztrakt osztály
 */

#ifndef CLONABLE_H
#define CLONABLE_H

/// Clonable absztrakt osztály.
struct Clonable {
    /// Előállít egy önmagával azonos példányt
    virtual Clonable* clone() const = 0;

    virtual ~Clonable() {};
};

#endif // CLONABLE_H
