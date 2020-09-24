#include "Activity.h"
#include <string>

using std::to_string;

/*
bool Activity::operator==(Activity a2) {
	return this->title == a2.title && this->description == a2.description;
}
*/
bool cmpTitle(const Activity& a1, const Activity& a2)
{
	return a1.getTitle() < a2.getTitle();
}

bool cmpDesc(const Activity& a1, const Activity& a2)
{
	return a1.getDesc() < a2.getDesc();
}


void DTO::increment()
{
	cnt++;
}

void DTO::set_procent(int all)
{
	procent = (cnt * 1.0 / all) * 100;
}

string DTO::get_str()
{
	string output = type + " " + to_string(procent) + "%\n";
	return output;
}
