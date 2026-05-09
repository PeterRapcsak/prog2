/*======================================================================
    colors.h - ANSI konzol szín konstansok
----------------------------------------------------------------------
    FELADAT:
     - Minden fájlban elérhető szín/stílus kódok egy helyen
     - Könnyen bővíthető, ha új szín kell

    Forrás: https://gist.github.com/JBlond/2fea43a3049b38287e5e9cefc87b2124
    Megjegyzés: "\e" helyett "\033"-at használunk ("\e" nem mindenhol szabványos)
======================================================================*/

#ifndef COLORS_H
#define COLORS_H

// CÉL: ANSI escape kódok konzol szövegszínezéshez és stílusokhoz
namespace Color {
    constexpr const char* RESET       = "\033[0m";    // minden stílus/szín visszaállítása
    constexpr const char* BOLD_RED    = "\033[1;31m"; // hibák, törölt lista, rossz válasz
    constexpr const char* BOLD_GREEN  = "\033[1;32m"; // helyes válasz, nyeremény összegek
    constexpr const char* BOLD_YELLOW = "\033[1;33m"; // cím, kérdésszöveg, menü számok
    constexpr const char* BOLD_CYAN   = "\033[1;36m"; // sorrendezős kérdés fejléc
    constexpr const char* CYAN        = "\033[36m";   // közönség szavazat sor
    constexpr const char* GREY_STRIKE = "\033[9;90m"; // már használt segítség (áthúzott, szürke)
}

#endif
