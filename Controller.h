#pragma once
#include "Activity.h"
#include "Repository.h"
#include <string>
#include <numeric>
#include <functional>
#include "Validator.h"
#include <map>
#include "Undo.h"
#include "Observer.h"
using std::unique_ptr;
using std::map;

using std::function;

class Controller : public Observable {
	DefaultRepo& repo;
	ValidateActivity& val;
	std::vector<unique_ptr<ActiuneUndo>> undoActions;
	std::vector<unique_ptr<ActiuneUndo>> undoActions2;

public:
	Controller(DefaultRepo& repo, ValidateActivity& val) :repo{ repo }, val{ val }{}
	//nu permitem copierea de obiecte PetStore
	Controller(const Controller& ot) = delete;
	~Controller() = default;
	/*
	 returneaza toate animalele in ordinea in care au fost adaugate
	*/
	//~Controller();

	vector<Activity> getAll() {
		return repo.getAll();
	}

	vector<Activity> getAll2() {
		return repo.getAll2();
	}


	/*
	Adauga un nou pet
	arunca exceptie daca: nu se poate salva, nu este valid
	*/
	void ctrlAddActivity(const string& title, const string& descr, const string& type, int time);

	void ctrlDeleteActivity(const string& title);

	void ctrlModifyActivity(const string& title, const string& modify, int cmd);

	const Activity& ctrlSearch(const string& title);

	//MyList<Activity> filter(function<bool(const Activity&)> fct);

	vector<Activity> filterDescription(string desc);

	vector<Activity> filterType(string type);

	vector<Activity> generalSort(bool(*maiMicF)(const Activity&, const Activity&));
	
	vector<Activity> sortByTitle();

	vector<Activity> sortByDesc();

	vector<Activity> sortByTypeDuration();

	vector<DTO> statistica();

	//vector<Activity> ctrlAddCurrentActivity1(const string& title);

	void ctrlAddCurrentActivity2(const string& title);

	void ctrlDestroyCurrentList();

	void ctrlGenerateCurrentList(int i);

	int ctrlTotalTime();

	void undo();

	void undo2();

};

void testsCtr();
