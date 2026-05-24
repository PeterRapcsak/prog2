/*======================================================================
    question.h - Kérdés osztályok deklarációja
----------------------------------------------------------------------
    FELADAT:
     - Question: közös absztrakt alap osztály
         - kérdés szövege
         - kategória
         - helyes válasz
         - válaszlehetőségek

     - ChooseQuestion: feleletválasztós kérdés
         - nehézségi szint
         - kérdés kiírása simán vagy 50:50 / közönség segítséggel
         - válasz ellenőrzése

     - OrderQuestion: sorrendezős kérdés
         - rendezendő elemek eltárolása
         - 4 betűs sorrend ellenőrzése
         - itt nincs segítség
======================================================================*/

#ifndef QUESTION_H
#define QUESTION_H

#include <string>
#include <vector>

//! ---------- ALAP OSZTÁLY ----------

// CÉL: Közös absztrakt alap minden kérdéstípushoz, NEM PÉLDÁNYOSÍTJUK
class Question {
protected:
    std::string question;             // kérdés szövege
    std::string category;             // kategória
    std::string correctAnswer;        // helyes válasz, pld: "B" vagy "CDAB"
    std::vector<std::string> answers; // válaszlehetőségek / rendezendő elemek

    /*
        CÉL: Közös kérdésadatok eltárolása
        MEGJEGYZÉS:
            Az alap osztály tárolja azokat az adatokat,
            amik mindkét kérdéstípusnál kellenek
    */
    Question(const std::string& q,                  // kérdés szövege
             const std::string& cat,                // kategória
             const std::string& correct,            // helyes válasz
             const std::vector<std::string>& ans);  // válaszlehetőségek vektora

public:
    // Virtuális destruktor, mert ebből örökölnek más osztályok
    virtual ~Question();

    // CÉL: Kérdés kiírása
    virtual void display() const = 0;

    // CÉL: Játékos válaszának ellenőrzése
    virtual bool checkAnswer(const std::string& input) const = 0;

    // CÉL: Kérdés lekérése
    const std::string& getQuestion() const;

    // CÉL: Kategória lekérése
    const std::string& getCategory() const;

    // CÉL: Helyes válasz lekérése
    const std::string& getCorrectAnswer() const;

    // CÉL: Válaszlehetőségek lekérése
    const std::vector<std::string>& getAnswers() const;
};


//! ---------- FELELETVÁLASZTÓS KÉRDÉS ----------

// CÉL: Feleletválasztós kérdés nehézségi szinttel
class ChooseQuestion : public Question {
private:
    int difficulty; // nehézségi szint, 1-től 12-ig

public:
    /*
        CÉL: Feleletválasztós kérdés létrehozása
        MEGJEGYZÉS:
            A közös adatokat az ősosztály kapja meg,
            a nehézséget pedig ez az osztály tárolja.
    */
    ChooseQuestion(int diff,                                // nehézségi szint, 1-től 12-ig
                   const std::string& q,                    // kérdés szövege
                   const std::string& cat,                  // kategória
                   const std::string& correct,              // helyes válasz betűje, pld: "A"
                   const std::vector<std::string>& ans);    // válaszlehetőség: A, B, C, D

    // CÉL: Feleletválasztós kérdés felszabadítása
    ~ChooseQuestion() override;

    // CÉL: Feleletválasztós kérdés sima kiírása
    void display() const override;

    /*
        CÉL: Feleletválasztós kérdés kiírása segítségekkel
        MEGJEGYZÉS:
            hidden0 / hidden1 az elrejtett válaszok indexei.
            audience lehet nullptr, ilyenkor nem írunk ki százalékot.
    */
    void displayWithHints(
        int hidden0,
        int hidden1,
        const int* audience
    ) const;

    // CÉL: Feleletválasztós válasz ellenőrzése
    bool checkAnswer(const std::string& input) const override;

    // CÉL: Nehézségi szint lekérése
    int getDifficulty() const;
};


//! ---------- SORRENDEZŐS KÉRDÉS ----------

// CÉL: Sorrendezős kérdés, ahol 4 elemet kell helyes sorrendbe rakni
class OrderQuestion : public Question {
public:
    /*
        CÉL: Sorrendezős kérdés létrehozása
        MEGJEGYZÉS:
            Itt nincs difficulty, mert a sorrendezős kérdések
            nincsenek külön szintekhez kötve.
    */
    OrderQuestion(const std::string& q,                 // kérdés szövege
                  const std::string& cat,               // kategória
                  const std::string& correct,           // helyes sorrend, pld: "BDAC"
                  const std::vector<std::string>& ans); // 4 rendezendő elem

    // CÉL: Sorrendezős kérdés felszabadítása
    ~OrderQuestion() override;

    // CÉL: Sorrendezős kérdés kiírása
    void display() const override;

    // CÉL: Sorrendezős válasz ellenőrzése
    bool checkAnswer(const std::string& input) const override;
};

#endif