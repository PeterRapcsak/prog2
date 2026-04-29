**Készítsen** generikus algoritmust (*monoton*) annak eldöntésére, hogy egy iterátorokkal adott jobbról nyílt intervallum elemei monoton sorozatot alkotnak-e. Az algoritmus harmadik paramétere egy predikátum! 
Ezzel a kétoperandusú predikátummal adható meg egy adott elem és az őt megelőző elem viszonya a sorozaton belül.
A sablon használatára mutat példát az alábbi kódrészlet: 

        std::vector<int> v;
		...
		if (monoton(v.begin(), v.end(), std::greater<int>()))
			std::cout << "Ez a sorozat szigorúan monoton növekvő\n";
		else if (monoton(v.begin(), v.end(), std::greater_equal<int>()))
			std::cout << "Ez a sorozat monoton növekvő\n";
		else if (monoton(v.begin(), v.end(), std::less<int>()))
			std::cout << "Ez a sorozat szigorúan monoton csökkenő\n";
		else if (monoton(v.begin(), v.end(), std::less_equal<int>()))
			std::cout << "Ez a sorozat monoton csökkenő\n";
		else 
			std::cout << "Ez a sorozat nem monoton\n";

**Feladatok:**

1. Töltse le az előkészített projektet az SVN tárolóból! 
   [https://git.iit.bme.hu/prog2/ell_feladat/CppMonoton](https://git.iit.bme.hu/prog2/ell_feladat/CppMonoton)
2. Készítse el a **monoton** sablont a *monoton.hpp* állományban! Az algoritmust úgy valósítsa meg hogy csak az 
ún. *Input Iterator*-okra jellemző műveleteket használja (konstruktor, másoló, értékadó, ++, ==, != ++, *, ->). 
3. Tesztelje a megadott tesztprogrammal a sablont!
4. Amennyiben a tesztek jól futnak, töltse fel a *JPorta* feladatbeadó rendszerbe a **monoton.hpp** fájlt!
