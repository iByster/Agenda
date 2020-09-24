#include "UI.h"
#include "Activity.h"
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>

using std::cin;
using std::cout;
using std::stringstream;
std::ofstream myfile;

void Console::printUI(const vector<Activity>& activities) {
	cout << "Activitati : \n";

	for (const auto& act : activities) {
		cout << "  " << act.getTitle() << ':' << act.getDesc() << " Tip:" << act.getType() << ", Durata:" << act.getTime() << '\n';
	}
	cout << '\n';
}

void Console::addUI() {
	string ti, de, ty;
	int tim;
	cout << "Titlu: ";
	cin >> ti;
	cout << "Descriere: ";
	cin >> de;
	cout << "Tipul: ";
	cin >> ty;
	cout << "Durata: ";
	cin >> tim;
	ctr.ctrlAddActivity(ti, de, ty, tim);
	cout << "Flawless adding!\n";
}

void Console::deleteUI() {
	string ti;
	cout << "Titlu: ";
	cin >> ti;
	ctr.ctrlDeleteActivity(ti);
	cout << "Flawless destruction!\n";
}

void Console::modifyUI() {
	int cmd, valid = 0;
	string ti, modi;
	cout << "Alegeti activitatea pe care vreti sa o modificati\n";
	cout << "Titlu: ";
	cin >> ti;
	cout << "Ce doriti sa modificati?\n1 Descrierea\n2 Tipul\n3 Durata\n";
	cin >> cmd;
	cout << "Introduceti modificare: ";
	cin >> modi;
	if (modi.size() == 0) valid = 1;
	stringstream aux(modi);
	int x = 0;
	aux >> x;
	if (x < 0)valid = 1;
	if (valid == 0) {
		ctr.ctrlModifyActivity(ti, modi, cmd);
		cout << "Flawless modification!\n";
	}
	else {
		printf("Modificari invalide!\n");
	}

}

void Console::searchUI() {
	string ti;
	cout << "Titlu: ";
	cin >> ti;
	
	Activity act = ctr.ctrlSearch(ti);

	cout << "Activitatea gasita :" << act.getTitle() << ':' << act.getDesc() << " Tip:" << act.getType() << ", Durata:" << act.getTime() << '\n';

}

void Console::filterUI() {
	cout << "Dupa ce doriti sa filtrati?\n1 Tip\n2 Descriere\n";
	int cmd;
	cin >> cmd;
	if (cmd == 2) {
		cout << "Dupa ce descriere doriti sa cautati?\n";
		string desc;
		cin >> desc;
		printUI(ctr.filterDescription(desc));
	}
	if (cmd == 1) {
		cout << "Dupa ce tip doriti sa cautati?\n";
		string type;
		cin >> type;
		printUI(ctr.filterType(type));
	}
}

void Console::sortUI() {
	cout << "Dupa ce doriti sa sortati?\n1 Titlu\n2 Descriere\n3 Tipul + durata\n";
	int cmd;
	cin >> cmd;
	if (cmd == 1) {
		printUI(ctr.sortByTitle());
	}
	if (cmd == 2) {
		printUI(ctr.sortByDesc());
	}
	if (cmd == 3) {
		printUI(ctr.sortByTypeDuration());
	}
	
}

void Console::statisticUI(){
	vector<DTO> stats  = ctr.statistica();
	for (auto s : stats) {
		cout << s.get_str();
	}
}

