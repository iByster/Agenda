#include "GUI.h"
#include <sstream>
#include "Repository.h"
#include <fstream>

std::ofstream myfile;

void GUI::initGUI() {
	auto lyMain = new QHBoxLayout{};

	setLayout(lyMain);

	auto lyAux = new QVBoxLayout{};

	auto formLy = new QFormLayout;
	formLy->addRow("Titlu", txtTitlu);
	formLy->addRow("Descriere", txtDesc);
	formLy->addRow("Tipul", txtTip);
	formLy->addRow("Timp", txtTimp);

	lyAux->addLayout(formLy);

	auto lyBtns = new QVBoxLayout{};

	lyBtns->addWidget(btnAdd);
	lyBtns->addWidget(btnSterge);
	lyBtns->addWidget(btnModifica);
	lyBtns->addWidget(btnCautare);
	auto lyBtns2 = new QHBoxLayout{};
	lyBtns2->addWidget(btnFiltrare1);
	lyBtns2->addWidget(btnFiltrare2);
	lyBtns->addLayout(lyBtns2);
	auto lyBtns3 = new QHBoxLayout{};
	lyBtns3->addWidget(btnSortare1);
	lyBtns3->addWidget(btnSortare2);
	lyBtns3->addWidget(btnSortare3);
	lyBtns->addLayout(lyBtns3);
	lyBtns->addWidget(new QPushButton{ "Statistica" });
	lyBtns->addWidget(btnListCurr);
	lyBtns->addWidget(new QPushButton{ "Durata totala al activitatiilor" });
	lyBtns->addWidget(btnCosCRUDGUI);
	lyBtns->addWidget(btnCosReadOnlyGUI);
	lyBtns->addWidget(btnStatistica);
	//lyBtns->addWidget(btnAbstract);
	lyBtns->addWidget(btnUndo);
	lyBtns->addWidget(btnExit);



	lyAux->addLayout(lyBtns);

	lyMain->addLayout(lyAux);

	
	lyMain->addWidget(lst);
	btnDyn->setLayout(lyBtnDy);
	lyMain->addWidget(btnDyn);

}

void GUI::loadData(const vector<Activity>& lstAct){
	lst->clear();

	for (const auto& act : lstAct) {
		lst->addItem(QString::fromStdString(act.getTitle()));
	}
}

