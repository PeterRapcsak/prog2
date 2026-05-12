/*======================================================================
    question.cpp - Kérdés objektumok megvalósítása
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
         - rendezendő elemek kiírása
         - 4 betűs sorrend ellenőrzése
         - itt nincs segítség
======================================================================*/

#include "question.h"
#include "colors.h"

#include <iostream> // konzol kiírás

using std::string;
using std::vector;
using std::cout;

//! ---------- ALAP OSZTÁLY ----------


// CÉL: Közös kérdés adatok eltárolása, NEM PÉLDÁNYOSÍTJUK
Question::Question(
    const string& q,            // kérdés szövege
    const string& cat,          // kategória
    const string& correct,      // helyes válasz
    const vector<string>& ans)  // válaszlehetőségek vektora
    : question(q),
      category(cat),
      correctAnswer(correct),
      answers(ans)
{}

// Virtuális destruktor, mert ebből örökölnek más osztályok
Question::~Question() {}

// CÉL: Kérdés lekérése
const string& Question::getQuestion() const {
    return question;
}

// CÉL: Kategória lekérése
const string& Question::getCategory() const {
    return category;
}

// CÉL: Helyes válasz lekérése
const string& Question::getCorrectAnswer() const {
    return correctAnswer;
}

// CÉL: Válaszlehetőségek lekérése
const vector<string>& Question::getAnswers() const {
    return answers;
}


//! ---------- FELELETVÁLASZTÓS KÉRDÉS ----------

/*
    CÉL: Feleletválasztós kérdés létrehozása
    MEGJEGYZÉS:
        A közös adatokat az ősosztály kapja meg,
        a nehézséget pedig ez az osztály tárolja.
*/
ChooseQuestion::ChooseQuestion(
    int diff,                   // nehézségi szint, 1-től 12-ig
    const string& q,            // kérdés szövege
    const string& cat,          // kategória
    const string& correct,      // Helyes válasz betűje, pld: "A"
    const vector<string>& ans)  // 4 válaszlehetőség: A, B, C, D        
    : Question(q, cat, correct, ans),
      difficulty(diff)
{}

ChooseQuestion::~ChooseQuestion() {}

// CÉL: Feleletválasztós kérdés sima kiírása
void ChooseQuestion::display() const {

    // Ugyanazt a kiíró függvényt használjuk, csak segítségek nélkül
    // -1      -> nincs elrejtett válasz
    // nullptr -> nincs közönség százalék
    displayWithHints(-1, -1, nullptr);
}
/*
    CÉL: Feleletválasztós kérdés kiírása segítségekkel
    MEGJEGYZÉS:
        hidden0 / hidden1 az elrejtett válaszok indexei.
        audience lehet nullptr, ilyenkor nem írunk ki százalékot.
*/
void ChooseQuestion::displayWithHints(
    int hidden0,
    int hidden1,
    const int* audience) const {

    cout << "  Téma: " << category << "\n\n";
    cout << "  " << Color::BOLD_YELLOW << question << Color::RESET << "\n\n";

    // Csak akkor írjuk ki, ha tényleg megvan a 4 válasz
    if (answers.size() >= 4) {

        // Végigmegyünk az A, B, C, D válaszokon
        for (int i = 0; i < 4; i++) {

            // Ha ezt az opciót elrejtette az 50:50, akkor kihagyjuk
            if (i == hidden0 || i == hidden1) {
                continue;
            }

            // Válasz betűjének kiírása
            // 0 + 'A' -> A, 
            // 1 + 'A' -> B, stb.
            cout << "  "
                 << Color::BOLD_YELLOW
                 << "[" << static_cast<char>('A' + i) << "]"
                 << Color::RESET;

            // Ha van közönség segítség, akkor kiírjuk mellé a százalékot is
            if (audience != nullptr) {
                cout << Color::BOLD_GREEN
                     << " ["
                     << audience[i] << "%";

                // Csak azért, hogy a 0-9% is szép legyen a konzolban
                if (audience[i] < 10) {
                    cout << " ";
                }

                cout << "]" << Color::RESET;
            }

            // Maga a válasz szövege
            cout << " " << answers[static_cast<std::size_t>(i)] << "\n";
        }
    }
}

// CÉL: Feleletválasztós válasz ellenőrzése
bool ChooseQuestion::checkAnswer(const string& input) const {
    return input == correctAnswer;
}

// CÉL: Nehézségi szint lekérése
int ChooseQuestion::getDifficulty() const {
    return difficulty;
}


//! ---------- SORRENDEZŐS KÉRDÉS ----------

/*
    CÉL: Sorrendezős kérdés létrehozása
    MEGJEGYZÉS:
        Itt nincs difficulty, mert a sorrendezős kérdések
        nincsenek külön szintekhez kötve. (Mindegyik elég nehéz)
*/
OrderQuestion::OrderQuestion(
    const string& q,            // kérdés szövege
    const string& cat,          // kategória
    const string& correct,      // helyes sorrend, pld: "BDAC"
    const vector<string>& ans)  // 4 rendezendő elem
    : Question(q, cat, correct, ans)
{}

OrderQuestion::~OrderQuestion() {}

// CÉL: Sorrendezős kérdés kiírása
void OrderQuestion::display() const {
    cout << "  "
         << Color::BOLD_CYAN << "[SORRENDEZŐS]" << Color::RESET
         << " Téma: " << category << "\n\n";

    cout << "  " << Color::BOLD_YELLOW << question << Color::RESET << "\n\n";

    // Csak akkor írjuk ki, ha van legalább 4 elem
    if (answers.size() >= 4) {

        // Végigmegyünk az A, B, C, D elemeken
        for (int i = 0; i < 4; i++) {

            cout << "  "
                 << Color::BOLD_YELLOW
                 << "[" << static_cast<char>('A' + i) << "]"
                 << Color::RESET
                 << " " << answers[static_cast<std::size_t>(i)] << "\n";
        }
    }
}

// CÉL: Sorrendezős válasz ellenőrzése
bool OrderQuestion::checkAnswer(const string& input) const {
    return input == correctAnswer;
}