#include "QtGuiApplication1.h"
#include <QtWidgets/QApplication>
#include "GUI.h"
#include "Controller.h"
#include "Repository.h"
#include "Validator.h"

void testAll() {
	testsRepo();
	testsCtr();
	testValidator();
}



int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	FileRepository repo("date.txt", 0.0);
	ValidateActivity val;
	Controller ctr{ repo, val };
	GUI w{ ctr };
	w.show();

	return a.exec();
}
