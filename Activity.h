#pragma once
#include <string>

using std::string;

class ActivityException {
	string msg;
public:
	ActivityException(const string& m) :msg{ m } {};
	string getMsg()const { return msg; }
	
};

class Activity {
	std::string title;
	std::string description;
	std::string type;
	int time;
public:

	Activity(const string title, const string descr, const string type, int time) :title{ title }, description{ descr }, type{ type }, time{ time }{}

	Activity(const Activity& ot) :title{ ot.title }, description{ ot.description }, type(ot.type), time{ ot.time }{}

	Activity() = default;


	string getTitle() const {
		return title;
	}

	string getDesc() const {
		return description;
	}

	string getType() const {
		return type;
	}

	int getTime() const noexcept {
		return time;
	}

	void setTitle(const string& title) {
		this->title = title;
	}

	void setDesc(const string& desc) {
		this->description = desc;
	}

	void setType(const string& type) {
		this->type = type;
	}

	void setTime(int time) {
		this->time = time;
	}

	bool operator==(Activity a2);

};
bool cmpTitle(const Activity& a1, const Activity& a2);

bool cmpDesc(const Activity& a1, const Activity& a2);

class DTO {
private:
	string type;
	int cnt;
	double procent;
public:
	DTO() = default;
	DTO(string key) : type{ key }, cnt{ 1 }, procent{ 0 }{};
	void increment();
	void set_procent(int all);
	string get_str();
};