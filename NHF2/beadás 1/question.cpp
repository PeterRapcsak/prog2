/*======================================================================
    question.cpp - Kérdés objektumok megvalósítása
----------------------------------------------------------------------
    FELADAT:
     - Question: közös absztrakt alap osztály
         - kérdés szövege
         - kategória
         - helyes válasz
         - válaszlehetőségek

     - ChooseQuestion: feleletválasztós kérdés
         - nehézségi szint
         - kérdés kiírása simán vagy 50:50 / közönség segítséggel
         - válasz ellenőrzése
         - ask() override: 50:50, közönség, A-D választás

     - OrderQuestion: sorrendezős kérdés
         - rendezendő elemek kiírása
         - 4 betűs sorrend ellenőrzése
         - ask() override: 4 betűs sorrend bekérése
         - itt nincs segítség
======================================================================*/

#include "memtrace.h" // memóriaszivárgás check

#include "question.h"
#include "colors.h"
#include "game.h" // a static segédek miatt (clearScreen, printLevelHeader, stb.)

#include <algorithm> // shuffle
#include <cctype>    // toupper
#include <iostream>  // konzol kiírás

using std::string;
using std::vector;
using std::cout;
using std::cin;

//! ---------- ALAP OSZTÁLY ----------


// CÉL: Közös kérdés adatok eltárolása, NEM PÉLDÁNYOSÍTJUK
Question::Question(
    const string& q,            // kérdés szövege
    const string& cat,          // kategória
    const string& correct,      // helyes válasz
    const vector<string>& ans)  // válaszlehetőségek vektora
    : question(q),
      category(cat),
      correctAnswer(correct),
      answers(ans)
{}

// Virtuális destruktor, mert ebből örökölnek más osztályok
Question::~Question() {}

// CÉL: Kérdés lekérése
const string& Question::getQuestion() const {
    return question;
}

// CÉL: Kategória lekérése
const string& Question::getCategory() const {
    return category;
}

// CÉL: Helyes válasz lekérése
const string& Question::getCorrectAnswer() const {
    return correctAnswer;
}

// CÉL: Válaszlehetőségek lekérése
const vector<string>& Question::getAnswers() const {
    return answers;
}


//! ---------- FELELETVÁLASZTÓS KÉRDÉS ----------

/*
    CÉL: Feleletválasztós kérdés létrehozása
    MEGJEGYZÉS:
        A közös adatokat az ősosztály kapja meg,
        a nehézséget pedig ez az osztály tárolja.
*/
ChooseQuestion::ChooseQuestion(
    int diff,                   // nehézségi szint, 1-től 12-ig
    const string& q,            // kérdés szövege
    const string& cat,          // kategória
    const string& correct,      // Helyes válasz betűje, pld: "A"
    const vector<string>& ans)  // 4 válaszlehetőség: A, B, C, D
    : Question(q, cat, correct, ans),
      difficulty(diff)
{}

ChooseQuestion::~ChooseQuestion() {}