void GUI::initConnect() {
	QObject::connect(btnExit, &QPushButton::clicked, [&]() {
		close();
	});

	QObject::connect(btnListCurr, &QPushButton::clicked, [&]() {
		auto lc = new GUIListCurent{ ctr };
		lc->show();
	});

	QObject::connect(btnUndo, &QPushButton::clicked, [&]() {
		try {
			ctr.undo();
			ctr.notify();
			loadData(ctr.getAll());
			adaugaButoane(ctr.getAll());
		}
		catch (const ActivityException & ex) {
			QMessageBox::warning(this, "Warning", QString::fromStdString(ex.getMsg()));
		}
	});


	QObject::connect(btnSortare1, &QPushButton::clicked, [&]() {
		loadData(ctr.sortByTitle());
		adaugaButoane(ctr.sortByTypeDuration());
	});

	QObject::connect(btnSortare2, &QPushButton::clicked, [&]() {
		loadData(ctr.sortByDesc());
		adaugaButoane(ctr.sortByTypeDuration());
	});

	QObject::connect(btnSortare3, &QPushButton::clicked, [&]() {
		loadData(ctr.sortByTypeDuration());
		adaugaButoane(ctr.sortByTypeDuration());
	});

	QObject::connect(btnFiltrare1, &QPushButton::clicked, [&]() {
		auto w = new QDialog;
		auto formLy = new QFormLayout;
		auto lyMain = new QVBoxLayout{};
		formLy->addRow("Tipul", tipulFiltrare);
		w->setLayout(lyMain);
		lyMain->addLayout(formLy);
		w->show();
		auto btn = new QPushButton{ "OK" };
		lyMain->addWidget(btn);
		//loadData(ctr.filterType)
		QObject::connect(btn, &QPushButton::clicked, [&]() {
			lst->clear();
			auto tip = tipulFiltrare->text().toStdString();
			vector<Activity> actvs = ctr.filterType(tip);
			vector<Activity> mainActvs = ctr.getAll();
			for (const auto& act : mainActvs) {
				QListWidgetItem* item = new QListWidgetItem(QString::fromStdString(act.getTitle()));
				for (const auto& act2 : actvs) {
					if (act.getTitle() == act2.getTitle()) {
						item->setBackground(QBrush{ Qt::green, Qt::SolidPattern });
						break;
					}
				}
				lst->addItem(item);
			}
			//w->close();
		});
		//w->close();
	});

	QObject::connect(btnFiltrare2, &QPushButton::clicked, [&]() {
		auto w = new QDialog;
		auto formLy = new QFormLayout;
		auto lyMain = new QVBoxLayout{};
		formLy->addRow("Descriere", tipulFiltrare);
		w->setLayout(lyMain);
		lyMain->addLayout(formLy);
		w->show();
		auto btn = new QPushButton{ "OK" };
		lyMain->addWidget(btn);
		//loadData(ctr.filterType)
		QObject::connect(btn, &QPushButton::clicked, [&]() {
			lst->clear();
			auto tip = tipulFiltrare->text().toStdString();
			vector<Activity> actvs = ctr.filterDescription(tip);
			vector<Activity> mainActvs = ctr.getAll();
			for (const auto& act : mainActvs) {
				QListWidgetItem* item = new QListWidgetItem(QString::fromStdString(act.getTitle()));
				for (const auto& act2 : actvs) {
					if (act.getTitle() == act2.getTitle()) {
						item->setBackground(QBrush{ Qt::green, Qt::SolidPattern });
						break;
					}
				}
				lst->addItem(item);
			}
			//w->close();
			});
		});

	QObject::connect(btnCautare, &QPushButton::clicked, [&]() {
		auto titlu = txtTitlu->text().toStdString();
		lst->clear();
		vector<Activity> lstAct = ctr.getAll();
		bool gasit = false;
		for (const auto& act : lstAct) {
			QListWidgetItem* item = new QListWidgetItem(QString::fromStdString(act.getTitle()));
			if (act.getTitle() == titlu) {
				item->setBackground(QBrush{ Qt::green, Qt::SolidPattern });
				gasit = true;
			}
			lst->addItem(item);
		}
		if (gasit == true) {
			QMessageBox::information(this, "Info", "Activitate Gasita!");
		}
		else {
			QMessageBox::information(this, "Info", "Nu exista aceasta activitate.");
		}
	});

	QObject::connect(btnSterge, &QPushButton::clicked, [&]() {
		auto titlu = txtTitlu->text().toStdString();
		try {
			ctr.ctrlDeleteActivity(titlu);
			clearLineEdit();
			QMessageBox::information(this, "Info", "Activitate Stearsa!");
		}
		catch (const RepositoryException & ex) {
			QMessageBox::warning(this, "Warning", QString::fromStdString(ex.getMsg()));
		}
		loadData(ctr.getAll());
		ctr.notify();
		adaugaButoane(ctr.getAll());
	});

	QObject::connect(btnModifica, &QPushButton::clicked, [&]() {
		auto titlu = txtTitlu->text().toStdString();
		auto gm = new GUIMod{ ctr , titlu};
		gm->show();
		loadData(ctr.getAll());
		ctr.notify();
		adaugaButoane(ctr.getAll());
	});


	QObject::connect(lst, &QListWidget::itemSelectionChanged, [&]() {
		auto sel = lst->selectedItems();
		if (sel.empty()) {
			clearLineEdit();
		}
		else {
			auto selItem = sel.at(0);
			auto titlu = selItem->text();
			txtTitlu->setText(titlu);
			try {
				auto act = ctr.ctrlSearch(titlu.toStdString());
				txtDesc->setText(QString::fromStdString(act.getDesc()));
				txtTip->setText(QString::fromStdString(act.getType()));
				txtTimp->setText(QString::fromStdString(std::to_string(act.getTime())));

			}
			catch (const RepositoryException & ex) {
				QMessageBox::warning(this, "Warning", QString::fromStdString(ex.getMsg()));
			}
			}
	});

	QObject::connect(btnAdd, &QPushButton::clicked, [&]() {
		auto titlu = txtTitlu->text().toStdString();
		auto desc = txtDesc->text().toStdString();
		auto tip = txtTip->text().toStdString();
		auto timp = txtTimp->text().toStdString();
		stringstream geek(timp);
		int x = 0; geek >> x;
		try {
			ctr.ctrlAddActivity(titlu, desc, tip, x);
			QMessageBox::information(this, "Info", "Activitate adaugata!");
			clearLineEdit();
		}
		catch (const RepositoryException& ex) {
			QMessageBox::warning(this, "Warning", QString::fromStdString(ex.getMsg()));
		}
		catch (const ValidateException& ex) {
			vector<string> s = ex.getMessges();
			string rez = "";
			for (int i = 0; i < s.size(); ++i) {
				rez += s[i] + "\n";
			}
			QMessageBox::warning(this, "Warning", QString::fromStdString(rez));
		}

		
		loadData(ctr.getAll());
		ctr.notify();
		adaugaButoane(ctr.getAll());
		});

	QObject::connect(btnCosCRUDGUI, &QPushButton::clicked, [&]() {
		cosCrud = new CosCRUDGUI{ ctr };
		cosCrud->setWindowTitle("Cos CRUD GUI");
		cosCrud->show();
		});
	QObject::connect(btnCosReadOnlyGUI, &QPushButton::clicked, [&]() {
		cosRead = new CosReadOnlyGUI{ ctr };
		cosRead->setWindowTitle("Cos Read Only");
		cosRead->show();
		});
	QObject::connect(btnStatistica, &QPushButton::clicked, [&]() {
		sts = new Statistica{ ctr };
		sts->setWindowTitle("Stats");
		sts->show();
		});

}

