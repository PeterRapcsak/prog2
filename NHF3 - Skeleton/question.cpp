#include "question.h"

#include <iostream>

/*======================================================================
	question.cpp - Skeleton forrásfájl
----------------------------------------------------------------------
	CÉL:
	 - Fordítható .cpp egység a kérdés osztályokhoz
	 - A deklarációk a headerben vannak
======================================================================*/

//! ---------- MEGJEGYZÉS ----------
// CÉL: Ebben a fázisban itt nincs végleges implementáció

Question::Question(const std::string& q,
				   const std::string& cat,
				   const std::string& correct,
				   const std::vector<std::string>& ans)
	: question(q), category(cat), correctAnswer(correct), answers(ans) {}

Question::~Question() = default;

const std::string& Question::getQuestion() const {
	return question;
}

const std::string& Question::getCategory() const {
	return category;
}

const std::string& Question::getCorrectAnswer() const {
	return correctAnswer;
}

const std::vector<std::string>& Question::getAnswers() const {
	return answers;
}

ChooseQuestion::ChooseQuestion(int diff,
							   const std::string& q,
							   const std::string& cat,
							   const std::string& correct,
							   const std::vector<std::string>& ans)
	: Question(q, cat, correct, ans), difficulty(diff) {}

ChooseQuestion::~ChooseQuestion() = default;

void ChooseQuestion::display() const {
	std::cout << "[Feleletválasztós] " << question << '\n';
}

bool ChooseQuestion::checkAnswer(const std::string& input) const {
	return input == correctAnswer;
}

int ChooseQuestion::getDifficulty() const {
	return difficulty;
}

OrderQuestion::OrderQuestion(const std::string& q,
							 const std::string& cat,
							 const std::string& correct,
							 const std::vector<std::string>& ans)
	: Question(q, cat, correct, ans) {}

OrderQuestion::~OrderQuestion() = default;

void OrderQuestion::display() const {
	std::cout << "[Sorrendezős] " << question << '\n';
}

bool OrderQuestion::checkAnswer(const std::string& input) const {
	return input == correctAnswer;
}