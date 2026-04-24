/**
 * \file test.cpp
 *
 * Cipher nevű szorgalmi feladat tesztjei gtest_lite eszközeivel megvalósítva 
 * A szorgalmi feladat megoldásához ezt az állományt nem kell beadni (feltölteni).
 *
 * A ELKESZULT makró vezérli az egyes tesztesetek fordítását, ezzel lehetővé válik
 * hogy kisebb lépésekben teszteljünk.
 *
 */

#include <iostream>
#include <ctime>

#include "gtest_lite.h"
#include "ciphers.h"
#include "memtrace.h"

#define ELKESZULT 0

/* ELKESZULT makró:
    = 5: Caesar titkosítás tesztjei
    = 8: Caesar és MyCipher titkosítás tesztjei
    = 10: Caesar, MyCipher és a CipherQueue tesztjei - az összes teszt
*/

int main() {
    GTINIT(std::cin);
    #if ELKESZULT > 0
        //Caesar titkosítás ellenőrzése
        TEST(Caesar shift=4, _alma) {
            CaesarCipher cc(4);
            std::string alma = cc.encode("alma");
            EXPECT_STREQ("epqe", alma.c_str());
            std::string almacr = cc.decode(alma);
            EXPECT_STREQ("alma", almacr.c_str());
        } END
    #endif

    #if ELKESZULT > 1
        TEST(Caesar shift=3, _uvwxyz) {
            CaesarCipher cc(3);
            std::string a = cc.encode("uvwxyz");
            EXPECT_STREQ("xyzabc", a.c_str());
            std::string b = cc.decode(a);
            EXPECT_STREQ("uvwxyz", b.c_str());
        } END
    #endif

    #if ELKESZULT > 2
        TEST(Caesar shift=-10, _caesar) {
            CaesarCipher cc(-10);
            std::string a = cc.encode("caesar");
            EXPECT_STREQ("squiqh", a.c_str());
            std::string b = cc.decode(a);
            EXPECT_STREQ("caesar", b.c_str());
        } END
    #endif

    #if ELKESZULT > 3
        TEST(Caesar shift=1000, _caesar) {
            CaesarCipher cc(1000);
            std::string a = cc.encode("caesar");
            EXPECT_STREQ("omqemd", a.c_str());
            std::string b = cc.decode(a);
            EXPECT_STREQ("caesar", b.c_str());
        } END
    #endif

    #if ELKESZULT > 4
        TEST(Caesar shift=10, _ez egy uzenet) {
            CaesarCipher cc(10);
            const std::string str = "ez egy uzenet";
            std::string a = cc.encode(str);
            EXPECT_STREQ("oj oqi ejoxod", a.c_str());
            std::string b = cc.decode(a);
            EXPECT_STREQ("ez egy uzenet", b.c_str());
        } END
    #endif

    #if ELKESZULT > 5
        TEST(My_Cipher key=abc counter=0, _titkositas) {
            MyCipher mc("abc");
            std::string a = mc.encode("titkositas");
            EXPECT_STREQ("tkxntzobkb", a.c_str());
            std::string b = mc.decode(a);
            EXPECT_STREQ("titkositas", b.c_str());
        } END
    #endif
    //My cipher tesztek
    #if ELKESZULT > 6
        TEST(const My_Cipher key=cipher counter=10, _titkositas) {
            const MyCipher mc("cipher", 10);
            Cipher * const clone = mc.clone();
            std::string a = clone->encode("titkositas");
            EXPECT_STREQ("fbuegyashs", a.c_str());
            std::string b = clone->decode(a);
            EXPECT_STREQ("titkositas", b.c_str());
            delete clone;
        } END
    #endif

    #if ELKESZULT > 7
        TEST(My_Cipher key=xyz counter=-3, _virtualis destruktor) {
            MyCipher mc("xyz", -3);
            std::string a = mc.encode("virtualis destruktor");
            EXPECT_STREQ("pepqtblkw ilybbdvgaf", a.c_str());
            std::string b = mc.decode(a);
            EXPECT_STREQ("virtualis destruktor", b.c_str());
        } END
    #endif
    //CipherQueue tesztek
    #if ELKESZULT > 8
        TEST(Cipher_Queue: caesar + mycipher, _meg tobb szorgalmit prog2bol) {
            CipherQueue queue;
            const CaesarCipher caesar(6);
            const MyCipher myCipher("abc", -2);
            queue.add(caesar.clone());
            queue.add(myCipher.clone());
            std::string str1 = queue.encode("meg tobb szorgalmit progkettobol");
            EXPECT_STREQ("qko czln fofhyuehfp pqpjmizyvkwv", str1.c_str());
            std::string str2 = queue.decode(str1);
            EXPECT_STREQ("meg tobb szorgalmit progkettobol", str2.c_str());
        } END
    #endif
    CipherQueue queue; //kell majd a következőkhöz is
    #if ELKESZULT > 9
        TEST(Cipher_Queue: inner queue, _pointert mindenhova_alfa) {
            CipherQueue inner;
            inner.add(new MyCipher("queue"));
            inner.add(new CaesarCipher(10));
            queue.add(inner.clone());
            queue.add(queue.clone()); //ennek sem kellene gondot okoznia
            std::string str1 = queue.encode("pointert mindenhova");
            EXPECT_STREQ("pyobdolj gcrdmrlcfs", str1.c_str());
            std::string str2 = queue.decode(str1);
            EXPECT_STREQ("pointert mindenhova", str2.c_str());
        } END
    #endif
    #if ELKESZULT > 9
        TEST(Cipher_Queue: inner queue, _pointert mindenhova_beta) {
            std::srand(std::time(nullptr));
            CipherQueue inner;
            inner.add(new MyCipher("queue"));
            inner.add(new CaesarCipher(std::rand() % 26));
            queue.add(inner.clone());
            queue.add(queue.clone()); //ennek sem kellene gondot okoznia
            std::string str = queue.decode(queue.encode("pointert mindenhova"));
            EXPECT_STREQ("pointert mindenhova", str.c_str());
        } END
    #endif

    if (ELKESZULT < 10)
      ADD_FAILURE() << "\nNem futott minden teszteset!" << std::endl;

    #if ELKESZULT > 7
        //Neptun ellenőrzése
        TEST(Kivetelek, Caesar/MyCipher) {
            CaesarCipher cc(2);
            MyCipher mc("a");
            try {
                EXPECT_THROW_THROW(cc.decode("A"), const char* p);
            } catch (const char *p) {
            #ifdef CPORTA
                EXPECT_ENVCASEEQ("ORG_ID", p);
            #endif
            }
            try {
                EXPECT_THROW_THROW(mc.encode("_"), const char* p);
            } catch (const char *p) {
            #ifdef CPORTA
                EXPECT_ENVCASEEQ("ORG_ID", p);
            #endif
            }
        } END
    #endif

    GTEND(std::cerr);
    return 0;
}