void GUI::clearLineEdit() {
	txtTitlu->setText("");
	txtDesc->setText("");
	txtTip->setText("");
	txtTimp->setText("");
}

void GUI::adaugaButoane(const vector<Activity>& lstAct) {
	for (auto& btn : btns) {
		delete btn;
	}
	btns.clear();
	for (const auto& act : lstAct) {
		auto btn = new QPushButton{ QString::fromStdString(act.getTitle()) };
		btns.push_back(btn);
		QObject::connect(btn, &QPushButton::clicked, [this, btn, act]() {
			ctr.ctrlDeleteActivity(act.getTitle());
			loadData(ctr.getAll());
			adaugaButoane(ctr.getAll());
			});
	}
	for (auto& btn : btns) {
		lyBtnDy->addWidget(btn);
	}
}

void GUIMod::initGUIMOD(){
	QVBoxLayout* ly = new QVBoxLayout;
	setLayout(ly);
	auto r = new QFormLayout;
	r->addRow("Optiune :", Option);
	r->addRow("Modificare :", Modi);
	ly->addLayout(r);
	ly->addWidget(Modifica);
	auto lbl = new QLabel{ "Optiuni : Descriere/Tip/Timp" };
	ly->addWidget(lbl);
}

void GUIMod::initConnectMOD() {
	QObject::connect(Modifica, &QPushButton::clicked, [&]() {
		auto opt = Option->text().toStdString();
		auto modificare = Modi->text().toStdString();

		if (modificare.size() > 0)
		{
			if (opt == "Descriere") {

				ctr.ctrlModifyActivity(titlu, modificare, 1);
				QMessageBox::information(this, "Info", "Activitate Modificata!");
				close();

			}
			else if (opt == "Tip") {
				ctr.ctrlModifyActivity(titlu, modificare, 2);
				QMessageBox::information(this, "Info", "Activitate Modificata!");
				close();
			}
			else if (opt == "Timp") {
				stringstream geek(modificare);
				int x = 0; geek >> x;
				ctr.ctrlModifyActivity(titlu, modificare, 3);
				QMessageBox::information(this, "Info", "Activitate Modificata!");
				close();
			}
			else {
				QMessageBox::warning(this, "Warning", "Optiune invalida");
			}
		}
		else
		{
			QMessageBox::warning(this, "Warning", "Modificare invalida");
		}
	});

}

