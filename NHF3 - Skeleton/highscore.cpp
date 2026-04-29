#include "highscore.h"

#include <algorithm>
#include <iostream>

/*======================================================================
	highscore.cpp - Skeleton forrásfile
----------------------------------------------------------------------
	CÉL:
	 - Fordítható .cpp egység a dicsőséglista osztályokhoz
	 - A deklarációk a headerben vannak
======================================================================*/

//! ---------- MEGJEGYZÉS ----------
// CÉL: Skeletonban még nincs végleges implementáció

HighScoreEntry::HighScoreEntry(const std::string& name,
							   const std::string& date,
							   int prize)
	: name(name), date(date), prize(prize) {}

const std::string& HighScoreEntry::getName() const {
	return name;
}

const std::string& HighScoreEntry::getDate() const {
	return date;
}

int HighScoreEntry::getPrize() const {
	return prize;
}

bool HighScoreEntry::operator>(const HighScoreEntry& rhs) const {
	return prize > rhs.prize;
}

HighScoreTable::HighScoreTable(const std::string& filename)
	: filename(filename) {}

void HighScoreTable::load() {
	entries.clear();
}

void HighScoreTable::save() const {
	// Skeleton: itt később fájlba mentés lesz.
}

void HighScoreTable::add(const std::string& name, int prize) {
	entries.emplace_back(name, "2026-04-29", prize);
}

void HighScoreTable::display() const {
	std::vector<HighScoreEntry> copy = entries;
	std::sort(copy.begin(), copy.end(), [](const HighScoreEntry& a, const HighScoreEntry& b) {
		return a.getPrize() > b.getPrize();
	});

	std::cout << "\n--- Dicsőséglista (skeleton) ---\n";
	for (const HighScoreEntry& e : copy) {
		std::cout << e.getName() << " | " << e.getPrize() << " | " << e.getDate() << '\n';
	}
}