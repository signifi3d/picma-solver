#pragma once

#include <QWidget>
#include <QVector>
#include <QLineEdit>
#include "Grid.h"

class GridUI : public QWidget {
	
	public:
		GridUI(QWidget *parent = 0);
	private:
		bool solve();
		void updateGrid(Grid&);
		QVector<QVector<QLineEdit*>> picmaGrid;
		int gridDimension = 16;
};
