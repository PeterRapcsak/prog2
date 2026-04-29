/**
 * \file: monoton_test.cpp
 *
 *  Tesztprogram monoton sablonhoz.
 *
 *  Extra:
 *  A feladat megoldása mellett érdemes elemezni a tárolók és iterátorok használatát,
 *  valamint a szignálkezelést.
 *
 */

#include <iostream>
#include <csignal>
#include <stdexcept>
#include <string>
#include <vector>
#include <set>
#include <list>
#include <functional>
#include <algorithm>
#include <iterator>
#include <cstdlib>
#include <ctime>

#include "gtest_lite.h"
#include "fancy_iterators.hpp"
#include "monoton.hpp"

using std::cout;
using std::endl;
using std::make_pair;

/// Segédsablon a kiírásához.
/// Egy sorozattárolóból kiír egy intervallumot
/// Az ostream inserterét std::pair esetre definiáljuk. Így könnyen kiíratható
/// egy intervallum egy sorozattárolóból.
/// @param Iter - sablonparaméter: iterátor típusa
/// @param os   - ostream ahova kiír
/// @param p    - iteratorokból képzett std::pair
template <typename Iter>
std::ostream& operator<<(std::ostream& os, std::pair<Iter, Iter> p) {
    os << "** Ezt vizsgaltuk: ";
    if (p.first == p.second)
        os << "Ures halmaz";
    while (p.first != p.second)
        os << *p.first++ << ", ";
    return os << std::endl;
}

/// Becsapós segédsablon: mindig igazat/vagy hamisat ad
/// Arra jó, hogy elszálljon a program, ha túlfut az iterátor a monoton sablonban.
/// Ha túlfut (last után), akkor van valószínűsége, hogy illegális memóriacímet olvas
template <typename T>
struct const_pred {
    bool ret;
    const_pred(bool b = true): ret(b) {}
    bool operator()(T, T) {
        return ret;
    }
};

/// Segmentation violation signal kezelése.
/// Akkor keletkezik, ha olyan memóriacímet próbál elérni a program, ami
/// nem a programhoz tartozik, vagy írni akar egy nem írható címre.
/// Mivel az op. rendszertől érkező szignálok nem generálnak kivételt,
/// nekünk kell ezt programozottan megoldani.
/// Ez az út azonban nem minden szigál esetében járható. Az aszinkron szignalok
/// esetében a handlerből indított throw nem biztonságos.
void sigSegv(int signal) {
    throw std::out_of_range("Illegalis memoria muvelet: Valoszinu, hogy lefutott az iterator a tarolorol!");
}

/// Olyan osztály, ami képes jelezni, ha írják, vagy olvassák.
/// Ezzel ellenőrizzük, hogy a megadott intervallum határain belül marad-e.
class isAccessed {
    int flg;
    void read() const { throw std::out_of_range("Nem letezo elem olvasasa!"); }
    void write() const { throw std::out_of_range("Nem letezo elem irasa!"); }
public:
    static const int R = 1; // olvasáskor jelez
    static const int W = 2; // íráskor jelez
    isAccessed(int f = 0) :flg(f) {}

    // másoló olvasást jelent
    isAccessed(const isAccessed& a) :flg(a.flg) {
        if (flg & R) read();
    }
    // op=  írást jelent
    isAccessed& operator=(const isAccessed& a) {
        if (flg & W) write();
        flg = a.flg;
        return *this;
    }
};

