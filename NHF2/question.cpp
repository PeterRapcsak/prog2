/*======================================================================
    question.cpp - Kérdés objektumok megvalósítása
----------------------------------------------------------------------
    FELADAT:
     - Question absztrakt osztály konstruktora, getter-ek

     - ChooseQuestion: feleletválasztós (+ nehézség 1-12)
    	 - display() -> Kiírja a krérdést és a válaszlehetőségeket
         - checkAnswer() -> Ellenőrzi a választ (1 betű)

     - OrderQuestion: sorrendezős
         - display() -> Kiírja a sorrendezős kérdést
         - checkAnswer() -> Ellenőrzi a választ (4 betű sorrend)
======================================================================*/

#include "question.h"
#include "colors.h"
#include <iostream> // konzol kiírás

using std::string;
using std::vector;
using std::cout;
using std::endl;

//! ---------- ALAP OSZTÁLY ----------

// CÉL: Közös kérdés adatok inicializálása

Question::Question(
	const string& q,            // kérdés szövege
	const string& cat,          // kategória
    const string& correct,      // helyes válasz (pld: "B" vagy "ABCD")
	const vector<string>& ans)  // válaszlehetőségek vektora
    : question(q),
	category(cat),
	correctAnswer(correct),
	answers(ans) {}

Question::~Question() {}

// CÉL: Kérdés szövegének lekérése
const string& Question::getQuestion() const {
    return question;
}

// CÉL: Kategória lekérése
const string& Question::getCategory() const {
    return category;
}

// CÉL: Helyes válasz kódjának lekérése
const string& Question::getCorrectAnswer() const {
    return correctAnswer;
}

// CÉL: Válaszlehetőségek lekérése
const vector<string>& Question::getAnswers() const {
    return answers;
}

//! ---------- FELELETVÁLASZTÓS KÉRDÉS ----------

ChooseQuestion::ChooseQuestion(
	int diff,                   // nehézségi szint
	const string& q,            // kérdés szövege
	const string& cat,          // kategória
    const string& correct,      // helyes válasz (pld: "A")
	const vector<string>& ans)  // 4 válaszlehetőség
    : Question(q, cat, correct, ans), difficulty(diff) {}

ChooseQuestion::~ChooseQuestion() {}

//CÉL: Feleletválasztós kérdés kiírása
void ChooseQuestion::display() const {
    displayWithHints(-1, -1, nullptr);
}

// CÉL: Kiírás elrejtett válaszokkal és közönség %-kal (nullptr = nem aktív)
void ChooseQuestion::displayWithHints(int hidden0, int hidden1, const int* audience) const {
    cout << "  Téma: " << category << "\n\n";
    cout << "  " << Color::BOLD_YELLOW << question << Color::RESET << "\n\n";
    if (answers.size() >= 4) {
        for (int i = 0; i < 4; ++i) {
            if (i == hidden0 || i == hidden1) continue;
            cout << "  " << Color::BOLD_YELLOW << "[" << static_cast<char>('A' + i) << "]"
                 << Color::RESET;
            if (audience != nullptr) {
                std::string pct = std::to_string(audience[i]);
                while (pct.size() < 3) pct = " " + pct;
                cout << " " << Color::CYAN << "[" << pct << "%]" << Color::RESET;
            }
            cout << " " << answers[static_cast<std::size_t>(i)] << "\n";
        }
    }
}

// CÉL: Válasz bekérése, compare, return
bool ChooseQuestion::checkAnswer(const string& input) const {
    return input == correctAnswer;
}

// CÉL: Nehézségi szint lekérése
int ChooseQuestion::getDifficulty() const {
    return difficulty;
}

//! ---------- SORRENDEZŐS KÉRDÉS ----------

OrderQuestion::OrderQuestion(
    const string& q,            // kérdés szövege
    const string& cat,          // kategória
    const string& correct,      // helyes sorrend (pl "ABCD")
    const vector<string>& ans)  // 4 rendezendő elem
    : Question(q, cat, correct, ans) {}

OrderQuestion::~OrderQuestion() {}

// CÉL: Sorrendezős kérdés kiírása
void OrderQuestion::display() const {
    cout << "  " << Color::BOLD_CYAN << "[SORRENDEZŐS]" << Color::RESET
         << " Téma: " << Color::BOLD_CYAN << category << Color::RESET << "\n\n";
    cout << "  " << Color::BOLD_YELLOW << question << Color::RESET << "\n\n";
    if (answers.size() >= 4) {
        for (int i = 0; i < 4; ++i) {
            cout << "  " << Color::BOLD_YELLOW << "[" << static_cast<char>('A' + i) << "]"
                 << Color::RESET << " " << answers[static_cast<std::size_t>(i)] << "\n";
        }
    }
}

// CÉL: Válasz bekérése, compare, return
bool OrderQuestion::checkAnswer(const string& input) const {
    return input == correctAnswer;
}
