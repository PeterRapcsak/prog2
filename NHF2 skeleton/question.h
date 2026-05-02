/*======================================================================
    question.h - Header file
----------------------------------------------------------------------
    CÉL:
     - Kérdés osztályok deklarációja (absztrakt + leszármazottak)
======================================================================*/

#ifndef QUESTION_H
#define QUESTION_H

#include <string>
#include <vector>

//! ---------- KÉRDÉS OSZTÁLYOK ----------

// CÉL: Absztrakt osztály az összes kérdés típushoz
class Question {
protected:
    std::string question;               // Kérdés szövege
    std::string category;               // Kérdés kategóriája
    std::string correctAnswer;          // Helyes válasz
    std::vector<std::string> answers;   // Válaszlehetőségek

    // CÉL: Közös kérdés adatok inicializálása
    Question(const std::string& q,
             const std::string& cat,
             const std::string& correct,
             const std::vector<std::string>& ans);

public:
    virtual ~Question();

    // CÉL: Kérdés + válaszlehetőségek kiírása
    //  - Teljesen virtuális, mert a két kérdéstípus formátuma eltérő, ezért ott kell majd implementálni
    virtual void display() const = 0;

    // CÉL: Játékos válaszának ellenőrzése
    virtual bool checkAnswer(const std::string& input) const = 0;

    // CÉL: Kérdés szövegének lekérése
    const std::string& getQuestion() const;

    // CÉL: Kategória lekérése
    const std::string& getCategory() const;

    // CÉL: Helyes válasz kódjának lekérése
    const std::string& getCorrectAnswer() const;

    // CÉL: Válaszok lekérése
    const std::vector<std::string>& getAnswers() const;
};

// CÉL: Feleletválasztós kérdés nehézségi szinttel
class ChooseQuestion : public Question {
private:
    int difficulty;

public:
    // CÉL: Feleletválasztós kérdés létrehozása
    ChooseQuestion(int diff,                                // Kérdés nehézsége (1-12)
                   const std::string& q,                    // Kérdés szövege
                   const std::string& cat,                  // Kérdés kategóriája
                   const std::string& correct,              // Helyes válasz ("B")
                   const std::vector<std::string>& ans);    // Válaszlehetőségek

    ~ChooseQuestion() override;

    // CÉL: Feleletválasztós formátumú kiírás
    void display() const override;

    // CÉL: Egybetűs válasz ellenőrzése
    bool checkAnswer(const std::string& input) const override;

    // CÉL: Nehézségi szint lekérése
    int getDifficulty() const;
};

// CÉL: Sorrendezős kérdés
class OrderQuestion : public Question {
public:
    // CÉL: Sorrendezős kérdés létrehozása
    OrderQuestion(const std::string& q,                    // Kérdés szövege
                  const std::string& cat,                  // Kérdés kategóriája
                  const std::string& correct,              // Helyes válasz ("CDAB")
                  const std::vector<std::string>& ans);    // Válaszlehetőségek

    ~OrderQuestion() override;

    // CÉL: Sorrendezős formátumú kiírás
    void display() const override;

    // CÉL: 4 betűs sorrend-válasz ellenőrzése
    bool checkAnswer(const std::string& input) const override;
};

#endif