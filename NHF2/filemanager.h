/*======================================================================
    filemanager.h - Header file
----------------------------------------------------------------------
    CÉL:
     - CSV filekezelő osztály deklarációja
======================================================================*/

#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <string>
#include <vector>

#include "question.h"

//! ---------- FÁJLKEZELÉS ----------

// CÉL: CSV állományok beolvasása
class FileManager {
public:
    // CÉL: Feleletválasztós kérdések betöltése CSV-ből
    // FORMA: "Nehézség;Kérdés;A;B;C;D;Válasz;Kategória"
    static std::vector<ChooseQuestion> loadChooseQuestions(const std::string& filename);

    // CÉL: Sorrendezős kérdések betöltése CSV-ből
    // FORMA: "Sorkérdés;A;B;C;D;Válasz;Kategória"
    static std::vector<OrderQuestion> loadOrderQuestions(const std::string& filename);
    // Mindkét CSV fileban skippelni kell az első sort és a hibás sorokat 
    //! EZEKET A FILEOKAT A PROJEKT KÖNYVTÁRÁBAN KERESSÜK

    // CÉL: Fájl létezésének ellenőrzése
    static bool fileExists(const std::string& path);
};

#endif