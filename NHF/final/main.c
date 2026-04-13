/*======================================================================
    main.c - Főprogram 
----------------------------------------------------------------------
    CÉL
     - A játékmenet lebonyolítása (segédfüggvényekkel)
     - a main() függvény

    PLATFORM
     - Windows: <windows.h>, <direct.h> (SetConsoleCP, _chdir)

    SZERZŐI JEGYZET
     - Minden függvénynek camelCase elnevezése van
     - A függvények fejléce fölötti értelmező 
        CÉL     = Mi a célja a függvénynek
        BE      = Bemenet
        KI      = Kimenet

    SZEKCIÓK:   //! ===== cím =====
======================================================================*/

#include <stdio.h>  // printf, fgets
#include <stdlib.h> // atoi, rand, srand
#include <string.h> // strlen, strcspn
#include <time.h>   // time
#include <ctype.h>  // toupper, isalpha
#include "game.h"   // struktúrák, konstansok, függvények
#include "debugmalloc.h"

//----- filenevek -----
const char* FILE_CHOOSE = "kerdesek.csv";
const char* FILE_ORDER  = "sorkerdesek.csv";

    
//CÉL: Egy teljes játékmenet levezénylése
void playGame(                                  // BEMENET: 
    ChooseQuestionArray *cqdata,                // feleletválasztós kérdések [szint][sorszám]
    int cqCounts[LEVELS],                       // szintenként feleletválasztós kérdések száma
    OrderQuestion oqdata[MAX_ORDER_QUESTIONS],  // sorrendezős kérdések tömbje
    int oqCount,                                // sorrendezős kérdések száma
    int mode                                    // játékmód
){

    //!---------- JÁTÉKOS NÉV BEKÉRÉSE ----------
    
    char playerName[MAX_NAME];

    getPlayerName(playerName);

    system("cls");

    // Változók
    int currentLevel = 0;   // Aktuális szint
    int gameOver = 0;       // Vége a játéknak?
    int walkAway = 0;       // Feladta a játékos?
    int finalPrize = 0;     // Főnyeremény
    int used5050 = 0;       // Felhasználta már a 50:50-t?
    int usedAudience = 0;   // Felhasználta már a közönség segítségét?
    int hiddenResponse[2] = { -1, -1 };  // 2 elemű lista (majd a 50:50-hez kell)

    //! ---------- FŐ JÁTÉKCIKLUS ----------
    
    while (currentLevel < LEVELS && !gameOver) {
        hiddenResponse[0] = -1;     // 50:50 segítség nullázása
        hiddenResponse[1] = -1;     // ha nem nullázzuk, a következő kérdésekben se lehet a kizártakat választani
        int isOrderLevel = 0;       // ha 1 akkor ez a szint sorrendezős
        char correctAnsStr[6];      // helyes válasz (pld: "A" vagy "ABCD") -> 6, hogy biztos legyen a hossz
        ChooseQuestion *cq = NULL;  // pointer a feleletválasztós kérdésre (ha ilyen a szint)

        // Sorrendezős ellenőrzése
        if (mode == MODE_ORDER)
            isOrderLevel = 1; // Ha csak sorrendezős akkor mindig 1
        else if (mode == MODE_BOTH && (currentLevel + 1) % 3 == 0)
            isOrderLevel = 1; // Ha vegyes akkor minden 3. kérdés sorrendezős

        // Fejléc
        printSeparator();

        char *s = formatPrize(PRIZE_LADDER[currentLevel]);
        printf("  %d. KÉRDÉS  |  Tét: %s Ft\n", currentLevel + 1, s);
        free(s);

        int safe = getSafePrize(currentLevel - 1);
        if (safe > 0){ // Ha a játékosnak van fix nyereménye akkor kiírjuk
            char *s = formatPrize(safe);
            printf("  (Biztos nyereményed: %s Ft)\n", s);
            free(s);
        }
        printSeparator();

        //! Kérdés generálás & kiírás
        if (isOrderLevel) {
            //? Sorrendezős

            // Random kérdés kiválasztása
            int r = rand() % oqCount;
            OrderQuestion *oq = &oqdata[r];

            printf("  [SORRENDEZŐS] Téma: %s\n\n  %s\n\n", oq->cat, oq->q);
            printf("  A: %s\n", oq->answers[0]);
            printf("  B: %s\n", oq->answers[1]);
            printf("  C: %s\n", oq->answers[2]);
            printf("  D: %s\n", oq->answers[3]);


            // Helyes sorrend (pld: "ABCD")
            // Biztonságos másolás és nagybetűsítés, ha szükséges
            strncpy(correctAnsStr, oq->correct, sizeof(correctAnsStr) - 1); // Másol
            correctAnsStr[sizeof(correctAnsStr) - 1] = '\0'; // Lezár

            for (int i = 0; correctAnsStr[i]; i++)
                correctAnsStr[i] = toupper((unsigned char)correctAnsStr[i]);
        } else {
            //? Feleletválasztós

            // Random kérdés kiválasztása
            int r = rand() % cqCounts[currentLevel];
            cq = &(*cqdata)[currentLevel][r];

            printf("  Téma: %s\n\n  %s\n\n", cq->cat, cq->q);
            printf("  A: %s\n", cq->answers[0]);
            printf("  B: %s\n", cq->answers[1]);
            printf("  C: %s\n", cq->answers[2]);
            printf("  D: %s\n", cq->answers[3]);


            // Helyes válasz (pld: "A")
            correctAnsStr[0] = cq->correct[0]; 
            correctAnsStr[1] = '\0';
        }
        printSeparator();

        //! Válasz bekérés
        char input[16];
        int valid = 0;
        
        while (!valid) {
            printf("\nVálaszod ");
            if (!isOrderLevel) { // Ha feleletválasztós kérdés: elérhetőek a segítségek
                if (!used5050)
                    printf("(F - 50:50) ");
                if (!usedAudience)
                    printf("(K - Közönség) ");
            }
            printf("vagy (Q - Megállás): ");
            fflush(stdout);
            
            // Beolvasás
            if (!fgets(input, sizeof(input), stdin))
                continue;

            // \n levágása
            input[strcspn(input, "\n")] = '\0';

            // Minden betűt nagybetűvé alakítunk
            for (int i = 0; input[i]; i++)
                input[i] = toupper((unsigned char)input[i]); 

            // Megállás
            if (strcmp(input, "Q") == 0) {
                walkAway = 1;
                gameOver = 1;
                valid = 1;

                // Ha kiszáll, az ELŐZŐ SZINT pénzét kapja meg
                // Ha elbukja, az előző checkpoint összegét kapja meg
                // (Ha az első kérdésnél száll ki akkor 0 Ft.)
                if (currentLevel > 0)
                    finalPrize = PRIZE_LADDER[currentLevel - 1];
                else
                    finalPrize = 0;

                break;
            }

            //! Segítségek: Csak akkor ha a kérdés feleletválasztós
            if (!isOrderLevel) {
                // 50:50
                if (strcmp(input, "F") == 0 && !used5050) {
                    used5050 = 1;

                    print5050(cq, hiddenResponse);
                    printf("\n50:50 AKTIVÁLVA! Maradt:\n");

                    char opts[] = "ABCD";
                    for (int i = 0; i < 4; i++) {
                        if (i != hiddenResponse[0] && i != hiddenResponse[1]) 
                            printf("  %c: %-20s\n", opts[i], cq->answers[i]);
                    }
                    continue;
                }
                // Közönség segítség
                if (strcmp(input, "K") == 0 && !usedAudience) {
                    usedAudience = 1;
                    printAudience(cq);
                    continue;
                }
            }

            //! Bevitel érvényesség ellenőrzése
            if (isOrderLevel) {
                // 4 karaktert várunk
                if (strlen(input) == 4 && strspn(input, "ABCD") == 4)
                    valid = 1;
                else printf("Hibás formátum! (pld: ABCD)\n"); // Hibakezelés

            } else {
                // 1 karaktert várunk
                if (strlen(input) == 1 && strchr("ABCD", input[0])) {
                    int idx = input[0] - 'A';

                    if (idx == hiddenResponse[0] || idx == hiddenResponse[1])
                        printf("Ez a válasz ki van zárva!\n"); // Hibakezelés
                    else 
                        valid = 1;
                } else printf("Érvénytelen! Csak A, B, C, D.\n"); // Hibakezelés
            }
        }

        //! Kiértékelés
        if (!walkAway) {
            // A Játékos kiszállt

            if (strcmp(input, correctAnsStr) == 0) {
                // Helyes válasz
                // \033[92m...\033[0m = Zöld színkód
                printf("\n\033[92mHELYES VÁLASZ!\033[0m\n");
                if (isSafeLevel(currentLevel)){
                    char *s = formatPrize(PRIZE_LADDER[currentLevel]);
                    printf(">>> BIZTOS SZINT ELÉRVE! (%s Ft) <<<\n", s);
                    free(s);
                }
                currentLevel++;
                waitEnter();
                system("cls");

            } else {
                // Helytelen válasz
                // \033[91m...\033[0m = Piros szín
                printf("\n\033[91mROSSZ VÁLASZ!\033[0m A helyes: %s\n", correctAnsStr);
                finalPrize = getSafePrize(currentLevel - 1);

                char *s = formatPrize(finalPrize);
                printf("Kiestél! Nyereményed: %s Ft\n", s);
                free(s);
                gameOver = 1;
            }
        }
    }

    //! Játék vége
    if (!walkAway && currentLevel == LEVELS) {
        // \x1b[93m...\x1b[0m = Citromsárga a szöveg
        printf("\n\x1b[93mGRATULÁLOK! ÖN MILLIOMOS LETT!\x1b[0m\n");

        finalPrize = PRIZE_LADDER[LEVELS - 1];

        char *s = formatPrize(finalPrize);
        printf("\nNyereményed: %s Ft\n", s);
        free(s);
        
        

    } else if (walkAway) {
        char *s = formatPrize(finalPrize);
        printf("\nMegálltál! Nyereményed: %s Ft\n", s);
        free(s);
        
    }

    //! Eredmény mentése
    saveHighscore(playerName, finalPrize);
    waitEnter();
}

