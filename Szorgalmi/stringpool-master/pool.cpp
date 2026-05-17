/**
 * \file pool.cpp
 *
 * StringPool osztály implemntációja
 * Valósítsd meg az osztály metódusait, a leírást a header fájlban találod.
 * 
 * Ezt a fájlt be kell adni (fel kell tölteni) a megoldással.
 */

#include "memtrace.h"
#include "pool.h"
#include <cstring>

using std::vector;

const char* NEPTUN = "T0R2E4";

//? PoolItem működése
struct StringPool::PoolItem {
    RString* str;   // Maga a string
    bool available; // Szabad-e? 

    PoolItem(size_t capacity) {
        str = new RString(capacity);
        available = true;
    }

    ~PoolItem() {
        delete str;
    }
};

//? CTOR
StringPool::StringPool(size_t obj_num, size_t init_cap) {

    for (size_t i = 0; i < obj_num; i++) {
        insert_item(new PoolItem(init_cap));
    }
}

//? Destruktor, végig iterálunk rajta és mindent felszabadítunk
StringPool::~StringPool() {

    for (vector<PoolItem*>::iterator i = items.begin(); i != items.end(); ++i) {
        delete *i; // NEM az iterator-t töröljük, hanem a PoolItem-et amire mutat
    }
}

// Egyszerű find függvény
// itt most const_iterator-t hasznalok mert csak keresünk
StringPool::PoolItem* StringPool::find_item(const RString& str) const {

    for (vector<PoolItem*>::const_iterator i = items.begin(); i != items.end(); ++i) {

        if ((*i)->str == &str) {
            return *i; // visszaadjuk a PoolItem-re mutató pointert
        }
    }
    return nullptr;
}

// Find függvény, csak most a szabad item-ek között keresünk
StringPool::PoolItem* StringPool::find_free(size_t capacity) const {

    for (vector<PoolItem*>::const_iterator i = items.begin(); i != items.end(); ++i) {

        if ((*i)->available && (*i)->str->capacity() >= capacity) {
            return *i;
        }
    }

    return nullptr;
}

// 
void StringPool::insert_item(PoolItem* item) {

    for (vector<PoolItem*>::iterator i = items.begin(); i != items.end(); ++i) {

        // Az Rstring kapacitása alapján rendezzük a pool-t 
        if ((*i)->str->capacity() > item->str->capacity()) {
            items.insert(i, item); // HA igaz, akkor ide szúrjuk be
            return;
        }
    }
    items.push_back(item); // Különben a végére tesszük
}


//! ------------ A FELADAT TÉNYLEGES MEGOLDÁSA ------------

RString& StringPool::acquire(size_t capacity) {
    if (capacity == 0) {
        throw NEPTUN;
    }

    // Próbálunk találni neki helyet a pool-ban
    PoolItem* item = find_free(capacity);

    // Ha nem sikerult, létrehozunk egy újat
    if (item == nullptr) {
        item = new PoolItem(capacity);
        item->available = false;

        // Betesszük a pool-ba a megfelelő helyre
        insert_item(item);
        return *item->str;
    }

    // Ha találtunk helyet, akkor azt foglaljuk le
    item->available = false;
    *item->str = ""; // Reseteljük a stringet
    return *item->str;
}

RString& StringPool::acquire(const char* str) {
    if (str == nullptr) {
        throw NEPTUN;
    }

    // Próbálunk találni neki helyet a pool-ban
    size_t needed = std::strlen(str) + 1;
    PoolItem* item = find_free(needed);

    // Ha nem sikerult, létrehozunk egy újat
    if (item == nullptr) {
        item = new PoolItem(needed);
        item->available = false;

        // Betesszük a pool-ba a megfelelő helyre
        insert_item(item);
    } else {
        // Ha találtunk helyet, akkor azt foglaljuk le
        item->available = false;
    }

    // Mindkét esetben visszaadjuk a stringet
    *item->str = str;
    return *item->str;
}

//? Újra kiosztható-e a str objektum?
bool StringPool::acquireable(const RString& str) const {

    PoolItem* item = find_item(str); // Benne van már a pool-ban?

    return item != nullptr && item->available; // Ha igen és szabad akkor true
}

//? Lenulláz egy objektumot, így újra kiosztható lesz
void StringPool::release(RString& str) {

    PoolItem* item = find_item(str); // Benne van már a pool-ban?

    if (item == nullptr) {
        return;
    }

    *item->str = ""; // Nullázzuk a stringet, mivel van már értékadó operátorunk
    item->available = true;
}

//? Hozzáfűzi str1 végére str2-t.
RString& StringPool::append(RString& str1, const RString& str2) {
    PoolItem* item1 = find_item(str1);
    PoolItem* item2 = find_item(str2);

    // Ha nincs benne / nincs lefoglalva
    if (item1 == nullptr || item2 == nullptr || item1->available || item2->available) {
        throw NEPTUN;
    }

    // Szükséges méret
    size_t len1 = str1.size();
    size_t len2 = str2.size();
    size_t needed = len1 + len2 + 1;

    if (str1.capacity() >= needed) {

        char* text1 = str1;
        const char* text2 = str2;

        // text1 végére másoljuk text2-t
        for (size_t i = 0; i < len2; i++) {
            text1[len1 + i] = text2[i];
        }

        text1[len1 + len2] = '\0'; // lezáró '\0'
        return str1; // mivel itt végeztük el a műveletet, így csak simán visszaadjuk
    }

    //? HA str1 kapacitása nem elég, keresünk egy új stringet amibe beleférnek
    RString& result = acquire(needed);

    // Most mindkettő const, mert mindkettőt másoljuk
    const char* text1 = str1;
    const char* text2 = str2;

    char* text_result = result;
    result = text1; // a megtalált stringbe másoljuk str1-et

    // és a végére fűzzük str2-t
    for (size_t i = 0; i < len2; i++) {
        text_result[len1 + i] = text2[i];
    }

    text_result[len1 + len2] = '\0'; // Lezárjuk
    return result; // Visszaadjuk
}

//? Pool méret getter
size_t StringPool::size() const {
    return items.size();
}

//? Pool-ban szabad objektumok száma getter
size_t StringPool::free_size() const {

    size_t count = 0;

    for (vector<PoolItem*>::const_iterator i = items.begin(); i != items.end(); ++i) {
        if ((*i)->available) {
            count++;
        }
    }
    return count;
}
