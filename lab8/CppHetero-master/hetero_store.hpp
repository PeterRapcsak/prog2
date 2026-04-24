#ifndef HETEROSTORE_HPP
#define HETEROSTORE_HPP

#include <iostream>
#include <stdexcept>
#include <cstddef>

template <typename T, std::size_t N = 100, typename Exception = std::out_of_range>
class HeteroStore {
	T* tarolo[N];   // tarolt pointerek
	std::size_t db; // aktualis elemszam

	// Kivetel dobasa megteleskor.
	void throwFull() {
		throw Exception("Nincs hely a taroloban");
	}

    /// Végül nem overloadoljuk a throwFull-t :>
	// std::out_of_range specialis kezelese.
	// void throwFull(std::out_of_range*) {
	//	throw std::out_of_range("Nincs hely a taroloban");
	// }

public:
	// Konstruktor: ures tarolo.
	HeteroStore() : db(0) {
		for (std::size_t i = 0; i < N; ++i)
			tarolo[i] = NULL;
	}

	// Destruktor
	~HeteroStore() {
		clear();
	}

	// Tarolt pointerek szama
	std::size_t size() const {
		return db;
	}

	// Tarolo kapacitasa.
	std::size_t capacity() const {
		return N;
	}

	// Uj pointer beszurasa.
	void add(T* p) {
		if (db >= N) {
			delete p;
			throwFull(); // Csak simán throwFull()
		}
		tarolo[db++] = p;
	}

	// Vegigjaras
	template <typename Func>
	void traverse(Func& func) { // Funktor referencia
		for (std::size_t i = 0; i < db; ++i)
			func(tarolo[i]); // Ez dönti el hogy mit csinalunk a pointerekkel
	}

	// Tarolo uritese.
	void clear() {
		for (std::size_t i = 0; i < db; ++i) {
			delete tarolo[i];
			tarolo[i] = NULL;
		}
		db = 0; // Üresre állítás
	}

private:
	// Masolo konstruktor tiltasa.
	HeteroStore(const HeteroStore&);

	// Ertekadas tiltasa.
	HeteroStore& operator=(const HeteroStore&);
};


#endif // HETEROSTORE_HPP
