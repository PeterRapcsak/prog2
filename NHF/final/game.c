/*======================================================================
    game.c - Segédfüggvények
---------------------------------------------------------------------- 
    CÉL:
     - Konzol beállítások, csv betöltés / free, 
======================================================================*/

#include <stdio.h>      // printf, fgets, FILE
#include <stdlib.h>     // atoi, rand, srand, strdup, realloc, free
#include <string.h>     // strlen, strcpy, strncpy, strtok, strcspn, strchr
#include <locale.h>     // setlocale (Konzolhoz)

#ifdef _WIN32
#include <windows.h>    // SetConsoleOutputCP, SetConsoleCP
#endif

#include <time.h>       // time, localtime, struct tm
#include "game.h"       // saját header file
#include "debugmalloc.h"

//! ---------- KONZOL ÉS EGYÉB ESZKÖZÖK ----------

/*
    CÉL: Windows konzolt UTF-8 módba állítani
    MELLÉKHATÁS: Windows-on más mint a többi rendszeren (feltételezzük hogy a felhasználó windows-on futtatja)
*/
void consoleUtf8(void) {
    setlocale(LC_ALL, ".UTF-8");
    // mivel úgy használják a számokat hogy "40.000.000" és nem "40000000"
    // ez a legegyszerűbb megoldás amit találtam
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    setlocale(LC_NUMERIC, "de_DE.UTF-8");
#endif
}


// CÉL: Egy elválasztó vonal kiírásaf
void printSeparator(void) {
    printf("----------------------------------------------------------------\n");
}

