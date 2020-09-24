#include "Controller.h"
#include <functional>
#include <algorithm>
#include <assert.h>
#include <random>
#include <fstream>
#include "Undo.h"

void Controller::ctrlAddActivity(const string& title, const string& descr, const string& type, int time) {
	double probabilitate = (rand() % 100) * 1.0 / 100;
	if (probabilitate < repo.getAll_err())
		throw RepositoryException("Chaos");
	Activity a{ title, descr, type, time };
	val.validate(a);
	repo.store(a);
	undoActions.push_back(std::make_unique<UndoAdauga>(repo, a));
}



void Controller::ctrlDeleteActivity(const string& title){
	double probabilitate = (rand() % 100) * 1.0 / 100;
	if (probabilitate < repo.getAll_err())
		throw RepositoryException("Chaos");
	Activity a = repo.search(title);
	repo.deleteAc(a);
	undoActions.push_back(std::make_unique<UndoSterge>(repo, a));

}

void Controller::ctrlModifyActivity(const string& title, const string& modify, int cmd) {
	double probabilitate = (rand() % 100) * 1.0 / 100;
	if (probabilitate < repo.getAll_err())
		throw RepositoryException("Chaos");
	Activity a = repo.search(title);
	Activity aux = repo.search(title);
	repo.modify(a, cmd, modify);
	if (cmd == 1) {
		undoActions.push_back(std::make_unique<UndoModifica>(repo, a, aux.getDesc(), cmd));
	}
	if (cmd == 2) {
		undoActions.push_back(std::make_unique<UndoModifica>(repo, a, aux.getType(), cmd));
	}
	if (cmd == 3) {
		undoActions.push_back(std::make_unique<UndoModifica>(repo, a, to_string(aux.getTime()), cmd));
	}
	
}

const Activity& Controller::ctrlSearch(const string& title) {
	return repo.search(title);
}

/*
MyList<Activity> Controller::filter(function<bool(const Activity&)> fct) {

	MyList<Activity> rez;
	for (const auto& act : repo.getAll()) {
		if (fct(act)) {
			rez.push_back(act);
		}
	}
	return rez;
}
*/
vector <Activity> Controller::filterDescription(string desc){
	vector<Activity> rez;
	vector<Activity> all_list = repo.getAll();
	std::copy_if(all_list.begin(), all_list.end(), std::back_inserter(rez), [desc](const Activity& a) {
		return a.getDesc() == desc;
		});
	return rez;

}

vector <Activity> Controller::filterType(string type) {
	vector<Activity> rez;
	vector<Activity> all_list = repo.getAll();
	std::copy_if(all_list.begin(), all_list.end(), std::back_inserter(rez), [type](const Activity& a) {
		return a.getType() == type;
		});
	return rez;
}

vector <Activity> Controller::generalSort(bool(*maiMicF)(const Activity&, const Activity&))
{
	vector<Activity> v = repo.getAll();
	for (size_t i = 0; i < v.size() - 1; i++) {
		for (size_t j = i + 1; j < v.size(); j++) {
			if (!maiMicF(v[i], v[j])) {
				Activity aux = v[i];
				v[i] = v[j];
				v[j] = aux;
			}
		}
	}

	return v;
}

vector<Activity> Controller::sortByTitle()
{
	vector<Activity> v = repo.getAll();
	sort(v.begin(), v.end(), cmpTitle);
	return v;
}

vector <Activity> Controller::sortByDesc()
{
	vector<Activity> v = repo.getAll();
	sort(v.begin(), v.end(), cmpDesc);
	return v;
}

vector <Activity> Controller::sortByTypeDuration()
{
	return generalSort([](const Activity& a1, const Activity& a2) {
		if (a1.getType() == a2.getType()) {
			return a1.getTime() < a2.getTime();
		}
		return a1.getType() < a2.getType();
		});
}


vector<DTO> Controller::statistica()
{
	auto activitati = repo.getAll();
	int nr_activitati = repo.getAll().size();
	map<string, DTO> statistica;

	for (auto act : activitati) {
		if (statistica.find(act.getType()) == statistica.end()) {
			DTO nou {act.getType()};
			statistica.insert({act.getType(), nou});
		}
		else {
			DTO aux = statistica[act.getType()];
			aux.increment();
			statistica[act.getType()] = aux;
		}

	}

	map<string, DTO>::iterator it = statistica.begin();
	vector <DTO> output;

	while (it != statistica.end()) {
		it->second.set_procent(nr_activitati);
		output.push_back(it->second);
		it++;
	}

	return output;
}
/*
vector<Activity> Controller::ctrlAddCurrentActivity1(const string& title){
	return repo.sameTitle(title);
}
*/
void Controller::ctrlAddCurrentActivity2(const string& title){
	Activity act = repo.search(title);
	repo.storeCurr(act);
	undoActions2.push_back(std::make_unique<UndoAdaugaCurr>(repo, act));

}

