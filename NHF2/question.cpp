/*======================================================================
    question.cpp - Kérdés objektumok megvalósítása
----------------------------------------------------------------------
    FELADAT:
     - Question: absztrakt alap osztály konstruktora és getter-ek

     - ChooseQuestion: feleletválasztós (nehézség 1-12)
         - display()          -> hint nélküli kiírás
         - displayWithHints() -> 50:50 elrejtéssel, közönség %-kal
         - checkAnswer()      -> egybetűs válasz ellenőrzése

     - OrderQuestion: sorrendezős
         - display()      -> kérdés és rendezendő elemek kiírása
         - checkAnswer()  -> 4 betűs sorrend ellenőrzése
======================================================================*/

#include "question.h"
#include "colors.h"
#include <iostream> // cout

// Egyesével importáljuk, hogy ne kelljen mindenhol std::-t írni
using std::string;
using std::vector;
using std::cout;

//! ---------- ALAP OSZTÁLY ----------

/*
    CÉL: Közös kérdésadatok inicializálása (minden leszármazott ezt hívja)
    BE: q       - kérdés szövege
        cat     - kategória (pl. "KONYHA")
        correct - helyes válasz kódja (pl. "B" vagy "CDAB")
        ans     - válaszlehetőségek vektora (4 elem)
*/
Question::Question(
    const string& q,
    const string& cat,
    const string& correct,
    const vector<string>& ans)
    : question(q),
      category(cat),
      correctAnswer(correct),
      answers(ans) {}

Question::~Question() {}

const string&         Question::getQuestion()      const { return question;      }
const string&         Question::getCategory()      const { return category;      }
const string&         Question::getCorrectAnswer() const { return correctAnswer; }
const vector<string>& Question::getAnswers()       const { return answers;       }

//! ---------- FELELETVÁLASZTÓS KÉRDÉS ----------

/*
    CÉL: Feleletválasztós kérdés létrehozása
    BE: diff    - nehézségi szint (1-12, egyezik a játékszinttel)
        q       - kérdés szövege
        cat     - kategória
        correct - helyes válasz betűje (pl. "C")
        ans     - 4 válaszlehetőség [A, B, C, D]
*/
ChooseQuestion::ChooseQuestion(
    int diff,
    const string& q,
    const string& cat,
    const string& correct,
    const vector<string>& ans)
    : Question(q, cat, correct, ans), difficulty(diff) {}

ChooseQuestion::~ChooseQuestion() {}

// CÉL: Kérdés kiírása hint nélkül – meghívja a displayWithHints-et semleges paraméterekkel
void ChooseQuestion::display() const {
    displayWithHints(-1, -1, nullptr); // -1 = nincs elrejtve, nullptr = nincs közönség %
}

/*
    CÉL: Kérdés kiírása 50:50 elrejtéssel és opcionális közönség %-kal
    BE: hidden0, hidden1 - az elrejtett válaszok indexei (0-3), -1 ha nincs
        audience         - közönség szavazatok tömbje [A,B,C,D] %-ban,
                           nullptr ha a közönség segítség nem aktív
    MEGJEGYZÉS: Elrejtett opciók teljesen kimaradnak (nem halványulnak, hanem eltűnnek)
*/
void ChooseQuestion::displayWithHints(int hidden0, int hidden1, const int* audience) const {
    cout << "  Téma: " << category << "\n\n";
    cout << "  " << Color::BOLD_YELLOW << question << Color::RESET << "\n\n";

    if (answers.size() >= 4) {
        for (int i = 0; i < 4; i++) {
            if (i == hidden0 || i == hidden1) {
                continue; // 50:50 elrejtett válasz kihagyása
            }

            cout << "  "
                 << Color::BOLD_YELLOW
                 << "[" << static_cast<char>('A' + i) << "]"
                 << Color::RESET;

            // Közönség százalék kiírása, ha aktív
            if (audience != nullptr) {
                cout << Color::BOLD_GREEN
                     << " [" 
                     << audience[i] << "%";

                if (audience[i] < 10) {
                    cout << " ";
                }

                cout << "]" << Color::RESET;
            }

            cout << " " << answers[static_cast<std::size_t>(i)] << "\n";
        }
    }
}

/*
    CÉL: Egybetűs feleletválasztós válasz ellenőrzése
    BE: input - a játékos válasza nagybetűsítve (pl. "B")
    KI: true ha egyezik a helyes válasszal
*/
bool ChooseQuestion::checkAnswer(const string& input) const {
    return input == correctAnswer;
}

// CÉL: Nehézségi szint lekérése (1-12)
int ChooseQuestion::getDifficulty() const { return difficulty; }

//! ---------- SORRENDEZŐS KÉRDÉS ----------

/*
    CÉL: Sorrendezős kérdés létrehozása
    BE: q       - kérdés szövege (mit kell sorba rendezni)
        cat     - kategória
        correct - helyes sorrend betűkódként (pl. "CDAB")
        ans     - 4 rendezendő elem [A, B, C, D]
*/
OrderQuestion::OrderQuestion(
    const string& q,
    const string& cat,
    const string& correct,
    const vector<string>& ans)
    : Question(q, cat, correct, ans) {}

OrderQuestion::~OrderQuestion() {}

// CÉL: Sorrendezős kérdés kiírása [SORRENDEZŐS] típusjelzéssel és a 4 rendezendő elemmel
void OrderQuestion::display() const {
    cout << "  " << Color::BOLD_CYAN << "[SORRENDEZŐS]" << Color::RESET
         << " Téma: " << category << "\n\n";
    cout << "  " << Color::BOLD_YELLOW << question << Color::RESET << "\n\n";

    if (answers.size() >= 4) {
        for (int i = 0; i < 4; ++i) {
            cout << "  " << Color::BOLD_YELLOW << "[" << static_cast<char>('A' + i) << "]"
                 << Color::RESET << " " << answers[static_cast<std::size_t>(i)] << "\n";
        }
    }
}

/*
    CÉL: Sorrendezős válasz ellenőrzése
    BE: input - a játékos sorrendje nagybetűsítve (pl. "CDAB")
    KI: true ha egyezik a helyes sorrenddel
*/
bool OrderQuestion::checkAnswer(const string& input) const {
    return input == correctAnswer;
}
