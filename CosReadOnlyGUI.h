#pragma once
#include "Observer.h"
#include "Controller.h"
#include <qwidget.h>
#include <qpainter.h>

class CosReadOnlyGUI : public QWidget, public Observer {
private:
	Controller& ctr;

public:
	CosReadOnlyGUI(Controller& controller) : ctr{ controller } {
		ctr.addObs(this);
		srand(time(NULL));
	}
	void update() override {
		repaint();

	}
	void paintEvent(QPaintEvent* ev) override {
		QPainter p{ this };
		int count = ctr.getAll2().size();
		for (int i = 0; i < count; i++) {
			p.drawRect(rand() % (this->width() - 62) + 21, rand() % (this->height() - 62) + 21, 20, 20);
		}
	}
};