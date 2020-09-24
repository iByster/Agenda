#pragma once
#include "Controller.h"
#include "Activity.h"

class Console {
	Controller& ctr;
	/*
	Citeste datele de la tastatura si adauga Pet
	arunca exceptie daca: nu se poate salva, nu e valid
	*/
	void addUI();
	/*
	Tipareste o lista de animale la consola
	*/
	void printUI(const vector<Activity>& activities);
	/*
	Sterge o activitate dupa titlu si descriere
	*/
	void deleteUI();
	/*
	Modifica o activitate
	*/
	void modifyUI();
	/*
	Cauta o activitate
	*/
	void searchUI();
	/*
	Filtreaza activitatiile dupa tip sau descriere
	*/
	void filterUI();
	/*
	Sorteaza activitatiile dupa titlu, descrire, tipul + durata
	*/
	void sortUI();
	
	void statisticUI();

	void menuCurrentListUI();

	void menuCurrentListAddUI();

	void menuCurrentListExportUI();

	void menuCurrentListDestroyUI();

	void menuCurrentListGenerateUI();

	


public:
	Console(Controller& ctr) :ctr{ ctr }{}

	Console(const Console& ot) = delete;

	void run();
};