void GUIListCurent::initGUIListCurent() {
	auto lyMain = new QVBoxLayout{};
	setLayout(lyMain);
	lyMain->addWidget(lst);
	auto formLy = new QFormLayout;
	formLy->addRow("Titlu", txtTitlu);
	formLy->addRow("Fisier", fisier);
	lyMain->addLayout(formLy);
	auto lyBtns = new QVBoxLayout{};
	lyBtns->addWidget(btnAdd);
	lyBtns->addWidget(btnGoleste);
	lyBtns->addWidget(btnGenerare);
	lyBtns->addWidget(btnExport);
	lyBtns->addWidget(btnUndo);

	lyMain->addLayout(lyBtns);
}

void GUIListCurent::loadDataLsCurr(const vector<Activity>& lstAct) {
	lst->clear();

	for (const auto& act : lstAct) {
		lst->addItem(QString::fromStdString(act.getTitle()));
	}
}

void GUIListCurent::initConnectLsCurr(){
	QObject::connect(btnAdd, &QPushButton::clicked, [&]() {
		auto titlu = txtTitlu->text().toStdString();

		try {
			ctr.ctrlAddCurrentActivity2(titlu);
			QMessageBox::information(this, "Info", "Activitate adaugata!");
		}
		catch (const RepositoryException & ex) {
			QMessageBox::warning(this, "Warning", QString::fromStdString(ex.getMsg()));
		}
		loadDataLsCurr(ctr.getAll2());
	});
	QObject::connect(btnGoleste, &QPushButton::clicked, [&]() {
		ctr.ctrlDestroyCurrentList();
		loadDataLsCurr(ctr.getAll2());
	});

	QObject::connect(btnGenerare, &QPushButton::clicked, [&]() {
		auto w = new QDialog;
		auto formLy = new QFormLayout;
		auto lyMain = new QVBoxLayout{};
		formLy->addRow("Numarul de elemente ce se va genera", nr);
		w->setLayout(lyMain);
		lyMain->addLayout(formLy);
		auto btn = new QPushButton{ "OK" };
		lyMain->addWidget(btn);

		w->show();

		QObject::connect(btn, &QPushButton::clicked, [&]() {
			auto nrGen = nr->text().toStdString();
			stringstream geek(nrGen);
			int x = 0; geek >> x;
			ctr.ctrlGenerateCurrentList(x);
			loadDataLsCurr(ctr.getAll2());
		});

	});

	QObject::connect(btnExport, &QPushButton::clicked, [&]() {
		auto f = fisier->text().toStdString();
		string fileName = f;
		myfile.open(fileName);

		for (const auto& act : ctr.getAll2()) {
			myfile << act.getTitle() + "," + act.getDesc() + "," + act.getType() + "," + std::to_string(act.getTime()) + "\n";
		}

		myfile.close();

	});

	QObject::connect(btnUndo, &QPushButton::clicked, [&]() {
		QObject::connect(btnUndo, &QPushButton::clicked, [&]() {
			try {
				ctr.undo2();
				loadDataLsCurr(ctr.getAll2());
			}
			catch (const ActivityException & ex) {
				QMessageBox::warning(this, "Warning", QString::fromStdString(ex.getMsg()));
			}
			});
	});


}
