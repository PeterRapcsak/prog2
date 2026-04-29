/**
 * \file perziszt.cpp
 */

#include "elkeszult.h"

/// Ellenőrizzük, hogy az elkeszult.h-ban definiálta-e az ELKESZULT makrót
#ifndef ELKESZULT
#define ELKESZULT 0     /// ha mégse definiálta
#endif

#if ELKESZULT < 0       /// ha hibásan definiálta
#undef ELKESZULT
#define ELKESZULT 0
#endif
/// ------- Ellenőrző rész vége ----------

#include <iostream>
#include <sstream>

#include "palkalmazott.h"

#if ELKESZULT >= 3
# include "pirodistak.h"
#endif

#include "gtest_lite.h"

void perzTest(){

    TEST(perzist, Alkalmazott) {
        std::stringstream ss;
        Alkalmazott  ld("Lusta Dick", 100);
        PAlkalmazott(ld).write(ss);
        PAlkalmazott rsLd("", 0);
        rsLd.read(ss);
        EXPECT_EQ(ld.getNev(), rsLd.getNev());
        EXPECT_EQ(ld.getFiz(), rsLd.getFiz());
    END }

#if ELKESZULT >= 3
    TEST(perzist, CsopVez) {
        std::stringstream ss;
        CsopVez gt("Mr. Gatto", 100, 5);
        PCsopVez(gt).write(ss);
        PCsopVez rsGt("", 0, 0);
        rsGt.read(ss);
        EXPECT_EQ(gt.getNev(), rsGt.getNev());
        EXPECT_EQ(gt.getFiz(), rsGt.getFiz());
        EXPECT_EQ(gt.getCs(), rsGt.getCs());
    END }
#endif

#if ELKESZULT >= 4
    TEST(perzist, HatIdeju) {
        std::stringstream ss;
        HatIdeju gr("Grabovszki", 100, 5000);
        PHatIdeju(gr).write(ss);
        PHatIdeju rsGr("", 0, 0);
        rsGr.read(ss);
        EXPECT_EQ(gr.getNev(), rsGr.getNev());
        EXPECT_EQ(gr.getFiz(), rsGr.getFiz());
        EXPECT_EQ(gr.getIdo(), rsGr.getIdo());
    END }
#endif

#if ELKESZULT >= 5
    TEST(perzist, HatIdCsV) {
        std::stringstream ss;
        HatIdCsV tf("Mr. Tejfel", 100, 5555, 3);
        PHatIdCsV(tf).write(ss);
        PHatIdCsV rstf("", 0, 0, 0);
        rstf.read(ss);
        EXPECT_EQ(tf.getNev(), rstf.getNev());
        EXPECT_EQ(tf.getFiz(), rstf.getFiz());
        EXPECT_EQ(tf.getCs(), rstf.getCs());
        EXPECT_EQ(tf.getIdo(), rstf.getIdo());
    END }
#endif

#if ELKESZULT >= 6
    TEST(perzist, HatIdCsVezH) {
        std::stringstream ss;
        HatIdCsV tf("Mr. Tejfel", 100, 5555, 3);
        HatIdCsVezH  safi("Safranek", 100, 55, tf);
        PHatIdCsVezH(safi).write(ss);
        PHatIdCsVezH rsafi("", 0, 0, tf);
        rsafi.read(ss);
        EXPECT_EQ(safi.getNev(), rsafi.getNev());
        EXPECT_EQ(safi.getFiz(), rsafi.getFiz());
        EXPECT_EQ(safi.getCs(), rsafi.getCs());
        EXPECT_EQ(safi.getIdo(), rsafi.getIdo());
    END }
#endif

}