void Controller::ctrlDestroyCurrentList(){
	repo.destroy();
}

void Controller::ctrlGenerateCurrentList(int i){

	vector<Activity> list = repo.getAll();

	std::mt19937 mt{ std::random_device{}() };
	std::uniform_int_distribution<> dist(0, list.size() - 1);
	

	while (i) {
		int rndNr = dist(mt);// numar aleator intre [0,size-1]

		try {
			ctrlAddCurrentActivity2(list[rndNr].getTitle());
		}
		catch (const RepositoryException & ex) {
			i++;
		}
		i--;
	}
}

int Controller::ctrlTotalTime(){
	vector<Activity> v = repo.getAll();
	return std::accumulate(v.begin(), v.end(), 0, [](int durata, Activity act) {return act.getTime() + durata; });
}

void Controller::undo(){
	if (undoActions.empty()) {
		throw ActivityException{ "Nu mai exista operatii" };
	}

	undoActions.back()->doUndo();
	undoActions.pop_back();
}

void Controller::undo2() {
	if (undoActions2.empty()) {
		throw ActivityException{ "Nu mai exista operatii" };
	}

	undoActions2.back()->doUndo();
	undoActions2.pop_back();
}





void testCtrlAddDelete() {
	Repository rep;
	ValidateActivity val;
	Controller ctr{ rep,val };
	ctr.ctrlAddActivity("a", "a", "a", 6);
	assert(ctr.getAll().size() == 1);
	ctr.ctrlDeleteActivity("a");
	assert(ctr.getAll().size() == 0);

	//adaug ceva invalid
	try {
		ctr.ctrlAddActivity("", "", "", -1);
		assert(false);
	}
	catch (ValidateException&) {
		assert(true);
	}
	//incerc sa adaug ceva ce existadeja
	try {
		ctr.ctrlAddActivity("a", "a", "a", -1);
		assert(false);
	}
	catch (ValidateException&) {
		assert(true);
	}
	
	try {
		ctr.ctrlDeleteActivity("a");
		assert(false);
	}
	catch (RepositoryException&) {
		assert(true);
	}
	
}

void testCtrlModify() {
	Repository rep;
	ValidateActivity val;
	Controller ctr{ rep,val };
	ctr.ctrlAddActivity("a", "a", "a", 6);
	ctr.ctrlModifyActivity("a", "b", 1);
	assert(rep.search("a").getDesc() == "b");
}

void testCtrlSearch() {
	Repository rep;
	ValidateActivity val;
	Controller ctr{ rep,val };
	ctr.ctrlAddActivity("a", "a", "a", 6);
	assert(ctr.ctrlSearch("a").getTime() == 6);
}

void testFilter() {
	Repository rep;
	ValidateActivity val;
	Controller ctr{ rep,val };
	ctr.ctrlAddActivity("a", "c", "a", 6);
	ctr.ctrlAddActivity("b", "b", "l", 6);
	ctr.ctrlAddActivity("c", "c", "k", 6);
	ctr.ctrlAddActivity("d", "c", "l", 6);
	assert(ctr.filterType("l").size() == 2);
	assert(ctr.filterType("k").size() == 1);
	assert(ctr.filterDescription("c").size() == 3);

}

void testSort() {
	Repository rep;
	ValidateActivity val;
	Controller ctr{ rep,val };
	ctr.ctrlAddActivity("d", "y", "a", 6);
	ctr.ctrlAddActivity("c", "x", "l", 4);
	ctr.ctrlAddActivity("b", "w", "k", 3);
	ctr.ctrlAddActivity("a", "z", "l", 5);

	auto firstAct = ctr.sortByTitle().front();
	assert(firstAct.getTitle() == "a");

	firstAct = ctr.sortByDesc().front();
	assert(firstAct.getDesc() == "w");

	firstAct = ctr.sortByTypeDuration().front();
	assert(firstAct.getType() == "a");
	
}

void testStatistica() {
	Repository rep;
	ValidateActivity val;
	Controller ctr{ rep,val };
	ctr.ctrlAddActivity("d", "y", "a", 6);
	ctr.ctrlAddActivity("c", "x", "l", 4);
	ctr.ctrlAddActivity("b", "w", "k", 3);
	ctr.ctrlAddActivity("a", "z", "l", 5);

	auto rez = ctr.statistica();

	assert(rez.size() == 3);
	for (auto s : rez) {
		string x = s.get_str();
		assert(x.size() > 0);
	}
	

}

