#include "Repository.h"
#include <assert.h>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <fstream>

using std::ostream;
using std::stringstream;


Repository::Repository(double v_err){
	err = v_err;
}

void Repository::store(const Activity& a) {
	/*if (exist(a)) {
		throw RepositoryException("Activitate existenta!\n");
	}

	ActivityList.push_back(a);*/

	if (ActivityList.find(a.getTitle()) != ActivityList.end()) {
		throw RepositoryException("Activitate existenta!\n");
	}
	ActivityList[a.getTitle()] = a;
}

void Repository::storeCurr(const Activity& a){
	/*if (exist2(a)) {
		throw RepositoryException("Activitate existenta!\n");
	}

	CurrActivityList.push_back(a);
	*/
	if (CurrActivityList.find(a.getTitle()) != CurrActivityList.end()) {
		throw RepositoryException("Activitate existenta!\n");
	}
	CurrActivityList[a.getTitle()] = a;
}




void Repository::deleteAc(const Activity& a) {
	/*for (const auto& act : ActivityList) {
		if (act.getTitle() == a.getTitle() && a.getDesc() == act.getDesc()) {
			ActivityList.erase;
			break;
		}
	}*/
	ActivityList.erase(a.getTitle());

	
}

void Repository::deleteAc2(const Activity& a) {
	/*for (const auto& act : ActivityList) {
		if (act.getTitle() == a.getTitle() && a.getDesc() == act.getDesc()) {
			ActivityList.erase;
			break;
		}
	}*/
	CurrActivityList.erase(a.getTitle());


}



void Repository::modify(Activity& a, int cmd, string modify) {
	auto it = ActivityList.find(a.getTitle());
	/*if (cmd == 1) {
		it->second.setTitle(modify);
		it->first = modify;
	}*/
	if (cmd == 1) {
		it->second.setDesc(modify);
	}
	if (cmd == 2) {
		it->second.setType(modify);
	}
	if (cmd == 3) {
		stringstream aux(modify);
		int x = 0;
		aux >> x;
		it->second.setTime(x);
	}		
}


const Activity& Repository::search(string title){
	/*for (const auto& a : ActivityList) {
		if (a.second.getTitle() == title && a.second.getDesc() == description)
			return a.second;
	}

	throw RepositoryException("Nu extista aceasta activitate!\n");*/
	unordered_map<string, Activity>::const_iterator it = ActivityList.find(title);
	if (it != ActivityList.end()) {
		return it->second;
	}
	else {
		throw RepositoryException("Nu extista aceasta activitate!\n");
	}
}

const Activity& Repository::search2(string title) const{
	unordered_map<string, Activity>::const_iterator it = CurrActivityList.find(title);
	if (it != CurrActivityList.end()) {
		return it->second;
	}
	else {
		throw RepositoryException("Nu extista aceasta activitate!\n");
	}
}

/*
bool Repository::exist(const Activity& a) const {
	try {
		search(a.getTitle(), a.getDesc());
		return true;
	}
	catch (RepositoryException&) {
		return false;
	}
}

bool Repository::exist2(const Activity& a) const{
	try {
		search2(a.getTitle(), a.getDesc());
		return true;
	}
	catch (RepositoryException&) {
		return false;
	}
}
*/
vector<Activity> Repository::getAll() {
	/*vector<Activity> v;
	for (const auto& a : ActivityList) {
		v.push_back(a.second);
	}

	return v;*/
	auto value_selector = [](auto& pair) {return pair.second; };
	vector<Activity> values(ActivityList.size());
	transform(ActivityList.begin(), ActivityList.end(), values.begin(), value_selector);
	return values;
}

vector<Activity> Repository::getAll2() {
	/*vector<Activity> v;
	for (const auto& a : CurrActivityList) {
		v.push_back(a.second);
	}

	return v;*/
	auto value_selector = [](auto& pair) {return pair.second; };
	vector<Activity> values(CurrActivityList.size());
	transform(CurrActivityList.begin(), CurrActivityList.end(), values.begin(), value_selector);
	return values;
}
/*
vector<Activity> Repository::sameTitle(const string title){
	vector<Activity> v;

	for (const auto& a : ActivityList) {
		if (a.second.getTitle() == title) {
			v.push_back(a.second);
		}
	}

	return v;
}
*/

void Repository::destroy(){
	CurrActivityList.clear();
}

ostream& operator<<(ostream& out, const RepositoryException& ex) {
	out << ex.msgs;
	return out;
}

double DefaultRepo::getAll_err() {
	return err;
}

void FileRepository::loadFromFile() {
	std::ifstream in(fName);
	if (!in.is_open()) { //verify if the stream is opened		
		throw ActivityException("Unable to open file:" + fName);
	}
	while (!in.eof()) {
		std::string title;
		in >> title;
		std::string desc;
		in >> desc;
		std::string type;
		in >> type;

		int time;
		in >> time;
		if (in.eof()) {	//nu am reusit sa citesc numarul
			break;
		}
		Activity a{ title.c_str(), desc.c_str(), type.c_str(),  time };
		Repository::store(a);
	}
	in.close();
}

