/*======================================================================
    TESZT PROGRAM!!!
----------------------------------------------------------------------
    CÉL:
     - Osztályok használatának bemutatása
     - Betöltés és statikus segédfüggvények kipróbálása (olykor rossz inputokkal is)
======================================================================*/

#include "memtrace.h" // memóriaszivárgás check

#include "gtest_lite.h" //gtest_lite használata a tesztekhez

/*
    EXPECT_EQ       | egyenlőség teszt
    EXPECT_TRUE     | igaz teszt
    EXPECT_FALSE    | hamis teszt
    EXPECT_GT       | greater than teszt
    EXPECT_NO_THROW | nem dob kivételt teszt
*/

#include <vector>
#include <iostream>

#include "filemanager.h"
#include "game.h"
#include "highscore.h"
#include "question.h"
#include "utils.h"

using std::string;

//! ---------- TESZTPROGRAM ----------

// Visszatérési érték: 0 = siker/nem futtatjuk, 1 = futtattuk
int tesztelek(bool run) {
    if (!run) {
        return 0; 
    }

    //! ---------- GAME ----------

    //? Utils segédfüggvényei hibás inputokkal
    TEST(Utils, FormatPrize)
        EXPECT_EQ(string("0"), formatPrize(0));
        EXPECT_EQ(string("-5.000.000"), formatPrize(-5000000));
        EXPECT_EQ(string("999"), formatPrize(999));
    END

    TEST(Utils, IsSafeLevel)
        EXPECT_FALSE(isSafeLevel(-1));
        EXPECT_FALSE(isSafeLevel(0));
        EXPECT_TRUE(isSafeLevel(4));   // 5. szint
        EXPECT_TRUE(isSafeLevel(9));   // 10. szint
        EXPECT_FALSE(isSafeLevel(67)); // 10. szint után
    END

    TEST(Utils, GetSafePrize)
        EXPECT_EQ(0, getSafePrize(-1));
        EXPECT_EQ(0, getSafePrize(0));
        EXPECT_EQ(100000, getSafePrize(4));   // 5. szint után
        EXPECT_EQ(1500000, getSafePrize(9));  // 10. szint után
        EXPECT_EQ(1500000, getSafePrize(67)); // 10. szint után
    END


    //! ---------- CHOOSE QUESTION ----------

    //? Hibás input: CHOOSE class 
    TEST(ChooseQuestion, CheckAnswer)
        std::vector<std::string> temp = {"optA", "optB", "optC", "optD"};
        ChooseQuestion badChoose(0, "", "", "A", temp); // A helyes válasz = 'A'

        EXPECT_FALSE(badChoose.checkAnswer("Z")); // hibás input
        EXPECT_TRUE(badChoose.checkAnswer("A"));  // helyes input
    END

    TEST(ChooseQuestion, GetDifficulty)
        std::vector<std::string> temp = {"optA", "optB", "optC", "optD"};
        ChooseQuestion q(5, "Teszt kérdés", "Kategória", "B", temp);
        
        EXPECT_EQ(5, q.getDifficulty());
    END


    //! ---------- ORDER QUESTION ----------

    //? Hibás input: ORDER class
    TEST(OrderQuestion, CheckAnswer)
        std::vector<std::string> temp = {"optA", "optB", "optC", "optD"};
        OrderQuestion badOrder("Valami bla bla", "cat", "ABCD", temp); // A helyes válasz = "ABCD"

        EXPECT_FALSE(badOrder.checkAnswer("AB"));  // hibás input
        EXPECT_TRUE(badOrder.checkAnswer("ABCD")); // helyes input
    END


    //! ---------- LOADING ----------

    //? Nincs file / rossz path
    TEST(FileManager, LoadChooseQuestions_MissingFile)
        std::vector<ChooseQuestion> missingChoose = FileManager::loadChooseQuestions("hajnal.csv");
        EXPECT_TRUE(missingChoose.empty()) << "Nem töltött be semmit";
    END

    TEST(FileManager, LoadOrderQuestions_MissingFile)
        std::vector<OrderQuestion> missingOrder = FileManager::loadOrderQuestions("kettővan.csv");
        EXPECT_TRUE(missingOrder.empty()) << "Nem töltött be semmit";
    END


    //? Kérdések betöltése fileból

    // Ezt egy jó 20p volt mire megoldottam
    // 'U' kell a végére mert size_t-t ad vissza a .size() és int-tel akartam comparelnni
    // ....
    TEST(FileManager, LoadChooseQuestions)
        std::vector<ChooseQuestion> choose = FileManager::loadChooseQuestions("kerdesek.csv");
        EXPECT_GT(choose.size(), 0U) << "Feleletválasztós kérdések betöltve";
    END

    TEST(FileManager, LoadOrderQuestions)
        std::vector<OrderQuestion> order = FileManager::loadOrderQuestions("sorkerdesek.csv");
        EXPECT_GT(order.size(), 0U) << "Sorrendezős kérdések betöltve";
    END


    //? Betöltött kérdések száma

    TEST(FileManager, QuestionCounts)
        std::vector<ChooseQuestion> choose = FileManager::loadChooseQuestions("kerdesek.csv");
        std::vector<OrderQuestion> order = FileManager::loadOrderQuestions("sorkerdesek.csv");
        
        EXPECT_GT(choose.size(), 0U); 
        EXPECT_GT(order.size(), 0U);  
    END


    //! ---------- DICSŐSÉGLISTA ----------
    // MÁR NEM CSAK SUCCEED

    //? Hozzáadás megnöveli a size-t (üres listából indulva)
    TEST(HighScoreTable, AddIncreasesSize)
        HighScoreTable t("test.csv");
        t.reset(); // tiszta lap

        EXPECT_EQ(0U, t.size()) << "reset után üres legyen";

        t.add("Teszt1", 500000);
        EXPECT_EQ(1U, t.size()) << "1 hozzáadás = 1 elem legyen";

        t.add("Teszt2", 100000);
        EXPECT_EQ(2U, t.size()) << "2 hozzáadás = 2 elem legyen";

        t.reset(); // takarítás magunk után
    END

    //? Üres név -> "Játékos" default
    // (a HighScoreTable::add() ezt csinálja, nem a hívó dolga)
    TEST(HighScoreTable, EmptyNameToDefault)
        HighScoreTable t("test.csv");
        t.reset();

        t.add("", 500000);

        EXPECT_EQ(1U, t.size()) << "üres nevet is hozzáadta";

        t.reset();
    END

    //? Negatív nyereményt NEM tárolunk
    TEST(HighScoreTable, AddNegativePrizeStored)
        HighScoreTable t("test.csv");
        t.reset();

        t.add("Negativ", -100000);

        EXPECT_EQ(0U, t.size()) << "negatív nyereményt nem ad hozzá";
        t.reset();
    END

    //? Nulla nyeremény
    TEST(HighScoreTable, AddZeroPrize)
        HighScoreTable t("test.csv");
        t.reset();

        t.add("Nulla", 0);

        EXPECT_EQ(1U, t.size());

        t.reset();
    END

    //? Reset valóban kiüríti a listát ÉS a filet
    TEST(HighScoreTable, ResetClearsEverything)
        HighScoreTable t("test.csv");
        t.add("Teszt", 1000);
        t.add("Teszt2", 2000);
        t.reset();

        EXPECT_EQ(0U, t.size()) << "memóriából eltűnt";

        t.save();

        // Új load is üres lesz, mert a filet is felülírtuk
        HighScoreTable t2("test.csv");
        EXPECT_EQ(0U, t2.size()) << "fileból betöltve is üres";
    END

    //? Display ne crasheljen üres listán semú
    // EXPECT_NO_THROW jobb mint a sima SUCCEED()
    TEST(HighScoreTable, DisplayDoesNotCrash)
        EXPECT_NO_THROW({
            HighScoreTable t("test.csv");
            t.reset();
            t.display(); // üres lista
            t.add("xd", 100);
            t.display(); // 1 elemmel
            t.reset();
        });
    END

    std::cout << "\nTeszt vége\n";
    waitEnter(); // el is lehessen olvasni a teszt eredményét

    return 1;
}