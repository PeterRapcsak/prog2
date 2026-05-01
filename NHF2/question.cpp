/*======================================================================
    question.cpp - Kérdés osztályok implementáció
----------------------------------------------------------------------
    CÉL:
     - Question absztrakt osztály metódusai
     - ChooseQuestion és OrderQuestion implementáció
======================================================================*/

#include "question.h"
#include <iostream> // konzol kiírás

//! ---------- ALAP OSZTÁLY ----------

/*
    CÉL: Közös kérdés adatok inicializálása
    BE:
     - q: kérdés szövege
     - cat: kategória
     - correct: helyes válasz kód (pl "B" vagy "ABCD")
     - ans: válaszlehetőségek vektora
*/
Question::Question(const std::string& q,
                   const std::string& cat,
                   const std::string& correct,
                   const std::vector<std::string>& ans)
    : question(q), category(cat), correctAnswer(correct), answers(ans) {}

Question::~Question() {}

// CÉL: Kérdés szövegének lekérése
const std::string& Question::getQuestion() const {
    return question;
}

// CÉL: Kategória lekérése
const std::string& Question::getCategory() const {
    return category;
}

// CÉL: Helyes válasz kódjának lekérése
const std::string& Question::getCorrectAnswer() const {
    return correctAnswer;
}

// CÉL: Válaszlehetőségek lekérése
const std::vector<std::string>& Question::getAnswers() const {
    return answers;
}

//! ---------- FELELETVÁLASZTÓS KÉRDÉS ----------

/*
    CÉL: Feleletválasztós kérdés létrehozása
    BE:
     - diff: nehézségi szint
     - q: kérdés szövege
     - cat: kategória
     - correct: helyes válasz (pl "A")
     - ans: 4 válaszlehetőség
*/
ChooseQuestion::ChooseQuestion(int diff,
                               const std::string& q,
                               const std::string& cat,
                               const std::string& correct,
                               const std::vector<std::string>& ans)
    : Question(q, cat, correct, ans), difficulty(diff) {}

ChooseQuestion::~ChooseQuestion() {}

/*
    CÉL: Feleletválasztós kérdés kiírása
*/
void ChooseQuestion::display() const {
    std::cout << "  Téma: " << category << "\n\n";
    std::cout << "  " << question << "\n\n";
    if (answers.size() >= 4) {
        std::cout << "  A: " << answers[0] << "\n";
        std::cout << "  B: " << answers[1] << "\n";
        std::cout << "  C: " << answers[2] << "\n";
        std::cout << "  D: " << answers[3] << "\n";
    }
}

/*
    CÉL: Válasz ellenőrzése
    BE: input - játékos válasza
    KI: true ha helyes, false ha nem
*/
bool ChooseQuestion::checkAnswer(const std::string& input) const {
    return input == correctAnswer;
}

// CÉL: Nehézségi szint lekérése
int ChooseQuestion::getDifficulty() const {
    return difficulty;
}

//! ---------- SORRENDEZŐS KÉRDÉS ----------

/*
    CÉL: Sorrendezős kérdés létrehozása
    BE:
     - q: kérdés szövege
     - cat: kategória
     - correct: helyes sorrend (pl "ABCD")
     - ans: 4 rendezendő elem
*/
OrderQuestion::OrderQuestion(const std::string& q,
                             const std::string& cat,
                             const std::string& correct,
                             const std::vector<std::string>& ans)
    : Question(q, cat, correct, ans) {}

OrderQuestion::~OrderQuestion() {}

/*
    CÉL: Sorrendezős kérdés kiírása
*/
void OrderQuestion::display() const {
    std::cout << "  [SORRENDEZŐS] Téma: " << category << "\n\n";
    std::cout << "  " << question << "\n\n";
    if (answers.size() >= 4) {
        std::cout << "  A: " << answers[0] << "\n";
        std::cout << "  B: " << answers[1] << "\n";
        std::cout << "  C: " << answers[2] << "\n";
        std::cout << "  D: " << answers[3] << "\n";
    }
}

/*
    CÉL: Sorrendezős válasz ellenőrzése
    BE: input - játékos által megadott sorrend
    KI: true ha helyes, false ha nem
*/
bool OrderQuestion::checkAnswer(const std::string& input) const {
    return input == correctAnswer;
}
