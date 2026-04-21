**Készítsen** heterogén kollekciók tárolásához sablon osztályt (**HeteroStore**)! 
Az osztály fix méretű belső tömbben tárolja a heterogén gyűjteményhez tartozó objektumok pointereit! 
Sablonparaméterként vegye át az alaposztály típusát, a belső tároló méretét, 
valamint egy kivétel osztályt, amit hiba esetén eldob (ld. később)! 
Amennyiben ez nincs megadva, akkor az *std::out_of_range* kivételt dobja! 
Ha a méretet sem adják meg, akkor azt 100 értékűnek vegye! 
Az alábbi kódrészlet a sablon példányosítására mutat néhány példát:

    HeteroStore<Ember>  t1;  
    HeteroStore<Ember, 3>  t2;
    HeteroStore<Valami, 4, MyClass>  t3;

Ahol:

- t1 - 100 elemű tároló, ami *Ember\** típusú pointert tárol. Ha betelik, *std::out_of_range* kivételt dob.
- t2 - 3 elemű tároló, ami *Ember\** típusú pointert tárol. Ha betelik, std::out_of_range kivételt dob.
- t3 - 4 elemű tároló, ami  *Valami\** típusú pointert tárol. Ha betelik, *MyClass* kivételt dob.
   
**Valósítsa** meg a *HeteroStore* osztályban az alábbi metódusokat:

- **size_t size()** - megadja a tárolt pointerek számát
- **size_t capacity()** - megadja a tároló maximális méretét.
- **void add(p)** - beteszi a tárolóba a paraméterként kapott pointert. 
  Amennyiben a tárolóba már nem tudja betenni a pointert, dobjon kivételt! 
- **void traverse(func)** - a tároló minden elemével (pointerrel) meghívja a paraméterként 
  kapott funktort (func).
- **void clear()** - törli (felszabadítja) a tárolóban tárolt objektumokat.
   
Ne legyenek a **HeteroStore** osztályból létrehozott példányok másolhatók és értékadás műveletük 
se legyen elérhető!

**Feladatok:**

1. Töltse le az előkészített projektet a tantárgy git tárolójából! 
   [https://git.ik.bme.hu/Prog2/ell_feladat/CppHetero](https://git.ik.bme.hu/Prog2/ell_feladat/CppHetero)
2. Készítse el a **HeteroStore** osztályt a *hetero_store.hpp* állományban! 
3. Tesztelje a megadott tesztprogrammal az új osztályt! 
4. Amennyiben a tesztek jól futnak, töltse fel a *JPorta* feladatbeadó rendszerbe a **hetero\_store.hpp** fájlt!

**Megjegyzések:**

- Dinamikusan keletkező objektumok esetében fontos kérdés, hogy ki hozza létre és ki szünteti meg az objektumot, 
  azaz mikor ki felel az objektumért. Ügyeljen arra, hogy a tárolónak átadott **"objektumért" a tároló felel**. 
  Még akkor is, ha nem tudta elhelyezni azt a  tárolóban.
- A **traverse** tagfüggvény maga is egy sablon az osztályon belül. Megvalósítása formailag hasonlít az 
  iterátoros konstruktorok megvalósításához. 

**GIT Submodule kezelése**
- `git clone X` után:
- `git submodule update --init`

vagy:
- `git clone --recurse-submodules X`