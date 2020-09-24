#pragma once
#include <iostream>
using namespace std;

template <typename T>
struct Nod
{
	T val;
	Nod* urm;
	Nod(T v) : val{ v }, urm{ nullptr }{};
	Nod(T v, Nod* urm) : val{ v }, urm{ urm }{};
};

template <typename T>
class MyListIterator;

template <typename T>
class MyList {
	Nod<T>* cap;
	/*
	dealoca toate nodurile
	*/
	void elibereazaNoduri();

	/*
	Face o copie a listei inlantuite
	Parcurge recursiv lista si creaza noduri in care copieaza valorile
	*/
	Nod<T>* copieaza(Nod<T>* curent);

public:

	MyList() :cap{ nullptr } {};

	/*
	 Constructor de copiere
	 - apelat automat la transmitere prin valoare, la return prin valoare, explicit ex: MyList l2{l1};

	 Rule of three
	*/
	MyList(const MyList& ot);

	/*
	Operator de assignment
	- apelat automat la expresii a = b; unde a si b sunt de tip MyLista
	Rule of three
	*/

	void operator=(const MyList& ot);

	/*
	 Destructor - apelat cand obiectul este distrus (out of scope sau delete)
	 Elibereaza memoria pentru fiecare nod
	 Rule of three
	*/
	~MyList();

	/*
	Returneaza numarul de elemente
	*/
	size_t size() const;


	/*
	Adauga la inceput
	*/
	void addInceput(T v);

	/*
	adauga la sfarsit
	*/
	void push_back(T v);

	/*
	Sterge un element
	*/
	bool sterge(T v);

	/*
	Primul element
	*/
	T& first() const;

	/*
	Returneaza un iterator pozitionat pe primul element
	E nevoie de el ex. daca vrem sa folosim lista in range for
	*/
	MyListIterator<T> begin() const;

	/*
	Returneaza un iterator cu nodul nullptr (dupa ultimul element)
	*/
	MyListIterator<T> end() const;

	/*
	Acces dupa pozitie
	Operatie ineficienta, trebuie sa parcurg lista pana la poz
	*/
	T& operator[](size_t poz);
};

template <typename T>
MyList<T>::MyList(const MyList& ot) {
	//obictul acum se creeaza(constructor)
	//copiez din ot in this
	cap = copieaza(ot.cap);
}

template <typename T>
void MyList<T>::operator=(const MyList& ot) {
	//poaste this contine ceva deja
	elibereazaNoduri();
	//copiez din ot in this
	cap = copieaza(ot.cap);
}

/*
Destructor - apelat cand obiectul este distrus (out of scope sau delete)
Rule of three
*/
template <typename T>
MyList<T>::~MyList() {
	elibereazaNoduri();
}

template <typename T>
Nod<T>* MyList<T>::copieaza(Nod<T>* curent) {
	if (curent == nullptr) {
		return nullptr;
	}

	auto n = new Nod<T>{ curent->val };
	n->urm = copieaza(curent->urm);
	return n;
}

template <typename T>
void MyList<T>::addInceput(T v) {
	Nod<T>* n = new Nod<T>{ v, cap };
	cap = n;
}

template <typename T>
void MyList<T>::push_back(T v) {
	auto nCurent = cap;
	while (nCurent != nullptr && nCurent->urm != nullptr) {
		nCurent = nCurent->urm;
	}

	if (nCurent == nullptr) {
		cap = new Nod<T>{ v, nullptr };
	}
	else {
		nCurent->urm = new Nod<T>{ v, nullptr };
	}
}

template <typename T>
bool MyList<T>::sterge(T v) {
	if (cap == nullptr) return false;
	if (cap->val == v) {
		auto aux = cap;
		cap = cap->urm;
		delete aux;
		return true;
	}
	else {
		auto q = cap;
		bool gasit = false;

		while (q != nullptr && !gasit) {
			if (q->urm != nullptr) {
				if (q->urm->val == v) {
					gasit = true;
				}
			}

			if (!gasit) {
				q = q->urm;
			}

		}
		if (gasit == true) {
			Nod<T>* found = q->urm;
			q->urm = found->urm;

			delete found;
			return true;

		}


	}

	return false;
}

template <typename T>
size_t MyList<T>::size() const {
	auto nCurent = cap;
	int lg = 0;
	while (nCurent != nullptr) {
		lg++;
		nCurent = nCurent->urm;
	}
	return lg;
}

template <typename T>
T& MyList<T>::operator[](size_t poz) {
	auto nCurent = cap;
	int lg = 0;
	while (lg < poz) {
		lg++;
		nCurent = nCurent->urm;
	}

	return nCurent->val;
}

template <typename T>
T& MyList<T>::first() const {
	return cap->val;
}

template <typename T>
void MyList<T>::elibereazaNoduri() {
	auto nCurent = cap;
	while (nCurent != nullptr) {
		auto aux = nCurent->urm;
		delete nCurent;
		nCurent = aux;
	}
	cap = nullptr;
}

/*
  Iterator pentru lista inlantuita
*/
template<typename T>
class MyListIterator {
	//pozitia curenta - este nullptr daca am ajuns la sfarsitul listei
	Nod<T>* current;
public:
	MyListIterator(Nod<T>* c) :current{ c } {
	}

	/*
	Este nevoie de operator != pentru range for
	*/
	bool operator!=(const MyListIterator& ot);

	/*
	trece la urmatorul element din interatie
	Este nevoie de operator ++ pentru range for
	*/
	void operator++();
	/*
	Returneaza elementul curent din interatir
	Este nevoie de operator * pentru range for
	*/
	T& operator*();
};


template <typename T>
MyListIterator<T> MyList<T>::begin() const {
	return MyListIterator<T>{ cap };
}


template <typename T>
MyListIterator<T> MyList<T>::end() const {
	return MyListIterator<T>{ nullptr };
}


/*
Este nevoie de operator != pentru range for
*/
template <typename T>
bool MyListIterator<T>::operator!=(const MyListIterator& ot) {
	return current != ot.current;
}

/*
trece la urmatorul element din interatie
Este nevoie de operator ++ pentru range for
*/
template <typename T>
void MyListIterator<T>::operator++() {
	current = current->urm;
}
/*
Returneaza elementul curent din interatir
Este nevoie de operator * pentru range for
*/
template <typename T>
T& MyListIterator<T>::operator*() {
	return current->val;
}