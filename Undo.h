#pragma once
#include "Activity.h"
#include "Repository.h"

class ActiuneUndo {
public:
	virtual void doUndo() = 0;
	virtual ~ActiuneUndo() = default;
};

class UndoAdauga : public ActiuneUndo {
	Activity activitateAdaugat;
	DefaultRepo& repo;
public:
	UndoAdauga(DefaultRepo& repo, const Activity& a) : repo{ repo }, activitateAdaugat{ a }{}

	void doUndo() override {
		repo.deleteAc(activitateAdaugat);
	}
};

class UndoAdaugaCurr :public ActiuneUndo {
	Activity activitateAdaugat;
	DefaultRepo& repo;
public:
	UndoAdaugaCurr(DefaultRepo& repo, const Activity& a) : repo{ repo }, activitateAdaugat{ a }{}
	void doUndo() override {
		repo.deleteAc2(activitateAdaugat);
	}
};

class UndoSterge : public ActiuneUndo {
	Activity activitateSters;
	DefaultRepo& repo;
public:
	UndoSterge(DefaultRepo& repo, Activity& a) : repo{ repo }, activitateSters{ a }{}
	void doUndo() override {
		repo.store(activitateSters);
	}
};

class UndoModifica : public ActiuneUndo {
	Activity activitateModi;
	int cmd;
	string modify;
	DefaultRepo& repo;
public:
	UndoModifica(DefaultRepo& repo, Activity& a, string modify, int cmd) : repo{ repo }, activitateModi{ a }, modify{ modify }, cmd{ cmd }{}
	void doUndo() override {
		repo.modify(activitateModi, cmd, modify);
	}
};