//CÉL: "Nyomj Entert a folytatáshoz..." megálló
void waitEnter(void) {
    printf("\n[Nyomj Entert a folytatáshoz...]");
    fflush(stdout);

    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

/*
    CÉL: Megnézni, hogy egy fájl létezik-e
    KI: 1 = létezik, 0 = nem
*/
int fileExists(const char *filename) {
    FILE *f = fopen(filename, "r"); // megpróbáljuk megnyitni olvasásra

    if (f) {
        fclose(f);
        return 1;   // ha sikerül, bezárjuk és visszaadjuk 1-et
    }
    return 0;   // ha nem sikerül, 0
}

/*
   CÉL: Játékos név bekérése
   BE/KI: A játkos nevére mutató pointer
*/
void getPlayerName(char *playerName) {
    while (1) {
        printf("\nKérlek add meg a neved (Max 20 karakter): ");
        fflush(stdout);

        // Alap név, ha a felhasználó megszakítja a beolvasást
        if (!fgets(playerName, MAX_NAME, stdin)) {
            strcpy(playerName, "Jatekos");
            break;
        }

        // Sorvége levágása
        playerName[strcspn(playerName, "\n")] = '\0';

        // Alap név, ha nem ír be semmit
        if (strlen(playerName) == 0) {
            strcpy(playerName, "Jatekos");
            break;
        }

        // ';' karakter kiszűrése
        for (int i = 0; playerName[i]; i++) {
            if (playerName[i] == ';')
                playerName[i] = ' ';
        }

        break;
    }
}

/*
    CÉL: Egész számot "5.000.000" formátumra alakítani
    BE:  prize - pld: 5000000
    KI:  új malloc-olt string, pl. "5.000.000"  => FEL KELL SZABADÍTANI HASZNÁLAT UTÁN
    LINK: https://www.geeksforgeeks.org/c/snprintf-c-library/ 
    LINK: https://stackoverflow.com/questions/8257714/how-can-i-convert-an-int-to-a-string-in-c  
*/
char* formatPrize(int prize){

    char num[10];
    snprintf(num, sizeof(num), "%d", prize);

    int len = (int)strlen(num); // önmaga a szám hossza (\0 nélkül)

    // Max 7 számjegy + 2 pont + \0 -> 10
    char *out = malloc(10);
    if (!out) return NULL;

    // az utolsó elem '\0'
    int outIndex = 9;
    out[outIndex] = '\0';

    // HÁTULRÓL ELŐRE MEGYÜNK
    int count = 0;
    for (int i = len - 1; i >= 0; i--) {

        // Minden 3. karakter után egy '.' karakter
        if (count == 3) {
            out[--outIndex] = '.';
            count = 0;
        }

        out[--outIndex] = num[i];
        count++; // count-ot növelni
    }

    // A teljes string elejére mutató pointer
    char *result = strdup(&out[outIndex]);

    free(out); // a nagy buffer már nem kell

    return result; // malloc-oltunk, utána free() !!!!!!!!!!
}



//! ---------- CSV BETÖLTÉS ----------

/*
    CÉL: Feleletválasztós kérdések betöltése fájlból
    CSV formátum: Nehézség;Kérdés;A;B;C;D;Válasz;Kategória
    KI: 0 - siker, 1 - hiba
*/
int loadChooseQuestions(        // BEMENET: 
    const char *filename,       // fájlnév
    ChooseQuestionArray *cqd,   // kérdés tömb
    int cqCounts[LEVELS]        // kérdésszámok szintenként
    ){
    FILE *fp;

    if ((fp = fopen(filename,"r")) == NULL)
    {
        printf("Fájl megnyitási hiba!");
        return 0;
    }

    char line[500]; // ebbe olvassuk a sorokat

    
    char *temp; // segédváltozó, a nehézségi szint konvertálásához
    int d; // Nehézség
    
    // a fájl első sorát kihagyjuk (fejléc)
    if (fgets(line, 500, fp ) == NULL) {
        printf("Olvasási hiba!"); // üres fájl esetén hiba
        fclose(fp);
        return 0;
    }

    // addig olvasunk, amíg van mit
    while(fgets(line, 500, fp) != NULL)
    {

        //? --- PONTOSVESSZŐ SZÁMLÁLÁS ---
        //? Ezzel kiszűrjük a hibás sorokat

        int pvesszo = 0;
        for (int i = 0; line[i]; i++)
            if (line[i] == ';') pvesszo++;

        if (pvesszo != 7)
            continue;   // hibás sor

        temp = strtok(line, ";"); // kiszedjük a sorból az első sztringet a ';' karakterig
        // FONTOS megadni a line paramétert, mert a továbbiakban innen folytatja a darabolást

        d = atoi(temp) - 1; // számmá konvertáljuk, mert ez a nehézségi szint

        // kihagyjuk a LEVELS-nél nagyobb nehézségi szinteket
        if (d < 0 || d >= LEVELS) continue;

        // csak MAX_CHOOSE_QUESTIONS_PER_LEVEL darabnyi kérdést tárolunk szintenként
        if (cqCounts[d] >= MAX_CHOOSE_QUESTIONS_PER_LEVEL) continue;

        // lementjük a nehézségi szintet
        // a (*cqd)-vel hivatkozunk a paraméterként kapott tömbre,
        // melynek első indexe (dimenziója) a nehézségi szint,
        // a második pedig a már betöltött kérdések száma adott szinten
        // tehát pl. (*cqd)[2][5] a 3. nehézségi szint 6. kérdése
        (*cqd)[d][cqCounts[d]].diff = d + 1;
        
        // strtok() tovább darabol
        // strdup() kell mivel a 'line' a következő lépésben átíródik
        (*cqd)[d][cqCounts[d]].q = strdup(strtok(NULL, ";"));
        (*cqd)[d][cqCounts[d]].answers[0] = strdup(strtok(NULL, ";"));
        (*cqd)[d][cqCounts[d]].answers[1] = strdup(strtok(NULL, ";"));
        (*cqd)[d][cqCounts[d]].answers[2] = strdup(strtok(NULL, ";"));
        (*cqd)[d][cqCounts[d]].answers[3] = strdup(strtok(NULL, ";"));
        (*cqd)[d][cqCounts[d]].correct = strdup(strtok(NULL, ";"));
        (*cqd)[d][cqCounts[d]].cat = strdup(strtok(NULL, "\n"));

        cqCounts[d]++;// növeljük a betöltött kérdések számát az adott szinten
        
    }

    fclose(fp);
    return 1;
}

/*
    CÉL: A választós kérdések felszabadítása
    BE:
     - cqd: a kérdéseket tartalmazó tömbre mutató pointer
     - cqCounts: a kérdések számát tartalmazó tömb
*/
void freeChooseQuestions(ChooseQuestionArray *cqd, int cqCounts[LEVELS]){
    ChooseQuestion current; // aktuális pointer egy kérdésre

    for (int d = 0; d < LEVELS; d++) { // "d" változó végigmegy a kérdés szinteken

        for (int i = 0; i < cqCounts[d]; i++) { // "i" változó végigmegy a kérdéseken szintenként
            current = (*cqd)[d][i];
            
            free(current.q);           // Felszabadítjuk a kérdés string-jét
            for (int a = 0; a < 4; a++) // felszabadítjuk a válaszok sztringjeit
                free(current.answers[a]);
            free(current.correct);     // felszabadítjuk helyes válasz sztringjét
            free(current.cat);         // felszabadítjuk a kategoria sztringjét
        }
    }

    free(cqd);
}

/*
    CÉL: Sorrendezős kérdések betöltése CSV-ből
    CSV formátum: Sorkérdés;A;B;C;D;Válasz;Kategória
    KI: 1 = siker, 0 = hiba (nem nyitható fájl)
*/
int loadOrderQuestions(                             // BEMENET: 
    const char *filename,                           // filenév
    OrderQuestion questions[MAX_ORDER_QUESTIONS],   // tömb [MAX_ORDER_QUESTIONS]
    int *orderCount                                 // eddig betöltött kérdések száma
){
    FILE *fp = fopen(filename, "r");
    if (!fp) return 0;

    char line[1024];

    if (!fgets(line, sizeof(line), fp)) { // header skip (biztosítsuk, hogy sikerült)
        fclose(fp);
        return 0;
    }

    // a current pointer az mindig az aktuális rekord
    OrderQuestion *current;

    // addig olvasunk, amíg van mit ÉS a darabszám nem lépi túl a Maximumot
    while (fgets(line, sizeof(line), fp) && *orderCount < MAX_ORDER_QUESTIONS)
    {

        //? --- PONTOSVESSZŐ SZÁMLÁLÁS ---
        //? Ezzel kiszűrjük a hibás sorokat

        int pvesszo = 0;
        for (int i = 0; line[i]; i++)
            if (line[i] == ';') pvesszo++;

        if (pvesszo != 6)
            continue;   // hibás sor

        current = &questions[*orderCount];
        
        current->q = strdup(strtok(line, ";"));
        current->answers[0] = strdup(strtok(NULL, ";"));
        current->answers[1] = strdup(strtok(NULL, ";"));
        current->answers[2] = strdup(strtok(NULL, ";"));
        current->answers[3] = strdup(strtok(NULL, ";"));
        current->correct = strdup(strtok(NULL, ";"));
        current->cat = strdup(strtok(NULL, "\n"));

        (*orderCount)++;
    }

    fclose(fp);
    return 1;
}

/*
    CÉL: A sorrendezős kérdések felszabadítása
    BE:
     - questions: a kérdések tömbje
     - count: hány kérdés lett betöltve
*/
void freeOrderQuestions(OrderQuestion questions[MAX_ORDER_QUESTIONS], int count)
{
    for (int i = 0; i < count; i++)
    {
        free(questions[i].q);
        for (int a = 0; a < 4; a++)   // négy elem felszabadítása
            free(questions[i].answers[a]);
        free(questions[i].correct);
        free(questions[i].cat);
    }

    // Itt nem kell magát a tömböt felszabadítani mert nem "calloc"-kal foglaltuk
}

//!----- DICSŐSÉGLISTA -----

/*
    CÉL: Eredmény mentése a dicsőséglista CSV végére
    FORMÁTUM: "Nev;Nyeremeny;Datum\n"
*/
void saveHighscore(const char* name, int prize) {
    FILE *fp = fopen("dicsoseglista.csv", "a+"); // append mód
    if (!fp) return; // hiba

    time_t t = time(NULL);  // Aktuális dátum lekérése
    struct tm tm = *localtime(&t);

    fseek(fp, 0, SEEK_END); // ugrás a fájl végére
    long size = ftell(fp);  // aktuális pozíció = fájl méret

    if (size == 0) {
        // Ha üres, csak akkor írjuk ki a fejlécet
        fprintf(fp, "Név;Nyerémeny;Dátum\n");
    }

    //Sor kiírása: Név;Összeg;ÉÉÉÉ/HH/NN
    fprintf(fp, "%s;%d;%04d/%02d/%02d\n",
            name,
            prize,
            tm.tm_year + 1900,
            tm.tm_mon + 1,
            tm.tm_mday);

    fclose(fp); // file bezárása
}


/* 
CÉL: Dicsőséglista szép, formázott kiírása (nyeremény szerint csökkenő sorrendben)
     - Beolvassuk az összes sor adatait egy dinamikus tömbbe
     - kiírjuk a rendezett listát, majd felszabadítjuk a memóriát
*/
void showHighscores(void) {
    FILE *fp = fopen("dicsoseglista.csv", "r"); // olvasás mód
    if (!fp) {
        printf("\n\t[!] Még nincs dicsőséglista.\n");
        return;
    }

    // Fejléc kiírása
    printf("\n\t- DICSŐSÉGLISTA -\n");
    printSeparator();
    printf(" %-22s | %-13s | %s\n", "Játékos", "Nyeremény", "Dátum");
    printSeparator();

    char line[100];

    // első sor átugrása (fejléc)
    if (!fgets(line, sizeof(line), fp)) {
        // üres fájl
        printf("\n\t[!] Nincs bejegyzés.\n");
        fclose(fp);
        return;
    }

    HighScore *entries = NULL;
    HighScore *temp = NULL;
    int capacity = 0;
    int count = 0;

    // Soronként olvasás és tárolás a dinamikus tömbbe
    while (fgets(line, sizeof(line), fp)) {

        // trim végéről
        line[strcspn(line, "\r\n")] = '\0';

        char *nameToken = strtok(line, ";");
        char *prizeToken = strtok(NULL, ";");
        char *dateToken = strtok(NULL, "\n"); // utolsó mező, sor végéig

        // Ha valami hiányzik, lépjünk tovább
        if (!nameToken || !prizeToken || !dateToken)
            continue;

        // Ha nagyobb a darabszám mint a kapacitás 
        if (count + 1 > capacity) {
                int new_capacity;

                if (capacity == 0)
                    new_capacity = 16;
                else
                    new_capacity = capacity + 16; // dinamikus tömb növelése szükség szerint

            temp = realloc(entries, new_capacity * sizeof(HighScore));

            // Ha temp NULL, akkor a realloc nem sikerült, amit eddit lefoglaltunk az "entries"-ben, azt felszabadítjuk
            if (!temp) {
                for (int i = 0; i < count; i++) {
                    free(entries[i].name);
                    free(entries[i].date);
                }
                free(entries);
                fclose(fp);

                printf("\n\t[!] Memória hiba.\n");
                waitEnter();
                return;
            }

            entries = temp;
            capacity = new_capacity;
        }

        // Tárolás (strdup-olunk, mert strtok a line-t módosítja)
        entries[count].name = strdup(nameToken);
        entries[count].prize = atoi(prizeToken); // int-ként kezeljük
        entries[count].date = strdup(dateToken);
        count++;
    }

    fclose(fp);

    if (count == 0) {
        printf("\n\t[!] Nincs bejegyzés.\n");
        free(entries);
        return;
    }

    // Végigmegyünk a dicsőséglistán egy i változóval
    for (int i = 0; i + 1 < count; i++) {
        
        int legnagyobb = i; // legyen i a legnagyobb

        // A tömb többi része (i+1-től a végéig)
        for (int j = i + 1; j < count; j++) {
            
            // ha a j. elem nyereménye nagyobb mint az i. elemé, akkor indexet csere, legyen az a legnagyobb.
            if (entries[j].prize > entries[legnagyobb].prize) {
                legnagyobb = j; 
            }
        }

        // Csere: Ha találtunk nagyobb elemet, mint ami az i. helyen volt.
        // Szimpla 3-as csere egy temp változóval
        if (legnagyobb != i) {
            HighScore temp = entries[i];
            entries[i] = entries[legnagyobb];
            entries[legnagyobb] = temp; 
        }
    }

    // Kiírás
    for (int i = 0; i < count; i++) {
        
        char *s = formatPrize(entries[i].prize);
        printf(" %-20s | %9s Ft | %s\n", entries[i].name, s, entries[i].date);
        free(s);

    }

    printSeparator();

    // Felszabadítás
    for (int i = 0; i < count; i++) {
        free(entries[i].name);
        free(entries[i].date);
    }
    free(entries);
}

//! ---------- JÁTÉK LOGIKA ----------

/*
    CÉL: Megmondja, hogy egy szint "biztos szint"-e
    BE: Az aktuális level index
    KI: 1 ha safe, 0 ha nem
*/
int isSafeLevel(int index) {
    return (index == 4 || index == 9); // 5. és 10. kérdés
}

/*
    CÉL: Visszaadja a garantált nyereményt kiesés esetén
    BE: index - az a szint, AHONNAN kiesett (0-alapú index)
*/
int getSafePrize(int index) {
    if (index >= 9) return PRIZE_LADDER[9];
    if (index >= 4) return PRIZE_LADDER[4];
    return 0;
}

//! ---------- SEGÍTSÉGEK ----------

/*
    CÉL: 50:50 segítség
     - Két rossz választ véletlenszerűen "kikapcsol"
    BE:
     - q: aktuális kérdés
     - removed[2]: ide írjuk be a KIKAPCSOLT válaszindexeket (0..3)
 */
void print5050(ChooseQuestion *q, int hidden[2]) {
    int correctIdx = q->correct[0] - 'A'; // A helyes válasz indexe (A=0, B=1, C=2, D=3)
    int count = 0;

    // Kezdetben nincs eltávolított index
    hidden[0] = -1;
    hidden[1] = -1;

    while (count < 2) {
        int r = rand() % 4; // 0, 1, 2, 3

        // Csak akkor jó, ha:
        // - nem a helyes válasz
        // - még nem vettük ki korábban
        if (r != correctIdx && r != hidden[0]) {
            hidden[count++] = r;
        }
    }
}

/*
    CÉL: Közönség segítsége
    LOGIKA:
     - A helyes válasz kap 30-60% közötti arányt
     - A maradék százalék véletlenszerűen elosztva a többiek között
*/
void printAudience(ChooseQuestion *q) {
    int correctIdx = q->correct[0] - 'A'; // A helyes válasz indexe (A=0, B=1, C=2, D=3)
    int percentages[4] = {0}; // %-ok listája
    int remaining = 100; // Kezdetben elosztandó %

    // Helyes válasz: 30-60%
    percentages[correctIdx] = 30 + (rand() % 31); // 30 + 0...30
    remaining -= percentages[correctIdx];

    // Maradék elosztása
    for (int i = 0; i < 4; i++) {
        if (i == correctIdx)
            continue; // Helyes átugrása
        if (i == 3) {
            percentages[i] = remaining; // Utolsónak mindent
        } else {
            // Random % 0 és remaining között
            int temp = rand() % (remaining + 1);
            percentages[i] = temp;
            remaining -= temp;
        }
    }
    
    // Kiírás
    printf("\nA KÖZÖNSÉG SZAVAZATA:\n");
    printf("  A: %d%%  B: %d%%  C: %d%%  D: %d%%\n", percentages[0], percentages[1], percentages[2], percentages[3]);
}