void Console::menuCurrentListAddUI() {
	cout << "Introduceti titlul pe care il cautati :";

	string title;

	cin >> title;

	/*vector<Activity> activitiesWithSameTitle = ctr.ctrlAddCurrentActivity1(title);
	if (activitiesWithSameTitle.size() == 0) cout << "Nu exista activitati cu acest titlu!\n";
	else {
		if (activitiesWithSameTitle.size() > 1) {
			cout << "S-au gasit mai multe activitati cu acelasi titlu:\n";
			int i = 1;
			for (auto& act : activitiesWithSameTitle) {
				cout << i << "  " << act.getTitle() << ':' << act.getDesc() << " Tip:" << act.getType() << ", Durata:" << act.getTime() << '\n';
				++i;
			}
			cout << "Alegeti indicele activitati pe care doriti sa o introduceti in lista curenta!\n";

			int poz = 0;
			cin >> poz;

			if (poz < 0 || poz > i) {
				cout << "Comanda invalida!\n";
			}
			else {
				ctr.ctrlAddCurrentActivity2(activitiesWithSameTitle[poz - 1]);
			}
		}
		else {
			ctr.ctrlAddCurrentActivity2(activitiesWithSameTitle[0]);
		}
	}*/

	ctr.ctrlAddCurrentActivity2(title);
	
}

void Console::menuCurrentListDestroyUI() {
	ctr.ctrlDestroyCurrentList();
}

void Console::menuCurrentListGenerateUI() {

	cout << "Introduceti numarul de activitati ce vor sa fie introduse:";

	int i;

	cin >> i;
	
	ctr.ctrlGenerateCurrentList(i);
}

void Console::menuCurrentListExportUI() {
	cout << "Introduceti numele fisierului CVS in care doriti sa exportati activitatile:";
	string fileName; 
	cin >> fileName;
	myfile.open(fileName);

	for (const auto& act : ctr.getAll2()) {
		myfile << act.getTitle() + "," + act.getDesc() + "," + act.getType() + "," + std::to_string(act.getTime()) + "\n";
	}

	myfile.close();
}

void Console::menuCurrentListUI() {

	cout << "Lista activitatiilor curente :";

	printUI(ctr.getAll2());

	cout << "1 Adauga activitate dupa titlu\n2 Goleste lista\n3 Genereaza lista\n4 Exporta lista\n5 Undo\n";

	int cmd;

	cin >> cmd;

	try {
		switch (cmd)
		{
		case 1:
			menuCurrentListAddUI();
			break;
		case 2:
			menuCurrentListDestroyUI();
			break;
		case 3:
			menuCurrentListGenerateUI();
			break;
		case 4:
			menuCurrentListExportUI();
			break;
		case 5:
			ctr.undo2();
			break;
		default:
			cout << "Comanda invalida!\n";
		}
	}
	catch (const RepositoryException & ex) {
		cout << ex << '\n';
	}
	catch (const ValidateException & ex) {
		cout << ex << '\n';
	}

}


void Console::run() {
	while (true) {
		cout << "~~MENIU~~\n";
		cout << "1 Adauga\n2 Tiparest\n3 Sterge\n4 Modifica\n5 Cautare\n6 Filtrare\n7 Sortare\n8 Statistica\n9 Lista curenta de activitati\n10 Durata totala al activitatiilor\n11 Undo\n0 Exit\n";
		int cmd;
		cin >> cmd;
		try {
			switch (cmd) {
			case 1:
				addUI();
				break;
			case 2:
				printUI(ctr.getAll());
				break;
			case 3:
				deleteUI();
				break;
			case 4:
				printUI(ctr.getAll());
				modifyUI();
				break;
			case 5:
				searchUI();
				break;
			case 6:
				filterUI();
				break;
			case 7:
				sortUI();
				break;
			case 8:
				statisticUI();
				break;
			case 9:
				menuCurrentListUI();
				break;
			case 10:
				cout << ctr.ctrlTotalTime();
				break;
			case 11:
				ctr.undo();
				break;
			case 0:
				return;
			default:
				cout << "Comanda invalida!\n";

			}		
		}
		catch (const RepositoryException& ex) {
			cout << ex << '\n';
		}
		catch (const ValidateException & ex) {
			cout << ex << '\n';
		}
		catch (const ActivityException & ex) {
			cout << ex.getMsg() << '\n';
		}
	}
}