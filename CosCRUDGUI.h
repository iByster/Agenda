#pragma once
#include <QtWidgets/qapplication.h>
#include <QtWidgets/qlabel.h>
#include <QtWidgets/qpushbutton.h>
#include <QtWidgets/qboxlayout.h>
#include <QtWidgets/qlineedit.h>
#include <QtWidgets/qformlayout.h>
#include <QtWidgets/qlistwidget.h>
#include <qmessagebox.h>
#include <qdebug.h>
#include <vector>
#include <string>

#include "Observer.h"
#include "Controller.h"
#include "Activity.h"

using namespace std;

class CosCRUDGUI : public QWidget, public Observable, public Observer {
public:

	CosCRUDGUI(Controller& controller) : ctr{ controller } {
		initGUI();
		initConnect();
		loadData();
	}
private:
	Controller& ctr;

	QPushButton* btnPopuleaza = new QPushButton{ "&Populeaza" };
	QPushButton* btnGoleste = new QPushButton{ "&Goleste" };

	QListWidget* lst = new QListWidget();

	QLineEdit* txtNumar = new QLineEdit;

	void update() override {
		loadData();
	}

	void initGUI() {
		QHBoxLayout* lyMain = new QHBoxLayout{};
		QHBoxLayout* lyLeft = new QHBoxLayout{};
		QVBoxLayout* lyRight = new QVBoxLayout{};
		QHBoxLayout* lyMid = new QHBoxLayout{};
		setLayout(lyMain);

		//lst->setModel(listModel);

		lyLeft->addWidget(lst);

		auto formLy = new QFormLayout;
		formLy->addRow("Numar", txtNumar);
		lyRight->addLayout(formLy);

		lyMid->addWidget(btnPopuleaza);
		lyMid->addWidget(btnGoleste);

		lyRight->addLayout(lyMid);

		lyMain->addLayout(lyLeft);
		lyMain->addLayout(lyRight);
	}

	void loadData() {
		//int nr = 0;
		lst->clear();
		for (const auto& produs : ctr.getAll2()) {
			auto item = QString::fromStdString(produs.getTitle());
			//QModelIndex i = listModel->index(nr);
			//listModel->setData(i, item);
			//nr++;
			lst->addItem(item);
		}
	}

	void initConnect() {
		ctr.addObs(this);
		QObject::connect(btnPopuleaza, &QPushButton::clicked, [&]() {
			try {
				auto numar = stoi(txtNumar->text().toStdString());
				ctr.ctrlGenerateCurrentList(numar);
			}
			catch (RepositoryException & ex) {
				QMessageBox::warning(this, "Warning", QString::fromStdString(ex.getMsg()));
			}
			catch (exception & e) {
				QMessageBox::warning(this, "Warning", QString::fromStdString(e.what()));
			}
			ctr.notify();
			});
		QObject::connect(btnGoleste, &QPushButton::clicked, [&]() {
			ctr.ctrlDestroyCurrentList();
			ctr.notify();
			});
	}
};