/*
    CÉL: Program belépési pontja
     - Konzol beállítása
     - Kérdések betöltése
     - Főmenü kezelése
*/
int main(void) {

    consoleUtf8(); // Konzol beállítása
    srand((unsigned int)time(NULL)); // random mag init

    // Felülírni a debugmalloc max block méretét: Long típus, 1024 = Kb, 1024*1024 = Mb, Tehát 32 Mb méret a max amit ellenőrizni tud 
    debugmalloc_max_block_size(32L * 1024 * 1024);

    // itt hozzuk létre a választós kérdések két dimenziós tömbjét
    ChooseQuestionArray *cqdata = calloc(1, sizeof(ChooseQuestionArray));
    if (!cqdata) {
        printf("Memóriafoglalási hiba");
        waitEnter();
        return 1;
    }

    int cqCounts[LEVELS] = {0}; // Szintenként hány kérdés van

    // Sorrendezős kérdések
    static OrderQuestion oqdata[MAX_ORDER_QUESTIONS];
    int oqCount = 0; // hány  sorrendezős kérdés van

    printf("Rendszer indítása, adatok betöltése...\n");
    
    // Betöltés
    int hasChoose = loadChooseQuestions(FILE_CHOOSE, cqdata, cqCounts);
    int hasOrder = loadOrderQuestions(FILE_ORDER, oqdata, &oqCount);

    // Ha a fájlok léteznek, de üresek voltak, vagy betöltési hiba, 
    // megpróbáljuk legalább a fájl létét ellenőrizni a menühöz:
    if (!hasChoose) hasChoose = fileExists(FILE_CHOOSE);
    if (!hasOrder) hasOrder = fileExists(FILE_ORDER);

    if (!hasChoose && !hasOrder) {
        printf("HIBA: Nem találhatók a kérdésfájlok!\n");
        waitEnter();
        freeChooseQuestions(cqdata, cqCounts);
        freeOrderQuestions(oqdata, oqCount);
        return 1;
    }

    system("cls");
    int running = 1;

    //! ---------- FŐMENÜ ----------
    while (running) {
        system("cls");
        printSeparator();

        // \t\x1b[5m...\x1b[0m = Lassan villog a cím
        printf("\t\x1b[5mLEGYEN ÖN IS MILLIOMOS\x1b[0m\n");
        printSeparator();
        printf("\n  [1] ÚJ JÁTÉK\n  [2] DICSŐSÉGLISTA\n  [3] KILÉPÉS\n\n");
        printSeparator();
        printf("Választás: ");
        
        char menuInput[16];
        if (!fgets(menuInput, sizeof(menuInput), stdin))
            continue;
        int choice = atoi(menuInput);

        switch(choice) {
            case 1:
                // Játékmód választás
                printf("\nJátékmódok:\n");
                if (hasChoose) printf("  [1] Feleletválasztós\n");
                if (hasOrder)  printf("  [2] Sorrendezős\n");
                if (hasChoose && hasOrder) printf("  [3] Vegyes\n");

                while(1) {
                    printf("Mód: ");
                    if (!fgets(menuInput, sizeof(menuInput), stdin)) // Bekérünk egy módot
                        continue;
                    int m = atoi(menuInput); // Számmá alakítjuk

                    // Játékmód választás (Ha ezek közül bármelyik teljesül akkor mehet a játék)
                    if ((m == MODE_CHOOSE && hasChoose) ||
                        (m == MODE_ORDER && hasOrder) ||
                        (m == MODE_BOTH && hasChoose && hasOrder)) { 
                        system("cls");
                        playGame(cqdata, cqCounts, oqdata, oqCount, m);
                        break;
                    }
                    printf("Érvénytelen mód vagy hiányzó fájl!\n");
                }
                break;

            case 2:
                system("cls");
                showHighscores();
                waitEnter();
                break;

            case 3:
                running = 0;
                printf("\nViszontlátásra!\n");
                freeChooseQuestions(cqdata, cqCounts);
                freeOrderQuestions(oqdata, oqCount);
                break;

            default:
                // Rossz válasz -> újra kiírjuk a menüt
                printf("Érvénytelen menüpont! (1-3)\n");
                waitEnter();
                system("cls");
                break;
        }
    }
    return 0;
}