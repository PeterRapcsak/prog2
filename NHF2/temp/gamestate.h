
#ifndef GAMESTATE_H
#define GAMESTATE_H

// CÉL: Játékállapot tárolása hogy át ÉS vissza tudjuk adni a question.ask()-nak a paramétereit/eredményét
//  - Azért itt van, mert a game tulajdonságait tárolja / a game ctor-ban is használjuk
struct GameState {
    bool walkAway;          // Feladta?
    bool gameOver;          // Játék loop vége?
    bool used5050;          // Használta az 50:50 segítséget?
    bool usedAudience;      // Használta a közönség segítséget?
    int currentLevel;       // aktuális szintindex (0 - LEVELS-1)
    int finalPrize;         // végső nyeremény (biztos szint vagy aktuális szint)
    int hiddenResponses[2]; // 50:50 által elrejtett válaszok indexei (-1 = nincs elrejtve)
    int audienceValues[4];  // közönség szavazatok [A, B, C, D] %-ban
};

#endif