void FileRepository::writeToFile(){
	std::ofstream out(fName);
	if (!out.is_open()) { //verify if the stream is opened
		throw ActivityException("Unable to open file:");
	}
	for (auto& p : getAll()) {
		out << p.getTitle();
		out << std::endl;
		out << p.getDesc();
		out << std::endl;
		out << p.getType();
		out << std::endl;
		out << p.getTime();
		out << std::endl;
	}
	out.close();
}

void FileRepository::store(const Activity& a){
	Repository::store(a);
	writeToFile();
}

void FileRepository::deleteAc(const Activity& a){
	Repository::deleteAc(a);
	writeToFile();
}

const Activity& FileRepository::search(string title) {
	return Repository::search(title);
}

void FileRepository::modify(Activity& a, int cmd, string modify){
	Repository::modify(a, cmd, modify);
	writeToFile();
}

vector<Activity> FileRepository::getAll(){
	return Repository::getAll();
}



void testRepoStoreSearchDelete() {
	Repository rep;
	rep.store(Activity{ "a","a","a", 4 });
	rep.store(Activity{ "lll","ooo","a", 4 });
	rep.store(Activity{ "ccc","ppp","a", 4 });
	//assert(rep.sameTitle("a").size() == 3);
	rep.storeCurr(Activity{ "a","a","a", 4 });
	rep.storeCurr(Activity{ "b","b","b", 4 });
	rep.storeCurr(Activity{ "c","c","c", 4 });
	rep.storeCurr(Activity{ "d","d","d", 4 });
	
	assert(rep.getAll().size() == 3);
	assert(rep.search("a").getTitle() == "a");
	assert(rep.search2("a").getTitle() == "a");

	rep.store(Activity{ "b","b","b",4 });
	assert(rep.getAll().size() == 4);

	rep.store(Activity{ "c","c","c",4 });
	assert(rep.getAll().size() == 5);

	rep.store(Activity{ "d","d","d",4 });
	assert(rep.getAll().size() == 6);

	rep.deleteAc(Activity{ "b","b","b",4 });
	assert(rep.getAll().size() == 5);

	rep.deleteAc(Activity{ "a","a","a",4 });
	assert(rep.getAll().size() == 4);

	rep.deleteAc(Activity{ "d","d","d",4 });
	assert(rep.getAll().size() == 3);


	//nu pot adauga 2 cu acelasi tip si specie
	
	try {
		rep.store(Activity{ "c","c","c",4 });
		assert(false);
	}
	catch (const RepositoryException&) {
		assert(true);
	}
	try {
		rep.storeCurr(Activity{ "c","c","c",4 });
		assert(false);
	}
	catch (const RepositoryException&) {
		assert(true);
	}
	//cauta inexistent
	try {
		rep.search("a");
		assert(false);
	}
	catch (const RepositoryException & e) {
		stringstream os;
		os << e;
		assert(os.str().find("exista") >= 0);
	}
	try {
		rep.search2("e");
		assert(false);
	}
	catch (const RepositoryException & e) {
		stringstream os;
		os << e;
		assert(os.str().find("exista") >= 0);
	}

	rep.destroy();
	assert(rep.getAll2().size() == 0);
	
	
}

void testRepoModify(){
	Repository rep;
	
	Activity a = Activity{ "a","a","a", 4 };
	rep.store(a);
	assert(rep.getAll().size() == 1);
	assert(rep.search("a").getTitle() == "a");
	rep.modify(a, 1, "b");
	//a.setTitle("b");
	rep.modify(a, 2, "b");
	//a.setDesc("b");
	rep.modify(a, 3, "5");
	//rep.modify(a, 4, "5");
	assert(rep.search("a").getTitle() == "a");
	assert(rep.search("a").getDesc() == "b");
	assert(rep.search("a").getType() == "b");
	assert(rep.search("a").getTime() == 5);
	
}

void testFileRepo() {
	//std::ofstream out("test333.txt", std::ios::trunc);
	//out.close();//creez un fisier gol
	FileRepository repF{"test333.txt", 0.0 };
	repF.store(Activity{"nuexistaasaceva","bbb", "aaa",12 });
	auto a = repF.search("nuexistaasaceva");
	repF.modify(a , 1, "oke");
	assert(repF.search("nuexistaasaceva").getDesc() == "oke");
	FileRepository repF2{ "test333.txt", 0.7 };
	auto p = repF2.search("nuexistaasaceva");
	assert(p.getTime() == 12);
	try {
		repF2.search("aaa2");
		assert(false);
	}
	catch (RepositoryException&) {
	}
	repF2.deleteAc(Activity{ "nuexistaasaceva","bbb", "aaa", 12 });
	assert(repF2.getAll().size() == 0);
	try {
		repF2.search("nuexistaasaceva");
		assert(false);
	}
	catch (RepositoryException&) {
	}

	FileRepository repF3{ "test333.txt", 0.7 };
	assert(repF3.getAll().size() == 0);

	//fisierul nu exista si nu se poate crea (nu se creaza si directoare)
	//ar trebui sa arunce exceptie
	try {
		FileRepository repF4{ "te/stActivities.txt", 0.7 };
		assert(false);
	}
	catch (ActivityException&) {
		assert(true);
	}


}

void testsRepo() {
	testRepoStoreSearchDelete();
	testRepoModify();
	testFileRepo();
}

