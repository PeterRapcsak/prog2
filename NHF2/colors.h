/*======================================================================
    colors.h - ANSI konzol szín konstansok
----------------------------------------------------------------------
    FELADAT:
     - Szín kódok minden fájlhoz
======================================================================*/

#ifndef COLORS_H
#define COLORS_H

// CÉL: ANSI escape kódok konzol színezéshez
namespace Color {
    constexpr const char* RESET       = "\033[0m";      // 
    constexpr const char* BOLD_RED    = "\033[1;31m";   // 
    constexpr const char* BOLD_GREEN  = "\033[1;32m";   // 
    constexpr const char* BOLD_YELLOW = "\033[1;33m";   // 
    constexpr const char* BOLD_CYAN   = "\033[1;36m";   // 
    constexpr const char* CYAN        = "\033[36m";     //
    constexpr const char* GREY_STRIKE = "\033[9;90m";   //
}

// nagyon menő hogy ilyen könnyen lehet ilyet csinálni
// source: https://gist.github.com/JBlond/2fea43a3049b38287e5e9cefc87b2124
// csak "\e" helyett "\033" -at használunk ("\e" nem mindenhol szabványos) 

#endif
