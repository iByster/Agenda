#pragma once
#include <string>
#include "Activity.h"
#include <vector>

using std::vector;
using std::string;
using std::ostream;

class ValidateException {
	vector<string> msgs;
public:
	ValidateException(const vector<string>& errors) :msgs{ errors } {}
	friend ostream& operator<<(ostream& out, const ValidateException& ex);
	vector<string> getMessges() const { return msgs; };
};

ostream& operator<<(ostream& out, const ValidateException& ex);

class ValidateActivity {
public:
	void validate(const Activity& a);
};

void testValidator();