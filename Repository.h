#pragma once
#include "Activity.h"
#include "MyList.h"
#include <string>
#include <ostream>
//#include <vector>
#include <unordered_map>

using std::string;
using std::ostream;

class DefaultRepo {
protected:
	double err;
public:
	virtual void store(const Activity & a) = 0;
	virtual void storeCurr(const Activity & a) = 0;
	virtual void deleteAc(const Activity & a) = 0;
	virtual void deleteAc2(const Activity & a) = 0;
	virtual const Activity& search(string title) = 0;
	virtual const Activity& search2(string title) const = 0;
	virtual void modify(Activity & a, int cmd, string modify) = 0;
	virtual vector<Activity> getAll() = 0;
	virtual vector<Activity> getAll2() = 0;
	virtual void destroy() = 0;
	virtual ~DefaultRepo() = default;

	virtual double getAll_err();

};

class Repository : public DefaultRepo{
protected:
	unordered_map<string, Activity> ActivityList;

	unordered_map<string, Activity> CurrActivityList;

	/*
	medota privata verifica daca exista deja a in repo
	*/
	//bool exist(const Activity& a) const;
	//bool exist2(const Activity& a) const;

public:
	Repository() = default;
	//Repository(double err) : DefaultRepo(err) {};
	Repository(const Repository& ot) = delete;

	Repository(double v_err);

	/*
	Salvare activitate
	arunca exceptie daca mai exista o activitate cu acelasi titlu si descriere
	*/
	virtual void store(const Activity& a);
	void storeCurr(const Activity& a);
	/*
	Sterge activitate
	*/
	virtual void deleteAc(const Activity& a);
	void deleteAc2(const Activity& a);

	/*
	Cauta
	arunca exceptie daca nu exista activitea
	*/
	virtual const Activity& search(string title);
	const Activity& search2(string title) const;
	/*
	Modifica o activitate, se poate modifica orice cand
	*/

	virtual void modify(Activity& a, int cmd, string modify);

	/*
	returneaza toate activitatiile salvate
	*/
	virtual vector<Activity> getAll();
	vector<Activity> getAll2();

	//vector<Activity> sameTitle(const string title);

	void destroy();

};

class FileRepository : public Repository {
private:
	string fName;
	void loadFromFile();
	void writeToFile();
public:
	FileRepository(string fName, double err) : Repository(err), fName{ fName }{ loadFromFile(); }
	~FileRepository() = default;
	void store(const Activity& a) override;
	void deleteAc(const Activity& a) override;
	const Activity& search(string title) override;
	void modify(Activity& a, int cmd, string modify) override;
	vector<Activity> getAll() override;
};

class RepositoryException {
	string msgs;
public:
	RepositoryException(string m) :msgs{ m } {};
	//functie friend (vreau sa folosesc membru privat msg)
	friend ostream& operator<<(ostream& out, const RepositoryException& ex);
	string getMsg() const { return msgs; };
	
};

ostream& operator<<(ostream& out, const RepositoryException& ex);

void testsRepo();
