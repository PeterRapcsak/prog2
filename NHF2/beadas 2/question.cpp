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

     - OrderQuestion: sorrendezős kérdés
         - rendezendő elemek kiírása
         - 4 betűs sorrend ellenőrzése
         - itt nincs segítség
======================================================================*/

#include "memtrace.h" // memóriaszivárgás check

#include "question.h"
#include "gamestate.h" // GameState struct
#include "colors.h"

#include "utils.h"

#include <iostream> // konzol kiírás
#include <algorithm> // shuffle

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

void ChooseQuestion::apply5050(GameState& s) const {    
    s.used5050 = true; // A segítséget most használta el

    // Lekérjük a helyes választ
    string correct = getCorrectAnswer();

    // guardrail: ha üres
    if (correct.empty()) {
        return;
    }

    int correctIdx = toupper(correct[0]) - 'A';

    // Ide gyűjtjük a rossz válaszok indexeit
    vector<int> wrongIndexes;

    for (int i = 0; i < 4; i++) {
        if (i != correctIdx) {
            wrongIndexes.push_back(i);
        }
    }

    // Összekeverjük a rossz válaszokat
    std::shuffle(wrongIndexes.begin(), wrongIndexes.end(), rng());

    // Az első két rossz választ elrejtjük
    s.hiddenResponses[0] = wrongIndexes[0];
    s.hiddenResponses[1] = wrongIndexes[1];

}

void ChooseQuestion::applyAudience(GameState& s) const {
    // A segítséget most használta el
    s.usedAudience = true;

    // Lekérjük a helyes választ
    string correct = getCorrectAnswer();

    // guardrail: ha üres
    if (correct.empty()) {
        return;
    }

    int correctIdx = toupper(correct[0]) - 'A';

    // Minden százalékot lenullázunk
    for (int i = 0; i < 4; i++) {
        s.audienceValues[i] = 0;
    }

    // A helyes válasz kapjon 30 és 60 közötti százalékot
    s.audienceValues[correctIdx] = randomInt(30, 60);

    // Ennyi maradt a rossz válaszokra
    int remaining = 100 - s.audienceValues[correctIdx];

    vector<int> wrongIndexes;

    for (int i = 0; i < 4; i++) {
        if (i != correctIdx) {
            wrongIndexes.push_back(i);
        }
    }

    // Összekeverjük a rossz válaszokat
    // Mivel az elsőnek van esélye a legtöbbet kapni, az utolsónak a legkevesebbet
    std::shuffle(wrongIndexes.begin(), wrongIndexes.end(), rng());

    //? 1.
    int first = randomInt(0, remaining);
    remaining -= first;

    //? 2.
    int second = randomInt(0, remaining);
    remaining -= second;

    //? 3.
    int third = remaining;

    s.audienceValues[wrongIndexes[0]] = first;
    s.audienceValues[wrongIndexes[1]] = second;
    s.audienceValues[wrongIndexes[2]] = third;
}


void ChooseQuestion::ask(GameState& s) const {
    s.hiddenResponses[0] = -1;
    s.hiddenResponses[1] = -1;
    bool audienceThisQuestion = false; // ne mutasson százalékot a következő kérdésnél

    while (!s.gameOver) {
        clearScreen();
        printLevelHeader(s.currentLevel);

        // Kérdés kiírása, esetleges 50:50 elrejtésekkel, közönség százalékokkal
        displayWithHints(
            s.hiddenResponses[0],
            s.hiddenResponses[1],
            audienceThisQuestion ? s.audienceValues : nullptr
        );

        printSeparator();

        cout << "\nLehetőségek:\n";
        cout << "   A / B / C / D\n";

        if (!s.used5050) {
            cout << "   F - 50:50\n";
        } else {
            cout << "   " << Color::GREY_STRIKE << "F - 50:50" << Color::RESET << "\n";
        }
        if (!s.usedAudience) {
            cout << "   K - Közönség\n";
        } else {
            cout << "   " << Color::GREY_STRIKE << "K - Közönség" << Color::RESET << "\n";
        }
        cout << "   Q - Megállás\n";
        cout << "\nVálasz: ";

        string input;
        getline(cin, input);

        if (input.empty()) continue;
        normalizeInput(input);

        //? FELADÁS
        if (input == "Q") {
            s.walkAway = true;
            s.gameOver = true;
            return;
        }

        //? 50:50 SEGÍTSÉG
        if (input == "F") {
            if (s.used5050) {
                cout << "Az 50:50 már fel lett használva.\n\n";
            } else {
                apply5050(s);
            }
            continue;
        }

        //? KÖZÖNSÉG SEGÍTSÉG
        if (input == "K") {
            if (s.usedAudience) {
                cout << "A közönségsegítség már fel lett használva.\n\n";
            } else {
                applyAudience(s);
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
        if (isHiddenBy5050(selectedIndex, s.hiddenResponses[0], s.hiddenResponses[1])) {
            cout << "Ez az opció már el van rejtve az 50:50 miatt.\n\n";
            continue;
        }

        //? HELYESSÉG CHECK
        if (checkAnswer(input)) {
            cout << Color::BOLD_GREEN << "\nHelyes válasz!\n\n" << Color::RESET;
            waitEnter();
            return;
        }

        //! KÜLÖNBEN HELYTELEN
        cout << Color::BOLD_RED << "\nRossz válasz." << Color::RESET
             << " A helyes válasz: " << Color::BOLD_YELLOW << getCorrectAnswer()
             << Color::RESET << "\n";
             waitEnter();

        s.gameOver = true;
        s.finalPrize = getSafePrize(s.currentLevel - 1); // -1 mert ugye elbukta a szintet
        return;
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

void OrderQuestion::ask(GameState& s) const {
    while (!s.gameOver) {
        clearScreen();
        printLevelHeader(s.currentLevel);

        display();
        printSeparator();

        cout << "\nAdd meg a sorrendet 4 betűvel (pld: ABDC)\n";
        cout << "   Q - Megállás\n";
        cout << "\nVálasz: ";

        // Ugyanolyan input kezelés mint a feleletválasztósnál
        string input;
        getline(cin, input);
        if (input.empty()) continue;
        normalizeInput(input);

        //? FELADÁS
        if (input == "Q") {
            s.walkAway = true;
            s.gameOver = true;
            return;
        }

        //? ÉRVÉNYTELEN CHECK
        if (!isValidOrderInput(input)) {
            cout << "A válasz csak A, B, C, D betűket tartalmazhat, ismétlés nélkül.\n\n";
            continue;
        }

        //? HELYESSÉG CHECK
        if (checkAnswer(input)) {
            cout << Color::BOLD_GREEN << "\nHelyes válasz!\n\n" << Color::RESET;
            waitEnter();
            return;
        }

        //! KÜLÖNBEN HELYTELEN
        cout << Color::BOLD_RED << "\nRossz válasz." << Color::RESET
             << " A helyes sorrend: " << Color::BOLD_YELLOW << getCorrectAnswer()
             << Color::RESET << "\n";
             waitEnter();

        s.gameOver   = true;
        s.finalPrize = getSafePrize(s.currentLevel - 1);
        return;
    }
}


// CÉL: Sorrendezős válasz ellenőrzése
bool OrderQuestion::checkAnswer(const string& input) const {
    return input == correctAnswer;
}