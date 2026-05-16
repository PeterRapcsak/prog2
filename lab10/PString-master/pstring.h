#ifndef PSTRING_H
#define PSTRING_H

#include "string5.h"
#include "serializable.h"
#include <iosfwd>

//! ---------- PSTRING OSZTÁLY ----------

// CÉL: Olyan String, ami fileba írható és filebol visszaolvasható
class PString : public String, public Serializable {
public:

    //! ---------- KONSTRUKTOROK ----------

    PString(const char *p = "");

    PString(char ch);

    PString(const String& s);

    PString(const PString& ps);


    //! ---------- ÍRÁS ----------

    // CÉL: A PString tartalmának kiírása bin streambe
    // Először a hossz, utána a szöveg
    virtual void write(std::ostream& os) const;


    //! ---------- OLVASÁS ----------

    // CÉL: A PString tartalmának kiírása bin streambe
    // Először a hossz, utána a szöveg
    virtual void read(std::istream& is);


    //! ---------- DESTRUKTOR ----------

    // CÉL: Leszármaztatott osztály miatt virtuális destruktor
    virtual ~PString();
};

#endif // PSTRING_H