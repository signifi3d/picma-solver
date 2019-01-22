#pragma once

#include <QWidget>
#include <QVector>
#include <QLineEdit>
#include <QGridLayout>
#include <QPushButton>
#include "Grid.h"

enum gridIndices { UI_ROW = 0, UI_COL = 0, VERT_IN_ROW = 1, VERT_IN_COL = 0, HOR_IN_ROW = 0, HOR_IN_COL = 1, PUZZ_GRID_ROW = 1, PUZZ_GRID_COL = 1};

class GridUI : public QWidget {
	
	public:
		GridUI(QWidget *parent = 0);
	private:
		const int minVertWidth = 7;

		QGridLayout *grid;
		QGridLayout *uiCell;
		QGridLayout *vertInputCell;
		QGridLayout *horInputCell;
		QGridLayout *puzzleGridCell;
		QLineEdit   *dimensionInput;
		QVector<QVector<QLineEdit*>> picmaGrid;

		

		void buildGrid();
		bool solve();
		void updateGrid(Grid&);
		int gridDimension = 15;
		int inputGridOffset;
};
