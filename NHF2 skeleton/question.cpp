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

#include <iostream>
#include "question.h"

using std::string;
using std::vector;

// CÉL: Közös kérdés adatok inicializálása
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

// CÉL: Kérdés szövegének lekérése
const string& Question::getQuestion() const { return question; }
// CÉL: Kategória lekérése
const string& Question::getCategory() const { return category; }
// CÉL: Helyes válasz kódjának lekérése
const string& Question::getCorrectAnswer() const { return correctAnswer; }
// CÉL: Válaszok lekérése
const vector<string>& Question::getAnswers() const { return answers; }

// --- ChooseQuestion ---
// CÉL: Feleletválasztós kérdés létrehozása
ChooseQuestion::ChooseQuestion(
	int diff,
	const string& q,
	const string& cat,
    const string& correct,
	const vector<string>& ans)
    : Question(q, cat, correct, ans), difficulty(diff) {}

ChooseQuestion::~ChooseQuestion() {}

// CÉL: Feleletválasztós formátumú kiírás
void ChooseQuestion::display() const {
    std::cout << question << std::endl;
}

// CÉL: Egybetűs válasz ellenőrzése
bool ChooseQuestion::checkAnswer(const string&) const {
    return true;
}

// CÉL: Nehézségi szint lekérése
int ChooseQuestion::getDifficulty() const { return difficulty; }

// --- OrderQuestion ---
// CÉL: Sorrendezős kérdés létrehozása
OrderQuestion::OrderQuestion(
	const string& q,	
	const string& cat,
    const string& correct,
	const vector<string>& ans)
    : Question(q, cat, correct, ans) {}

OrderQuestion::~OrderQuestion() {}

// CÉL: Sorrendezős formátumú kiírás
void OrderQuestion::display() const {
    std::cout << "[TODO] OrderQuestion::display()" << std::endl;
}

// CÉL: 4 betűs sorrend-válasz ellenőrzése
bool OrderQuestion::checkAnswer(const string&) const {
    return true;
}

//! Ez a sok komment most csúnyán néz ki, és még pontosan ugyanaz mint a game.h-ban
//! de majd úgy írom meg a metódusokat, hogy pld írok olyan fieldeket is hogy BE: és KI:
//! hogy teljesen egyértelmű legyen hogy pontosan mi mit csinál.  :D