void testCtrlCurrent() {
	Repository rep;
	ValidateActivity val;
	Controller ctr{ rep,val };
	ctr.ctrlAddActivity("a", "a", "a", 6);
	ctr.ctrlAddActivity("b", "b", "a", 6);
	ctr.ctrlAddActivity("c", "c", "a", 6);
	ctr.ctrlAddActivity("d", "d", "d", 6);
	ctr.ctrlAddActivity("e", "e", "e", 6);
	ctr.ctrlAddActivity("f", "f", "f", 6);

	//assert(ctr.ctrlAddCurrentActivity1("a").size() == 3);
	//assert(ctr.ctrlAddCurrentActivity1("f").size() == 1);

	vector<Activity> listAux = rep.getAll();
	

	ctr.ctrlGenerateCurrentList(4);

	vector<Activity> listAuxCurr = rep.getAll2();

	assert(listAuxCurr.size() == 4);

	ctr.ctrlDestroyCurrentList();

	listAuxCurr = rep.getAll2();

	assert(ctr.getAll2().size() == 0);

	assert(listAuxCurr.size() == 0);

}

void testCtrlCurrentUndo() {
	Repository rep;
	ValidateActivity val;
	Controller ctr{ rep,val };
	ctr.ctrlAddActivity("a", "a", "a", 6);
	ctr.ctrlAddActivity("b", "b", "a", 6);
	ctr.ctrlAddActivity("c", "c", "a", 6);
	ctr.ctrlAddActivity("d", "d", "d", 6);
	ctr.ctrlAddActivity("e", "e", "e", 6);
	ctr.ctrlAddActivity("f", "f", "f", 6);

	ctr.ctrlAddCurrentActivity2("a");
	ctr.ctrlAddCurrentActivity2("f");
	assert(ctr.getAll2().size() == 2);
	ctr.undo2();
	assert(ctr.getAll2().size() == 1);
	ctr.undo2();
	assert(ctr.getAll2().size() == 0);
	try {
		ctr.undo2();
		assert(false);
	}
	catch (ActivityException & ex) {
		assert(ex.getMsg() == "Nu mai exista operatii");
	}

}

void testCtrlTotalTime() {
	Repository rep;
	ValidateActivity val;
	Controller ctr{ rep,val };
	ctr.ctrlAddActivity("a", "a", "a", 6);
	ctr.ctrlAddActivity("b", "b", "a", 6);
	ctr.ctrlAddActivity("c", "c", "a", 6);
	ctr.ctrlAddActivity("d", "d", "d", 6);
	ctr.ctrlAddActivity("e", "e", "e", 6);
	ctr.ctrlAddActivity("f", "f", "f", 6);
	assert(ctr.ctrlTotalTime() == 36);
}


void testCtrlUndo() {
	Repository rep;
	ValidateActivity val;
	Controller ctr{ rep,val };
	ctr.ctrlAddActivity("a", "a", "a", 6);
	ctr.ctrlAddActivity("b", "b", "a", 6);
	ctr.ctrlAddActivity("c", "c", "a", 6);
	ctr.undo();
	assert(ctr.getAll().size() == 2);
	ctr.ctrlDeleteActivity("b");
	assert(ctr.getAll().size() == 1);
	ctr.undo();
	assert(ctr.getAll().size() == 2);
	ctr.ctrlModifyActivity("b", "test", 1);
	assert(ctr.ctrlSearch("b").getDesc() == "test");
	ctr.ctrlModifyActivity("b", "test", 2);
	assert(ctr.ctrlSearch("b").getType() == "test");
	ctr.ctrlModifyActivity("b", "420", 3);
	assert(ctr.ctrlSearch("b").getTime() == 420);
	ctr.undo();
	ctr.undo();
	ctr.undo();
	assert(ctr.ctrlSearch("b").getDesc() == "b");
	assert(ctr.ctrlSearch("b").getType() == "a");
	assert(ctr.ctrlSearch("b").getTime() == 6);
	ctr.undo();
	ctr.undo();
	assert(ctr.getAll().size() == 0);
	
	try {
		ctr.undo();
		assert(false);
	}
	catch (ActivityException & ex) {
		assert(ex.getMsg() == "Nu mai exista operatii");
	}

}

string printstring(int n)
{
	char letters[26] = { 'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q',
	'r','s','t','u','v','w','x',
	'y','z' };
	string ran = "";
	for (int i = 0; i < n; i++)
		ran = ran + letters[rand() % 26];
	return ran;
}

void testCtrlMonkey() {
	ofstream out("chaos1.txt");
	FileRepository r("chaos.txt", 0.5);
	ValidateActivity v;
	Controller c{ r, v};

	for (int i = 0; i < 20; i++) {
		try {
			int time = rand();
			string title = printstring(150);
			string desc = printstring(100);
			string type = printstring(20);
			c.ctrlAddActivity(title, desc, type, time);
			out << "Succes\n";
		}
		catch (RepositoryException & e) {
			out << "FAIL\n";
		}
	}
	out.close();

}

void testsCtr() {
	testCtrlAddDelete();
	testCtrlModify();
	testCtrlSearch();
	testFilter();
	testSort();
	testStatistica();
	testCtrlCurrent();
	testCtrlTotalTime();
	testCtrlUndo();
	testCtrlCurrentUndo();
	testCtrlMonkey();
}