// CÉL: Feleletválasztós kérdés sima kiírása
void ChooseQuestion::display() const {

    // Ugyanazt a kiíró függvényt használjuk, csak segítségek nélkül
    // -1      -> nincs elrejtett válasz
    // nullptr -> nincs közönség százalék
    displayWithHints(-1, -1, nullptr);
}
/*
    CÉL: Feleletválasztós kérdés kiírása segítségekkel
    MEGJEGYZÉS:
        hidden0 / hidden1 az elrejtett válaszok indexei.
        audience lehet nullptr, ilyenkor nem írunk ki százalékot.
*/
void ChooseQuestion::displayWithHints(
    int hidden0,
    int hidden1,
    const int* audience) const {

    cout << "  Téma: " << category << "\n\n";
    cout << "  " << Color::BOLD_YELLOW << question << Color::RESET << "\n\n";

    // Csak akkor írjuk ki, ha tényleg megvan a 4 válasz
    if (answers.size() >= 4) {

        // Végigmegyünk az A, B, C, D válaszokon
        for (int i = 0; i < 4; i++) {

            // Ha ezt az opciót elrejtette az 50:50, akkor kihagyjuk
            if (i == hidden0 || i == hidden1) {
                continue;
            }

            // Válasz betűjének kiírása
            // 0 + 'A' -> A,
            // 1 + 'A' -> B, stb.
            cout << "  "
                 << Color::BOLD_YELLOW
                 << "[" << static_cast<char>('A' + i) << "]"
                 << Color::RESET;

            // Ha van közönség segítség, akkor kiírjuk mellé a százalékot is
            if (audience != nullptr) {
                cout << Color::BOLD_GREEN
                     << " ["
                     << audience[i] << "%";

                // Csak azért, hogy a 0-9% is szép legyen a konzolban
                if (audience[i] < 10) {
                    cout << " ";
                }

                cout << "]" << Color::RESET;
            }

            // Maga a válasz szövege
            cout << " " << answers[static_cast<std::size_t>(i)] << "\n";
        }
    }
}

// CÉL: Feleletválasztós válasz ellenőrzése
bool ChooseQuestion::checkAnswer(const string& input) const {
    return input == correctAnswer;
}

// CÉL: Nehézségi szint lekérése
int ChooseQuestion::getDifficulty() const {
    return difficulty;
}

//! ---------- CHOOSE SEGÍTSÉGEK ----------

// CÉL: 50:50 - kiválaszt 2 helytelen választ amit elrejtünk
void ChooseQuestion::apply5050(int& hidden0, int& hidden1) const {

    hidden0 = -1;
    hidden1 = -1;

    // guardrail: ha üres a helyes válasz
    if (correctAnswer.empty()) {
        return;
    }

    int correctIdx = toupper(correctAnswer[0]) - 'A';

    // Ide gyűjtjük a rossz válaszok indexeit
    vector<int> wrongIndexes;
    for (int i = 0; i < 4; i++) {
        if (i != correctIdx) {
            wrongIndexes.push_back(i);
        }
    }

    // Összekeverjük a rossz válaszokat
    std::shuffle(wrongIndexes.begin(), wrongIndexes.end(), Game::rng());

    // Az első két rossz választ elrejtjük
    hidden0 = wrongIndexes[0];
    hidden1 = wrongIndexes[1];
}

// CÉL: Közönség - pseudo-random szavazateloszlás (helyes kapja a legtöbbet)
void ChooseQuestion::applyAudience(int audience[4]) const {

    // Minden százalékot lenullázunk
    for (int i = 0; i < 4; i++) {
        audience[i] = 0;
    }

    // guardrail: ha üres a helyes válasz
    if (correctAnswer.empty()) {
        return;
    }

    int correctIdx = toupper(correctAnswer[0]) - 'A';

    // A helyes válasz kapjon 30 és 60 közötti százalékot
    audience[correctIdx] = Game::randomInt(30, 60);

    // Ennyi maradt a rossz válaszokra
    int remaining = 100 - audience[correctIdx];

    vector<int> wrongIndexes;
    for (int i = 0; i < 4; i++) {
        if (i != correctIdx) {
            wrongIndexes.push_back(i);
        }
    }

    // Összekeverjük a rossz válaszokat
    // Mivel az elsőnek van esélye a legtöbbet kapni, az utolsónak a legkevesebbet
    std::shuffle(wrongIndexes.begin(), wrongIndexes.end(), Game::rng());

    //? 1.
    int first = Game::randomInt(0, remaining);
    remaining -= first;

    //? 2.
    int second = Game::randomInt(0, remaining);
    remaining -= second;

    //? 3.
    int third = remaining;

    audience[wrongIndexes[0]] = first;
    audience[wrongIndexes[1]] = second;
    audience[wrongIndexes[2]] = third;
}

//! ---------- CHOOSE ASK ----------

