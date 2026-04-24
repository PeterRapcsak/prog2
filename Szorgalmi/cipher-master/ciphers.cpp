/*======================================================================
    ciphers.cpp - Titkosito osztalyok implementacioja
----------------------------------------------------------------------
    CEL:
     - CaesarCipher, MyCipher, CipherQueue metodusok implementacioja
     - ervenyessegi ellenorzes (abc kisbetuk + szokoz)
======================================================================*/

#include <cstdlib>

#include "memtrace.h"
#include "ciphers.h"


//! ---------- BELSŐ SEGÉDFÜGGVÉNYEK ----------

/*
    CEL: Visszaadja a dobando azonositot
    MEGJEGYZES:
     - CPORTA alatt ORG_ID kornyezeti valtozobol olvassuk
     - lokalis futasnal legyen egy alapertelmezett ertek
*/
namespace {

const char* getOrgId(void) {
    const char* id = std::getenv("ORG_ID");
    if (id != NULL)
        return id;
    return "ABC123";
}

/*
    CEL: Ellenorzi, hogy az uzenet karaktere megfelelo-e
*/
bool validMessageChar(char ch) {
    return ch == ' ' || (ch >= 'a' && ch <= 'z');
}

/*
    CEL: Ellenorzi, hogy a kulcs karaktere megfelelo-e
*/
bool validKeyChar(char ch) {
    return ch >= 'a' && ch <= 'z';
}

/*
    CEL: 26-os modulo pozitiv tartomanyban
*/
int norm26(int n) {
    int r = n % 26;
    if (r < 0)
        r += 26;
    return r;
}

/*
    CEL: Egy kisbetu eltolasa adott mertekben
*/
char shiftLetter(char ch, int amount) {
    int idx = ch - 'a';
    int moved = norm26(idx + amount);
    return (char)('a' + moved);
}

/*
    CEL: Uzenet ellenorzese encode/decode elott
    HIBA: ervenytelen karakter eseten a Neptun kodot dobjuk
*/
void checkMessage(const std::string& str) {
    for (std::size_t i = 0; i < str.size(); ++i) {
        if (!validMessageChar(str[i]))
            throw getOrgId();
    }
}

/*
    CEL: Kulcs ellenorzese
    HIBA: ures vagy ervenytelen kulcs eseten a Neptun kodot dobjuk
*/
void checkKey(const std::string& key) {
    if (key.empty())
        throw getOrgId();

    for (std::size_t i = 0; i < key.size(); ++i) {
        if (!validKeyChar(key[i]))
            throw getOrgId();
    }
}

} // namespace


//! ---------- CAESARCIPHER ----------

CaesarCipher::CaesarCipher(int shift) : shift(shift) {}

std::string CaesarCipher::encode(const std::string& message) {
    checkMessage(message);

    std::string out = message;
    for (std::size_t i = 0; i < out.size(); ++i) {
        if (out[i] != ' ')
            out[i] = shiftLetter(out[i], shift);
    }

    return out;
}

std::string CaesarCipher::decode(const std::string& ciphertext) {
    checkMessage(ciphertext);

    std::string out = ciphertext;
    for (std::size_t i = 0; i < out.size(); ++i) {
        if (out[i] != ' ')
            out[i] = shiftLetter(out[i], -shift);
    }

    return out;
}

Cipher* CaesarCipher::clone() const {
    return new CaesarCipher(*this);
}


//! ---------- MYCIPHER ----------

MyCipher::MyCipher(const std::string& key) : key(key), offset(0) {
    checkKey(this->key);
}

MyCipher::MyCipher(const std::string& key, int offset) : key(key), offset(offset) {
    checkKey(this->key);
}

std::string MyCipher::encode(const std::string& message) {
    checkMessage(message);

    std::string out = message;

    // i minden karakter pozicioja, szokoz is beleszamit
    for (std::size_t i = 0; i < out.size(); ++i) {
        char ch = out[i];
        int keyShift = key[i % key.size()] - 'a';
        int amount = offset + (int)i + keyShift;

        if (ch != ' ')
            out[i] = shiftLetter(ch, amount);
    }

    return out;
}

std::string MyCipher::decode(const std::string& ciphertext) {
    checkMessage(ciphertext);

    std::string out = ciphertext;

    // i minden karakter pozicioja, szokoz is beleszamit
    for (std::size_t i = 0; i < out.size(); ++i) {
        char ch = out[i];
        int keyShift = key[i % key.size()] - 'a';
        int amount = offset + (int)i + keyShift;

        if (ch != ' ')
            out[i] = shiftLetter(ch, -amount);
    }

    return out;
}

Cipher* MyCipher::clone() const {
    return new MyCipher(*this);
}


//! ---------- CIPHERQUEUE ----------

CipherQueue::CipherQueue() {}

CipherQueue::CipherQueue(const CipherQueue& rhs) {
    copyFrom(rhs);
}

CipherQueue& CipherQueue::operator=(const CipherQueue& rhs) {
    if (this != &rhs) {
        dispose();
        copyFrom(rhs);
    }
    return *this;
}

CipherQueue::~CipherQueue() {
    dispose();
}

/*
    CEL: Felszabaditja a tarolt kodoloket
*/
void CipherQueue::dispose() {
    for (std::size_t i = 0; i < ciphers.size(); ++i)
        delete ciphers[i];

    ciphers.clear();
}

/*
    CEL: Mely masolat keszitese egy masik sorrol
*/
void CipherQueue::copyFrom(const CipherQueue& rhs) {
    for (std::size_t i = 0; i < rhs.ciphers.size(); ++i)
        ciphers.push_back(rhs.ciphers[i]->clone());
}

/*
    CEL: Uj kodolo hozzaadasa a sor vegehez
    MEGJEGYZES: a pointer tulajdonjoga a sorra szall
*/
void CipherQueue::add(Cipher* cipher) {
    ciphers.push_back(cipher);
}

/*
    CEL: Sorban, elejetol a vegeig titkosit
*/
std::string CipherQueue::encode(const std::string& message) {
    std::string out = message;

    for (std::size_t i = 0; i < ciphers.size(); ++i)
        out = ciphers[i]->encode(out);

    return out;
}

/*
    CEL: Sorban, vegetol az elejeig fejt vissza
*/
std::string CipherQueue::decode(const std::string& ciphertext) {
    std::string out = ciphertext;

    for (std::size_t i = ciphers.size(); i > 0; --i)
        out = ciphers[i - 1]->decode(out);

    return out;
}

Cipher* CipherQueue::clone() const {
    return new CipherQueue(*this);
}

