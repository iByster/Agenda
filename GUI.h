#pragma once
#include <QtWidgets/qlabel.h>
#include <QtWidgets/qpushbutton.h>
#include <QtWidgets/qboxlayout.h>
#include <QtWidgets/qlineedit.h>
#include <QtWidgets/qformlayout.h>
#include <QtWidgets/qlistwidget.h>
#include <QtWidgets/qwidget.h>
#include <QtWidgets/qmessagebox.h>
#include "Controller.h"
#include "Activity.h"
#include <QtWidgets/qdialog.h> 
#include "CosCRUDGUI.h"
#include "CosReadOnlyGUI.h"
#include "Statistica.h"

class GUIMod : public QDialog {
private:
	Controller& ctr;
	string titlu;
	QLineEdit* Option = new QLineEdit;
	QLineEdit* Modi = new QLineEdit;
	QPushButton* Modifica = new QPushButton{ "&Modifica" };

	void initGUIMOD();

	void initConnectMOD();
public:
	GUIMod(Controller& ctr, string titlu) :ctr{ ctr }, titlu {titlu} {
		setWindowModality(Qt::ApplicationModal);
		initGUIMOD();
		initConnectMOD();
	}
};

class GUIListCurent : public QWidget, public Observable {
private:
	Controller& ctr;
	void initGUIListCurent();
	void loadDataLsCurr(const vector<Activity>& lstAct);
	void initConnectLsCurr();
	QLineEdit* txtTitlu = new QLineEdit;
	QLineEdit* nr = new QLineEdit;
	QLineEdit* fisier = new QLineEdit;


	QListWidget* lst = new QListWidget;
	QPushButton* btnAdd = new QPushButton{ "&Adauga" };
	QPushButton* btnGoleste = new QPushButton{ "&Goleste lista" };
	QPushButton* btnGenerare = new QPushButton{ "&Genereaza lista" };
	QPushButton* btnExport = new QPushButton{ "&Exporta" };
	QPushButton* btnUndo = new QPushButton{ "&Undo" };

public:
	GUIListCurent(Controller& ctr) : ctr{ ctr } {
		initGUIListCurent();
		loadDataLsCurr(ctr.getAll2());
		initConnectLsCurr();
	}
};

class GUI : public QWidget, public Observable {
public:
	
	GUI(Controller& ctr) :ctr{ ctr } {
		initGUI();
		loadData(ctr.getAll());
		initConnect();
		adaugaButoane(ctr.getAll());
	}
private:
	Controller& ctr;
	QPushButton* btnExit = new QPushButton{ "&Exit" };
	QPushButton* btnAdd = new QPushButton{ "&Adauga" };
	QPushButton* btnSterge = new QPushButton{ "&Sterge" };
	QPushButton* btnModifica = new QPushButton{ "&Modifica" };
	QPushButton* btnCautare = new QPushButton{ "&Cautare" };
	QPushButton* btnFiltrare1 = new QPushButton{ "&Filtrare Tip" };
	QPushButton* btnFiltrare2 = new QPushButton{ "&Filtrare Descriere" };
	QPushButton* btnSortare1 = new QPushButton{ "&Sortare Titlu" };
	QPushButton* btnSortare2 = new QPushButton{ "&Sortare Descriere" };
	QPushButton* btnSortare3 = new QPushButton{ "&Sortare Tip+Durata" };
	QPushButton* btnUndo = new QPushButton{ "&Undo" };
	QPushButton* btnListCurr = new QPushButton{ "&Lista curenta activitati" };

	QPushButton* btnCosCRUDGUI = new QPushButton{ "&Cos CRUD GUI" };
	QPushButton* btnCosReadOnlyGUI = new QPushButton{ "&Cos Read Only GUI" };
	QPushButton* btnStatistica = new QPushButton{ "&Statistica" };
	//QPushButton* btnAbstract = new QPushButton{ "&Abstract" };


	QListWidget* lst = new QListWidget;
	QLineEdit* txtTitlu = new QLineEdit;
	QLineEdit* txtDesc = new QLineEdit;
	QLineEdit* txtTip = new QLineEdit;
	QLineEdit* txtTimp = new QLineEdit;
	QLineEdit* tipulFiltrare = new QLineEdit;
	QWidget* btnDyn = new QWidget;
	QVBoxLayout* lyBtnDy = new QVBoxLayout;
	vector<QPushButton*> btns;

	CosCRUDGUI* cosCrud;
	CosReadOnlyGUI* cosRead;
	Statistica* sts;


	

	void initGUI();

	void loadData(const vector<Activity>& lstAct);

	void initConnect();

	void clearLineEdit();

	void adaugaButoane(const vector<Activity>& lstAct);
};