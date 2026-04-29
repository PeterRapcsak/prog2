#include "filemanager.h"

#include <fstream>

/*======================================================================
	filemanager.cpp - Skeleton forrásfájl
----------------------------------------------------------------------
	CÉL:
	 - Fordítható .cpp egység a fájlkezelő osztályhoz
	 - A deklarációk a headerben vannak
======================================================================*/

//! ---------- MEGJEGYZÉS ----------
// CÉL: Ebben a fázisban itt nincs végleges implementáció

std::vector<ChooseQuestion> FileManager::loadChooseQuestions(const std::string& filename) {
	(void)filename;
	return {};
}

std::vector<OrderQuestion> FileManager::loadOrderQuestions(const std::string& filename) {
	(void)filename;
	return {};
}

bool FileManager::fileExists(const std::string& path) {
	std::ifstream f(path);
	return f.good();
}