/*
    CÉL: Feleletválasztós kérdezés UI loop
    MEGJEGYZÉS:
        - Beolvas inputot, és kezeli a 50:50/közönség segítségeket.
        - A used5050 és usedAudience referencián jönnek be, hogy a következő
          kérdésnél is tudja a Game, hogy már felhasználta a játékos.
*/
Question::AskResult ChooseQuestion::ask(
    int currentLevel,
    bool& used5050,
    bool& usedAudience) {

    // 50:50 által elrejtett válaszok indexei (-1 = nincs elrejtve)
    int hidden0 = -1;
    int hidden1 = -1;

    // Közönség szavazatok [A, B, C, D] %-ban
    int audience[4] = {0, 0, 0, 0};

    // Ne mutasson százalékot a következő kérdésnél (csak amelyiknél kérte)
    bool audienceThisQuestion = false;

    while (true) {
        Game::clearScreen();
        Game::printLevelHeader(currentLevel);

        // Kérdés kiírása, esetleges 50:50 elrejtésekkel, közönség százalékokkal
        displayWithHints(
            hidden0,
            hidden1,
            audienceThisQuestion ? audience : nullptr
        );

        Game::printSeparator();

        cout << "\nLehetőségek:\n";
        cout << "   A / B / C / D\n";

        if (!used5050) {
            cout << "   F - 50:50\n";
        } else {
            cout << "   " << Color::GREY_STRIKE << "F - 50:50" << Color::RESET << "\n";
        }
        if (!usedAudience) {
            cout << "   K - Közönség\n";
        } else {
            cout << "   " << Color::GREY_STRIKE << "K - Közönség" << Color::RESET << "\n";
        }
        cout << "   Q - Megállás\n";
        cout << "\nVálasz: ";

        string input;
        getline(cin, input);

        if (input.empty()) continue;
        Game::normalizeInput(input);

        //? FELADÁS
        if (input == "Q") {
            return WALK_AWAY;
        }

        //? 50:50 SEGÍTSÉG
        if (input == "F") {
            if (used5050) {
                cout << "Az 50:50 már fel lett használva.\n\n";
            } else {
                apply5050(hidden0, hidden1);
                used5050 = true;
            }
            continue;
        }

        //? KÖZÖNSÉG SEGÍTSÉG
        if (input == "K") {
            if (usedAudience) {
                cout << "A közönségsegítség már fel lett használva.\n\n";
            } else {
                applyAudience(audience);
                usedAudience = true;
                audienceThisQuestion = true;
            }
            continue;
        }

        //? ÉRVÉNYTELEN CHECK
        if (input.size() != 1 || input[0] < 'A' || input[0] > 'D') {
            cout << "Érvénytelen válasz, próbáld újra.\n\n";
            continue;
        }

        int selectedIndex = input[0] - 'A'; //ASCII miatt, nagyon clean

        //? 50:50 ELREJTÉS CHECK
        if (selectedIndex == hidden0 || selectedIndex == hidden1) {
            cout << "Ez az opció már el van rejtve az 50:50 miatt.\n\n";
            continue;
        }

        //? HELYESSÉG CHECK
        if (checkAnswer(input)) {
            cout << Color::BOLD_GREEN << "\nHelyes válasz!\n\n" << Color::RESET;
            Game::waitEnter();
            return CORRECT;
        }

        //! KÜLÖNBEN HELYTELEN
        cout << Color::BOLD_RED << "\nRossz válasz." << Color::RESET
             << " A helyes válasz: " << Color::BOLD_YELLOW << correctAnswer
             << Color::RESET << "\n";
        Game::waitEnter();

        return WRONG;
    }
}


//! ---------- SORRENDEZŐS KÉRDÉS ----------

/*
    CÉL: Sorrendezős kérdés létrehozása
    MEGJEGYZÉS:
        Itt nincs difficulty, mert a sorrendezős kérdések
        nincsenek külön szintekhez kötve. (Mindegyik elég nehéz)
*/
OrderQuestion::OrderQuestion(
    const string& q,            // kérdés szövege
    const string& cat,          // kategória
    const string& correct,      // helyes sorrend, pld: "BDAC"
    const vector<string>& ans)  // 4 rendezendő elem
    : Question(q, cat, correct, ans)
{}