int main() {
  GTINIT(std::cin);       // Csak C(J)PORTA működéséhez kell

  try {

    std::signal(SIGSEGV, sigSegv);  /// Signal handler beállítása

    /// Üres halmazra szinte minden halmaztulajdonság igaz.
    TEST(Test1, Sanity1) {
        const char* p = "duma";

        EXPECT_TRUE(monoton(p, p, std::greater<char>())) << make_pair(p, p);    /// hiba esetén kiírjuk az intervallumot
        EXPECT_TRUE(monoton(p, p, std::greater_equal<char>())) << make_pair(p, p);
        EXPECT_TRUE(monoton(p, p, std::less<char>())) << make_pair(p, p);
        EXPECT_TRUE(monoton(p, p, std::less_equal<char>())) << make_pair(p, p);
        EXPECT_TRUE(monoton(p, p, const_pred<char>())) << "A tulfutast teszteli" << endl;
        EXPECT_TRUE(monoton(p, p, const_pred<char>(false))) << "A tulfutast teszteli" << endl;
    } END

    isAccessed a[4];
    TEST(Test1, Sanity2) {
        a[0] = isAccessed::R|isAccessed::W;
        a[3] = isAccessed::R|isAccessed::W;
        EXPECT_TRUE(monoton(&a[1], &a[3], const_pred<isAccessed>())) << "A tulfutast es a hozzaferest teszteli" << endl;
        EXPECT_FALSE(monoton(&a[1], &a[3], const_pred<isAccessed>(false))) << "A tulfutast es a hozzaferest teszteli" << endl;
    } END

    TEST(Test1, Sanity3) {
        a[2] = isAccessed::R|isAccessed::W;
        EXPECT_TRUE(monoton(&a[1], &a[2], const_pred<isAccessed>())) << "A tulfutast es a hozzaferest teszteli" << endl;
    } END

    TEST(Test1, Sanity4) {
        a[1] = isAccessed::R|isAccessed::W;
        EXPECT_TRUE(monoton(&a[1], &a[1], const_pred<isAccessed>())) << "A tulfutast es a hozzaferest teszteli" << endl;
    } END

     /// counting_iterator int típussal
    TEST(Test2, counting_iterator) {
        counting_iterator<int> b = 10, e = 25;          /// szigorúan növekvő sorozat
        std::cout << make_pair(b, e);
        EXPECT_TRUE(monoton(b, e, std::greater<int>())) << make_pair(b, e);
        EXPECT_TRUE(monoton(b, e, std::greater_equal<int>())) << make_pair(b, e);
        EXPECT_FALSE(monoton(b, e, std::less<int>())) << make_pair(b, e);
        EXPECT_FALSE(monoton(b, e, std::less_equal<int>())) << make_pair(b, e);
        EXPECT_TRUE(monoton(b, e, const_pred<int>())) << "A tulfutast teszteli" << endl;
        EXPECT_FALSE(monoton(b, e, const_pred<int>(false))) << "A tulfutast teszteli" << endl;
    } END

    /// int tömb
    TEST(Test3, intArray) {
        int a[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};     /// ez is szigorúan növekvő sorozat
        std::cout << make_pair(a, a+10);
        EXPECT_TRUE(monoton(a, a+10, std::greater<int>())) << make_pair(a, a+10);
        EXPECT_TRUE(monoton(a, a+10, std::greater_equal<int>())) << make_pair(a, a+10);
        EXPECT_FALSE(monoton(a, a+10, std::less<int>())) << make_pair(a, a+10);
        EXPECT_FALSE(monoton(a, a+10, std::less_equal<int>())) << make_pair(a, a+10);
        EXPECT_TRUE(monoton(a, a+10, const_pred<int>())) << "A tulfutast teszteli" << endl;
        EXPECT_FALSE(monoton(a, a+10, const_pred<int>(false))) << "A tulfutast teszteli" << endl;
    } END

    /// Teszteseten kívül deklaráljuk, mert több tesztesetben használjuk
    std::vector<int> v;

    /// int vector szigorúan monoton
    TEST(Test4, intVector) {
        counting_iterator<int> b = 20, e = 50;          /// ez is szigorúan növekvő sorozat
        /// vektorba tesszük és visszafelé iterálunk
        v.assign(b, e);
        EXPECT_TRUE(monoton(v.rbegin(), v.rend(), std::less<int>())) << make_pair(v.rbegin(), v.rend());
        EXPECT_TRUE(monoton(v.rbegin(), v.rend(), std::less_equal<int>())) << make_pair(v.rbegin(), v.rend());
        EXPECT_FALSE(monoton(v.rbegin(), v.rend(), std::greater<int>())) << make_pair(v.rbegin(), v.rend());
        EXPECT_FALSE(monoton(v.rbegin(), v.rend(), std::greater_equal<int>())) << make_pair(v.rbegin(), v.rend());
        EXPECT_TRUE(monoton(v.rbegin(), v.rend(), const_pred<int>())) << "A tulfutast teszteli" << endl;
        EXPECT_FALSE(monoton(v.rbegin(), v.rend(), const_pred<int>(false))) << "A tulfutast teszteli" << endl;
        EXPECT_TRUE(monoton(v.begin(), v.end(), const_pred<int>())) << "A tulfutast teszteli" << endl;
        EXPECT_FALSE(monoton(v.begin(), v.end(), const_pred<int>(false))) << "A tulfutast teszteli" << endl;
    } END


    /// int
    TEST(Test5, intVectorRep1) {
        std::vector<int> u = v;
        u.insert(u.begin(), *u.begin());                    /// elején ismétlődik
        EXPECT_FALSE(monoton(u.begin(), u.end(), std::greater<int>())) << make_pair(u.begin(), u.end());
        EXPECT_TRUE(monoton(u.begin(), u.end(), std::greater_equal<int>())) << make_pair(u.begin(), u.end());
        EXPECT_FALSE(monoton(u.begin(), u.end(), std::less<int>())) << make_pair(u.begin(), u.end());
        EXPECT_FALSE(monoton(u.begin(), u.end(), std::less_equal<int>())) << make_pair(u.begin(), u.end());
    } END

    TEST(Test6, intVectorRep2) {
        std::vector<int> u = v;
        u.insert(u.end(), *(u.end()-1));                    /// végén ismétlődik
        EXPECT_FALSE(monoton(u.begin(), u.end(), std::greater<int>())) << make_pair(u.begin(), u.end());
        EXPECT_TRUE(monoton(u.begin(), u.end(), std::greater_equal<int>())) << make_pair(u.begin(), u.end());
        EXPECT_FALSE(monoton(u.begin(), u.end(), std::less<int>())) << make_pair(u.begin(), u.end());
        EXPECT_FALSE(monoton(u.begin(), u.end(), std::less_equal<int>())) << make_pair(u.begin(), u.end());
    } END

    TEST(Test7, intVectorRep3) {
        std::vector<int> u = v;
        u.insert(u.begin()+5, 2, *(u.begin()+5));              /// belül ismétlődik 3-szor
        EXPECT_FALSE(monoton(u.begin(), u.end(), std::greater<int>())) << make_pair(u.begin(), u.end());
        EXPECT_TRUE(monoton(u.begin(), u.end(), std::greater_equal<int>())) << make_pair(u.begin(), u.end());
        EXPECT_FALSE(monoton(u.begin(), u.end(), std::less<int>())) << make_pair(u.begin(), u.end());
        EXPECT_FALSE(monoton(u.begin(), u.end(), std::less_equal<int>())) << make_pair(u.begin(), u.end());
    } END

    TEST(Test8, intVectorRep4) {
        /// betesszük kétszer a w vektorba rendezve.
        std::vector<int> w(2*v.size()); // 2-szer akkora hely kell
        std::merge(v.begin(), v.end(), v.begin(), v.end(), w.begin());

        EXPECT_FALSE(monoton(w.begin(), w.end(), std::greater<int>())) << make_pair(w.begin(), w.end());
        EXPECT_TRUE(monoton(w.begin(), w.end(), std::greater_equal<int>())) << make_pair(w.begin(), w.end());
        EXPECT_FALSE(monoton(w.rbegin(), w.rend(), std::less<int>())) << make_pair(w.rbegin(), w.rend());
        EXPECT_TRUE(monoton(w.rbegin(), w.rend(), std::less_equal<int>())) << make_pair(w.rbegin(), w.rend());
        EXPECT_TRUE(monoton(w.rbegin(), w.rend(), const_pred<int>())) << "A tulfutast teszteli" << endl;
        EXPECT_FALSE(monoton(w.rbegin(), w.rend(), const_pred<int>(false))) << "A tulfutast teszteli" << endl;
        EXPECT_TRUE(monoton(w.begin(), w.end(), const_pred<int>())) << "A tulfutast teszteli" << endl;
        EXPECT_FALSE(monoton(w.begin(), w.end(), const_pred<int>(false))) << "A tulfutast teszteli" << endl;
    } END

    TEST(Test9, inUpDown) {
        /// Betesszük kétszer a w vektorba.
        /// Először növekvő, majd csökkenő sorrendben
        std::vector<int> w(v.begin(), v.end());
        w.insert(w.end(), v.rbegin(), v.rend());

        EXPECT_FALSE(monoton(w.begin(), w.end(), std::greater<int>())) << make_pair(w.begin(), w.end());
        EXPECT_FALSE(monoton(w.begin(), w.end(), std::greater_equal<int>())) << make_pair(w.begin(), w.end());
        EXPECT_FALSE(monoton(w.rbegin(), w.rend(), std::less<int>())) << make_pair(w.rbegin(), w.rend());
        EXPECT_FALSE(monoton(w.rbegin(), w.rend(), std::less_equal<int>())) << make_pair(w.rbegin(), w.rend());
        EXPECT_TRUE(monoton(w.rbegin(), w.rend(), const_pred<int>())) << "A tulfutast teszteli" << endl;
        EXPECT_FALSE(monoton(w.rbegin(), w.rend(), const_pred<int>(false))) << "A tulfutast teszteli" << endl;
        EXPECT_TRUE(monoton(w.begin(), w.end(), const_pred<int>())) << "A tulfutast teszteli" << endl;
        EXPECT_FALSE(monoton(w.begin(), w.end(), const_pred<int>(false))) << "A tulfutast teszteli" << endl;
    } END

    /// Teszteseten kívül deklaráljuk, mert több tesztesetben használjuk
    std::set<double> s;         /// Az std::set kulcs szerint rendezett !

    /// hasonló tesztek double halmazzal
    TEST(Test10, doubleSet) {
        std::srand(std::time(NULL));
        int n = rand() % 10 + 5;
        while (n--) {
            double d = ((double)rand()/RAND_MAX) * 1000;
            s.insert(d);
        }

        // A set miatt biztos, hogy szigorúan monoton lesz
        std::cout << make_pair(s.begin(), s.end());

        EXPECT_TRUE(monoton(s.begin(), s.end(), std::greater<double>())) << make_pair(s.begin(), s.end());
        EXPECT_TRUE(monoton(s.rbegin(), s.rend(), std::less<double>())) << make_pair(s.rbegin(), s.rend());
        EXPECT_TRUE(monoton(s.rbegin(), s.rend(), const_pred<double>())) << "A tulfutast teszteli" << endl;
        EXPECT_FALSE(monoton(s.rbegin(), s.rend(), const_pred<double>(false))) << "A tulfutast teszteli" << endl;
        EXPECT_TRUE(monoton(s.begin(), s.end(), const_pred<double>())) << "A tulfutast teszteli" << endl;
        EXPECT_FALSE(monoton(s.begin(), s.end(), const_pred<double>(false))) << "A tulfutast teszteli" << endl;

    } END

    /// hasonló tesztek double listával
    TEST(Test11, doubleList) {
        // itt áttesszük listába
        std::list<double> l(s.begin(), s.end());
        EXPECT_TRUE(monoton(l.begin(), l.end(), std::greater<double>())) << make_pair(l.begin(), l.end());
        EXPECT_TRUE(monoton(l.rbegin(), l.rend(), std::less<double>())) << make_pair(l.rbegin(), l.rend());
        EXPECT_TRUE(monoton(l.rbegin(), l.rend(), const_pred<double>())) << "A tulfutast teszteli" << endl;
        EXPECT_FALSE(monoton(l.rbegin(), l.rend(), const_pred<double>(false))) << "A tulfutast teszteli" << endl;
        EXPECT_TRUE(monoton(l.begin(), l.end(), const_pred<double>())) << "A tulfutast teszteli" << endl;
        EXPECT_FALSE(monoton(l.begin(), l.end(), const_pred<double>(false))) << "A tulfutast teszteli" << endl;

        // és elrontjuk a rendezettséget
        l.insert(l.end(), s.rbegin(), s.rend());
        EXPECT_FALSE(monoton(l.begin(), l.end(), std::greater<double>())) << make_pair(l.begin(), l.end());
        EXPECT_FALSE(monoton(l.begin(), l.end(), std::less<double>())) << make_pair(l.begin(), l.end());
        EXPECT_TRUE(monoton(l.rbegin(), l.rend(), const_pred<double>())) << "A tulfutast teszteli" << endl;
        EXPECT_FALSE(monoton(l.rbegin(), l.rend(), const_pred<double>(false))) << "A tulfutast teszteli" << endl;
        EXPECT_TRUE(monoton(l.begin(), l.end(), const_pred<double>())) << "A tulfutast teszteli" << endl;
        EXPECT_FALSE(monoton(l.begin(), l.end(), const_pred<double>(false))) << "A tulfutast teszteli" << endl;

    } END

    /// Sztringgel is megnézzük
    TEST(Test12, sring) {
        const char *duma[] = { "alma", "meggy", "szilva", "uborka" };
        std::vector<std::string> s(duma, duma+4);
        std::ostream_iterator<std::string> os(cout, ", ");
        std::copy(s.begin(), s.end(), os);
        cout << endl;

        EXPECT_TRUE(monoton(s.begin(), s.end(), std::greater<std::string>())) << make_pair(s.begin(), s.end());
        EXPECT_FALSE(monoton(s.rbegin(), s.rend(), std::greater<std::string>())) << make_pair(s.rbegin(), s.rend());
        EXPECT_TRUE(monoton(s.rbegin(), s.rend(), const_pred<std::string>())) << "A tulfutast teszteli" << endl;
        EXPECT_FALSE(monoton(s.rbegin(), s.rend(), const_pred<std::string>(false))) << "A tulfutast teszteli" << endl;
        EXPECT_TRUE(monoton(s.begin(), s.end(), const_pred<std::string>())) << "A tulfutast teszteli" << endl;
        EXPECT_FALSE(monoton(s.begin(), s.end(), const_pred<std::string>(false))) << "A tulfutast teszteli" << endl;

    } END
  } catch (std::exception& e) {
      FAIL() << e.what() << std::endl;
  }

/// itt a vége
  GTEND(std::cerr);       // Csak C(J)PORTA működéséhez kell
  return 0;
}

