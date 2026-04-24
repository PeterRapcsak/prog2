/*======================================================================
    ciphers.h - Titkosito osztalyok deklaracioja
----------------------------------------------------------------------
    CEL:
     - Cipher absztrakt interfesz
     - CaesarCipher, MyCipher, CipherQueue deklaracioja
======================================================================*/

#ifndef CIPHERS_H
#define CIPHERS_H

#include <string>
#include <vector>
#include "memtrace.h"

/*
    CEL: Absztrakt osztaly
*/
class Cipher {
public:
    virtual std::string encode(const std::string& message) = 0;
    virtual std::string decode(const std::string& ciphertext) = 0;
    virtual Cipher* clone() const = 0;
    virtual ~Cipher() { }
};


//! ---------- CAESAR TITKOSITO ----------

class CaesarCipher : public Cipher {
    int shift;  // eltolas merteke

public:
    CaesarCipher(int shift);

    std::string encode(const std::string& message);
    std::string decode(const std::string& ciphertext);
    Cipher* clone() const;
};


//! ---------- MY CIPHER ----------

class MyCipher : public Cipher {
    std::string key;    // kulcsszo
    int offset;         // kezdo eltolas

public:
    MyCipher(const std::string& key);
    MyCipher(const std::string& key, int offset);

    std::string encode(const std::string& message);
    std::string decode(const std::string& ciphertext);
    Cipher* clone() const;
};


//! ---------- CIPHER QUEUE ----------

class CipherQueue : public Cipher {
    std::vector<Cipher*> ciphers;   // heterogen kollekcio (Cipher pointerek)

    void dispose();
    void copyFrom(const CipherQueue& rhs);

public:
    CipherQueue();
    CipherQueue(const CipherQueue& rhs);
    CipherQueue& operator=(const CipherQueue& rhs);
    ~CipherQueue();

    void add(Cipher* cipher);

    std::string encode(const std::string& message);
    std::string decode(const std::string& ciphertext);
    Cipher* clone() const;
};

#endif // CIPHERS_H