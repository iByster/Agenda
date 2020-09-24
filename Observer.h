#pragma once
#include <vector>
#include <algorithm>
using namespace std;

class Observer {
public:
	virtual void update() = 0;
};

class Observable {
private:
	vector<Observer*> interesati;
public:
	void notify() {
		for (auto obs : interesati) {
			obs->update();
		}
	}
public:
	void addObs(Observer* obs) {
		interesati.push_back(obs);
	}
	void removeObs(Observer* obs) {
		interesati.erase(remove(interesati.begin(), interesati.end(), obs));
	}
};