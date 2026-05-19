**Készítsen** heterogén kollekciók tárolásához sablon osztályt (**ClonableHeteroStore**), 
ami egy belső *sorozattároló* segítségével tárolja a heterogén gyűjteményhez tartozó objektumok pointereit! 
Sablonparaméterként vegye át a heterogén kollekció alaposztályát, a tároláshoz használt osztályt, valamint egy kivétel osztályt, amit hiba esetén eldob (ld. később)! Amennyiben ez nincs megadva, akkor az *std::out_of_range* kivételt dobja! 
Amennyiben a tároló osztályt sem adják meg, akkor azt az *std::vector* sablonból hozza létre!
Az alábbi kódrészlet a sablon példányosítására mutat néhány példát:

    ClonableHeteroStore<Ember>  t1;  
    ClonableHeteroStore<Ember>, std::deque<Ember*>  t2;
    ClonableHeteroStore<Valami, std::list<Valami*>, MyClass>  t3;

Ahol:

- t1 - *Ember\** típusú pointereket tárol *std::vector* sablon segítségével. Hiba esetén *std::out_of_range* kivételt dob.
- t2 - *Ember\** típusú pointereket tárol *std::deque* sablon segítségével. Hiba esetén *std::out_of_range* kivételt dob.
- t3 - *Valami\** típusú pointereket tárol *std::list* sablon segítségével. Hiba esetén *MyClass* kivételt dob.
   
Minimumkövetelmények a tárolásra használt osztályra vonatkozóan:

- A paraméterként átadott tárolótól elvárjuk, hogy legyen: 
	- létrehozható, megszüntethető, másolható, értékadható. 
- A paraméterként átadott tárolótól elvárjuk, hogy valósítsa meg az alábbi metódusokat illetve típusokat:
    - *value\_type* - a tárolt adat típusa.
	- *iterator* - iterátor. 
	- *const\_iterator* - konstans iterátor.
    - *begin()*, *end()* - visszaad egy iterátort, ami az első adatra, illetve az utolsó adat után mutat.
	- *size()* - visszaadja a tárolt elemek számát.
	- *push\_back()* - betesz egy új adatot a tárolóba - pontosabban az adat egy másolatát - a tárolt adatok után. Amennyiben helyhiány miatt ez nem sikerül, *std::bad_alloc* kivételt dob.
	- *clear()* - törli a tárolót, azaz eldobja a tárolt adatokat, a tárolt darabszámot 0-ra állítja.

A fenti követelmények meglétét feltételezve **tervezze** meg a *ClonableHeteroStore* osztályt úgy, hogy az rendelkezzen az alábbi publikus metódusokkal, illetve illetve típusokkal:

- **const_iterator** - konstans iterátor.
- **begin()** - visszaad egy iterátort (*const\_iterator*) a belső tároló első elemére.
- **end()** - visszaad egy iterátort (*const\_iterator*), ami a belső tárolóban tárolt utolsó elem után mutat.
- **size_t size()** - megadja a tárolt pointerek számát
- **void add(p)** - beteszi a tárolóba a paraméterként kapott pointert. 
  Amennyiben a tárolóba már nem tudja betenni a pointert, dobjon kivételt! 
- **void clear()** - törli (felszabadítja) a tárolóban tárolt objektumokat.
   
Legyenek a **ClonableHeteroStore** osztályból létrehozott objektumok másolhatók és szerepelhessenek értékadás bal oldalán is!
Támogassa a többszörös értékadást! **Tételezze fel**, hogy a tárolóban tárolt heterogén gyűjtemény osztályainak van **clone()** metódusa, ami egy teljes másolatot (deep copy) készít az adott objektumról.

**Feladatok:**

1. Töltse le az előkészített projektet a tárolóból! 
   [https://git.iit.bme.hu/prog2/ell_feladat/ClonableHetero](https://git.iit.bme.hu/prog2/ell_feladat/ClonableHetero)
2. Készítse el a **ClonableHeteroStore** osztályt a *clonable_hetero_store.hpp* állományban!
3. Tesztelje a megadott tesztprogrammal az új osztályt!
4. Elemezze a **ClonableTestClass**, a **ClonableTestClass1**, valamint a **ClonableTestClass2** osztályok megvalósítását a *clonable_testclass.h* állományban! A megvalósítás többszörös öröklést és virtuális alaposztályt használ. Értse meg, hogy miért van erre szükség!
5. A fentiek alapján hozzon létre klónozható baktériumokat, melyek kompatibilisek a **Bacterium**, valamint **Clonable** osztályokkal! Az osztályokat a *clonable_bacterium.h* fájlban implementálja!
6. Projekt szinten definiálja a **BACI\_IS\_VAN** makrót, fordítson és ismét teszteljen!   
4. Amennyiben a tesztek jól futnak, töltse fel a *JPorta* feladatbeadó rendszerbe a **clonable\_bacterium.h** és **clonable\_hetero\_store.hpp** fájlokat!

**Megjegyzések:**

- Dinamikusan keletkező objektumok esetében fontos kérdés, hogy ki hozza létre és ki szünteti meg az objektumot, azaz mikor ki felel az objektumért. Ügyeljen arra, hogy a tárolónak átadott **"objektumért" a tároló felel**. Még akkor is, ha nem tudta elhelyezni azt a  tárolóban.
- Azzal, hogy a tárolt elemek iterátorral is elérhetők, nincs szükség külön bejáró (traverse) tagfüggvényre. A tároló az iterátor segítségével egyszerűen bejárható. 
