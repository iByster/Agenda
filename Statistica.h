#pragma once
#include "Observer.h"
#include "Controller.h"
#include <qwidget.h>
#include <qpainter.h>

class Statistica : public QWidget, public Observer {
private:
	Controller& ctr;

public:
	Statistica(Controller& controller) :ctr{ controller } {
		ctr.addObs(this);
	}
	void update() override {
		repaint();
	}
	void paintEvent(QPaintEvent* ev) override {
		QPainter p{ this };
		auto all = ctr.getAll();
		vector<pair<string, int>> vect;
		for (auto elem : all) {
			bool found = false;
			for (int i = 0; i < vect.size() && !found; i++) {
				if (vect[i].first == elem.getType()) {
					vect[i].second++;
					found = true;
				}
			}
			if (!found) {
				vect.push_back(make_pair(elem.getType(), 1));
			}
		}
		int x = 10;
		for (auto elem : vect) {
			p.drawRect(x, 0, 20, elem.second * 5);
			x += 40;
		}
	}
};