#ifndef EMBER_H
#define EMBER_H

class Ember {
    char *nev;      // dinamikusan tárolt név
protected:
    int szulEv;     // születési év
public:
    // Default konstrtuktor
    // n - név
    // ev - születési év
    Ember(const char* n = "T0R2E4", int ev = 2005);

    // Másoló konstruktor
    // e - másolandó Ember
    Ember(const Ember& e);

    // Értékadó operátor
    // e - rhs operandus
    // return : referencia önmagára
    Ember& operator=(const Ember& e);

    // Név lekérdezése
    // return : pointer a névre
    const char* getNev() const;

    // Adott évben betöltött kor lekérdezése
    // ev - adott év
    // return : betöltött kor
    virtual int getKor(int ev) const;

    // Foglalkozás lekérdezése
    // return : foglalkozás szövege
    virtual const char* foglalkozas() const;

    // Virtuális destruktor
    virtual ~Ember();
};

#endif
