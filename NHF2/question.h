/*======================================================================
    question.h - Kérdés osztályok deklarációja
----------------------------------------------------------------------
    FELADAT:
     - Question: absztrakt alap minden kérdéstípushoz
     - ChooseQuestion: feleletválasztós, nehézség 1-12
     - OrderQuestion: sorrendezős, 4 elem helyes sorrendbe rakása
======================================================================*/

#ifndef QUESTION_H
#define QUESTION_H

#include <string>
#include <vector>

//! ---------- ALAP OSZTÁLY ----------

// CÉL: Absztrakt alap, amelyből minden kérdéstípus örököl
class Question {
protected:
    std::string question;             // kérdés szövege
    std::string category;             // téma (pl. "KONYHA")
    std::string correctAnswer;        // helyes válasz kódja (pl. "B" vagy "CDAB")
    std::vector<std::string> answers; // válaszlehetőségek [A, B, C, D]

    /*
        CÉL: Közös kérdésadatok inicializálása
        BE: q       - kérdés szövege
            cat     - kategória
            correct - helyes válasz kódja
            ans     - válaszlehetőségek (4 elem)
    */
    Question(const std::string& q,
             const std::string& cat,
             const std::string& correct,
             const std::vector<std::string>& ans);

public:
    virtual ~Question();

    // CÉL: Kérdés + válaszlehetőségek kiírása (teljesen virtuális – típusonként más formátum)
    virtual void display() const = 0;

    // CÉL: Játékos válaszának ellenőrzése (teljesen virtuális – típusonként más logika)
    virtual bool checkAnswer(const std::string& input) const = 0;

    const std::string&              getQuestion()     const; // kérdés szöveg lekérése
    const std::string&              getCategory()     const; // kategória lekérése
    const std::string&              getCorrectAnswer()const; // helyes válasz kód lekérése
    const std::vector<std::string>& getAnswers()      const; // összes válasz lekérése
};

//! ---------- FELELETVÁLASZTÓS KÉRDÉS ----------

// CÉL: Feleletválasztós kérdés nehézségi szinttel (1-12)
class ChooseQuestion : public Question {
private:
    int difficulty; // nehézségi szint; egyezik a játékszinttel (1 = 1. kérdés szintje)

public:
    /*
        CÉL: Feleletválasztós kérdés létrehozása
        BE: diff    - nehézségi szint (1-12)
            q       - kérdés szövege
            cat     - kategória
            correct - helyes válasz betűje (pl. "C")
            ans     - 4 válaszlehetőség [A, B, C, D]
    */
    ChooseQuestion(int diff,
                   const std::string& q,
                   const std::string& cat,
                   const std::string& correct,
                   const std::vector<std::string>& ans);

    ~ChooseQuestion() override;

    // CÉL: Kérdés kiírása segítség nélkül (meghívja a displayWithHints-et hint nélkül)
    void display() const override;

    /*
        CÉL: Kérdés kiírása 50:50 elrejtéssel és opcionális közönség %-kal
        BE: hidden0, hidden1 - elrejtett válaszindexek (-1 = nem rejtett)
            audience         - közönség szavazatok tömbje [A,B,C,D], nullptr ha nem aktív
    */
    void displayWithHints(int hidden0, int hidden1, const int* audience) const;

    // CÉL: Egybetűs válasz ellenőrzése (pl. "B" == correctAnswer)
    bool checkAnswer(const std::string& input) const override;

    // CÉL: Nehézségi szint lekérése (1-12)
    int getDifficulty() const;
};

//! ---------- SORRENDEZŐS KÉRDÉS ----------

// CÉL: Sorrendezős kérdés – 4 elemet kell helyes sorrendbe rakni
class OrderQuestion : public Question {
public:
    /*
        CÉL: Sorrendezős kérdés létrehozása
        BE: q       - kérdés szövege (mit kell sorba rendezni)
            cat     - kategória
            correct - helyes sorrend kódként (pl. "CDAB")
            ans     - 4 rendezendő elem [A, B, C, D]
    */
    OrderQuestion(const std::string& q,
                  const std::string& cat,
                  const std::string& correct,
                  const std::vector<std::string>& ans);

    ~OrderQuestion() override;

    // CÉL: Kérdés és elemek kiírása [SORRENDEZŐS] fejléccel
    void display() const override;

    // CÉL: 4 betűs sorrend válasz ellenőrzése (pl. "CDAB" == correctAnswer)
    bool checkAnswer(const std::string& input) const override;
};

#endif