OrderQuestion::~OrderQuestion() {}

// CÉL: Sorrendezős kérdés kiírása
void OrderQuestion::display() const {
    cout << "  "
         << Color::BOLD_CYAN << "[SORRENDEZŐS]" << Color::RESET
         << " Téma: " << category << "\n\n";

    cout << "  " << Color::BOLD_YELLOW << question << Color::RESET << "\n\n";

    // Csak akkor írjuk ki, ha van legalább 4 elem
    if (answers.size() >= 4) {

        // Végigmegyünk az A, B, C, D elemeken
        for (int i = 0; i < 4; i++) {

            cout << "  "
                 << Color::BOLD_YELLOW
                 << "[" << static_cast<char>('A' + i) << "]"
                 << Color::RESET
                 << " " << answers[static_cast<std::size_t>(i)] << "\n";
        }
    }
}

// CÉL: Sorrendezős válasz ellenőrzése
bool OrderQuestion::checkAnswer(const string& input) const {
    return input == correctAnswer;
}

// CÉL: Sorrendezős válasz validitás ellenőrzése (4 betű, csak A-D, ismétlés nélkül)
bool OrderQuestion::isValidOrderInput(const string& input) {
    if (input.size() != 4) {
        return false;
    }

    for (int i = 0; i < 4; i++) {
        // Csak A, B, C, D lehet
        if (input[i] < 'A' || input[i] > 'D') { // ASCII szám alapján
            return false;
        }

        // Megnézzük, hogy volt-e már ugyanilyen betű korábban
        for (int j = 0; j < i; j++) {
            if (input[i] == input[j]) {
                return false;
            }
        }
    }

    return true;
}

//! ---------- ORDER ASK ----------

/*
    CÉL: Sorrendezős kérdezés UI loop
    MEGJEGYZÉS:
        - used5050 és usedAudience NEM kell sorrendezős kérdésnél, de
          az ősosztály signaturája miatt itt is benne van. Direkt nem
          használjuk őket (a (void) cast elkerüli a warning-ot).
*/
Question::AskResult OrderQuestion::ask(
    int currentLevel,
    bool& used5050,
    bool& usedAudience) {

    // Itt nincs 50:50 / közönség, tehát az unused warning-ot lecsendesítjük
    (void)used5050;
    (void)usedAudience;

    while (true) {
        Game::clearScreen();
        Game::printLevelHeader(currentLevel);

        display();
        Game::printSeparator();

        cout << "\nAdd meg a sorrendet 4 betűvel (pld: ABDC)\n";
        cout << "   Q - Megállás\n";
        cout << "\nVálasz: ";

        // Ugyanolyan input kezelés mint a feleletválasztósnál
        string input;
        getline(cin, input);
        if (input.empty()) continue;
        Game::normalizeInput(input);

        //? FELADÁS
        if (input == "Q") {
            return WALK_AWAY;
        }

        //? ÉRVÉNYTELEN CHECK
        if (!isValidOrderInput(input)) {
            cout << "A válasz csak A, B, C, D betűket tartalmazhat, ismétlés nélkül.\n\n";
            continue;
        }

        //? HELYESSÉG CHECK
        if (checkAnswer(input)) {
            cout << Color::BOLD_GREEN << "\nHelyes válasz!\n\n" << Color::RESET;
            Game::waitEnter();
            return CORRECT;
        }

        //! KÜLÖNBEN HELYTELEN
        cout << Color::BOLD_RED << "\nRossz válasz." << Color::RESET
             << " A helyes sorrend: " << Color::BOLD_YELLOW << correctAnswer
             << Color::RESET << "\n";
        Game::waitEnter();

        return WRONG;
    }
}
