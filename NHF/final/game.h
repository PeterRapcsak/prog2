/*======================================================================
    game.h - Header file
----------------------------------------------------------------------
    CÉL:
     - Konstansok, stuct-ok, függvény-definiálások
    COMPILE: 
     - gcc.exe -Wall -Wextra -g3 game.c main.c -o loim.exe
======================================================================*/

#ifndef GAME_H
#define GAME_H

//! ---------- KONSTANSOK ----------

#define LEVELS 12                           // Szintek száma
#define MAX_CHOOSE_QUESTIONS_PER_LEVEL 600  // feleletválasztós / szint (Azért kell hogy egyszerűen tudjunk random kérdést generálni)
#define MAX_ORDER_QUESTIONS 200             // sorrendezős összesen

#define MAX_TXT  256    // kérdés / válasz / kategória max hossza
#define MAX_NAME 21     // játékos neve max hossza (20 + \0)

// 15 lépcsős pénzlétra (Ft)
static const int PRIZE_LADDER[LEVELS] = {
    5000,       10000,      20000,      50000,      100000,
    200000,     300000,     500000,     800000,     1500000,
    3000000,    5000000
};

// Játékmódok
#define MODE_CHOOSE 1   // Csak feleletválasztós
#define MODE_ORDER 2   // Csak sorrendezős
#define MODE_BOTH 3   // Vegyes mód

//! ---------- STRUKTÚRÁK ----------

typedef struct {
    int  diff;          // nehézség (1..15)
    char *q;            // kérdés szövege
    char *answers[4];   // A,B,C,D válasz
    char *correct;      // helyes válasz betűje ('A'..'D')
    char *cat;          // kategória
} ChooseQuestion;       // Feleletválasztós kérdés struct

// Tömb típusdefiníció a feleletválasztós kérdésekhez
typedef ChooseQuestion ChooseQuestionArray[LEVELS][MAX_CHOOSE_QUESTIONS_PER_LEVEL];

typedef struct {
    char *q;            // Kérdés
    char *answers[4];   // 4 elem, amiket sorba kell rakni
    char *correct;      // helyes sorrend pld: "ACBD" + '\0'
    char *cat;          // kategória
} OrderQuestion;        // Sorrendezős kérdés struct

typedef struct {
    char *name; // Játékos neve
    int prize;  // Elért nyeremény
    char *date; // Dátum
} HighScore;    // Dicsőséglista struct

// Tömb típusdefiníció a sorren kérdésekhez
typedef OrderQuestion OrderQuestionArray[MAX_ORDER_QUESTIONS];

//! ---------- FÜGGVÉNYEK ----------

// Konzol / rendszer
void consoleUtf8(void);
void printSeparator(void);
void waitEnter(void);
int fileExists(const char *filename);
void getPlayerName(char *playerName);

char* formatPrize(int prize);

// Betöltés
int loadChooseQuestions(const char *filename, ChooseQuestionArray *cqd, int cqCounts[LEVELS]);
int loadOrderQuestions(const char *filename, OrderQuestion questions[MAX_ORDER_QUESTIONS], int *orderCount);

// Felszabadítás
void freeChooseQuestions(ChooseQuestionArray *cqd, int cqCounts[LEVELS]);
void freeOrderQuestions(OrderQuestion questions[MAX_ORDER_QUESTIONS], int count);

// Dicsőséglista
void saveHighscore(const char* name, int prize);
void showHighscores(void);

// Játék logika
int isSafeLevel(int index);
int getSafePrize(int currentLevelIndex);

// Segítségek
void print5050(ChooseQuestion *q, int removedIndices[2]);
void printAudience(ChooseQuestion *q);

#endif