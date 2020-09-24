#include "Validator.h"
#include <assert.h>
#include <sstream>


ostream& operator<<(ostream& out, const ValidateException& ex) {
	for (const auto& msg : ex.msgs) {
		out << msg << " ";
	}
	return out;
}


void ValidateActivity::validate(const Activity& a) {
	vector<string> msgs;
	if (a.getTitle().size() == 0)msgs.push_back("Titlu invalid!\n");
	if (a.getDesc().size() == 0)msgs.push_back("Descriere invalida!\n");
	if (a.getType().size() == 0)msgs.push_back("Tipul invalid!\n");
	if (a.getTime() < 0)msgs.push_back("Timp invalid!\n");
	if (msgs.size() > 0) {
		throw ValidateException(msgs);
	}
}




void testValidator() {
	ValidateActivity v;
	Activity a{ "","","", -1};
	try {
		v.validate(a);}
	catch (const ValidateException& ex) {
		std::stringstream sout;
		sout << ex;
		auto mesaj = sout.str();
		assert(mesaj.find("Timp invalid!\n") >= 0);
		assert(mesaj.find("Titlu invalid!\n") >= 0);
		assert(mesaj.find("Descriere invalida!\n") >= 0);
		assert(mesaj.find("Tipul invalid!\n") >= 0);
	}

}