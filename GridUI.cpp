#include <QGridLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QString>
#include "GridUI.h"
#include "Grid.h"
#include "PicmaBox.h"
#include <iostream>

QString* generateTestInput(int x, int y) {
	if ( y == 0 ) {
		if ( x == 1 ) {
			return new QString("13");	
		} else if ( x == 2 ) {
			return new QString("3 2");
		} else if ( x == 3 ) {
			return new QString("1 5 2");
		} else if ( x == 4 ) {
			return new QString("3 1 3 2");
		} else if ( x == 5 ) {
			return new QString("3 1 3 1");
		} else if ( x == 6 ) {
			return new QString("5 1 2 1");
		} else if ( x == 7 ) {
			return new QString("1 5 1 1");
		} else if ( x == 8 ) {
			return new QString("1 1 1 1 1 1");
		} else if ( x == 9 ) {
			return new QString("1 3 1 1 1 1");
		} else if ( x == 10 ) {
			return new QString("1 5 1 2 1");
		} else if ( x == 11 ) {
			return new QString("3 1 3 1");
		} else if ( x == 12 ) {
			return new QString("3 1 3 2");
		} else if ( x == 13 ) {
			return new QString("1 5 2");
		} else if ( x == 14 ) {
			return new QString("3 3");
		} else if ( x == 15 ) {
			return new QString("11 1");
		}	
	} else if ( x == 0 ) {
		if ( y == 1 ) {
			return new QString("3 1 3");	
		} else if ( y == 2 ) {
			return new QString("2 1 1 2");
		} else if ( y == 3 ) {
			return new QString("2 9 2");
		} else if ( y == 4 ) {
			return new QString("1 3 3 1");
		} else if ( y == 5 ) {
			return new QString("1 2 2 2 2 1");
		} else if ( y == 6 ) {
			return new QString("1 1 7 1 1");
		} else if ( y == 7 ) {
			return new QString("1 2 2 2 2 1");
		} else if ( y == 8 ) {
			return new QString("1 1 1 1 1");
		} else if ( y == 9 ) {
			return new QString("1 2 3 2 1");
		} else if ( y == 10 ) {
			return new QString("1 3 3 1");
		} else if ( y == 11 ) {
			return new QString("2 2 3 2 2");
		} else if ( y == 12 ) {
			return new QString("3 2 2 1");
		} else if ( y == 13 ) {
			return new QString("1 2 5 2");
		} else if ( y == 14 ) {
			return new QString("2 3");
		} else if ( y == 15 ) {
			return new QString("1 3 1 1");
		}
	} else {
		return new QString(" ");
	}
}

GridUI::GridUI(QWidget* parent) : QWidget(parent) {

	QGridLayout *grid = new QGridLayout(this);

	grid->setSpacing(2);
	QPushButton *solveButton = new QPushButton("solve", this);
	solveButton->setFixedSize(60, 20);
	grid->addWidget(solveButton, 0, 0);
	
	for (int i = 0; i < 16; ++i) {
		QVector<QLineEdit*> gridRow;
		for (int j = 0; j < 16; ++j) {
			if ( i == 0 && j == 0 ) {
				gridRow.append(NULL);
				continue;
			}
			if ( i != 0 && j != 0 ) {
				gridRow.append(new QLineEdit(this));
				gridRow[j]->setReadOnly(true);
				gridRow[j]->setFixedWidth(20);
			} else {
				QString *defaultText = generateTestInput(i, j);
				gridRow.append(new QLineEdit(*defaultText, this));
				gridRow[j]->setFixedWidth(60);
			}
			grid->addWidget(gridRow[j], j, i);	
		}
		picmaGrid.append(gridRow);
	}
	
	setLayout(grid);

	connect(solveButton, &QPushButton::clicked, this, &GridUI::solve);
}

bool GridUI::solve() {
	std::cout<<"solve entered."<<std::endl;
	Grid solutionBoard(16, picmaGrid);
	std::cout<<"Board Made."<<std::endl;
	solutionBoard.solve();
	std::cout<<"Solve passed."<<std::endl;
	updateGrid(solutionBoard);
	std::cout<<"updateGrid passed."<<std::endl;
	return true;
}

void GridUI::updateGrid(Grid& newGrid) {
	for (int i = 0; i < 15; ++i) {
		for (int j = 0; j < 15; ++j) {
			QString *boxState;
			switch (newGrid.boxStateAt(i,j)) {
				case OPEN:
					boxState = new QString("?");
					break;
				case FILLED:
					boxState = new QString("+");
					break;
				case CLOSED:
					boxState = new QString("x");
					break;
				default:
					boxState = new QString(" ");
			}
			picmaGrid[i+1][j+1]->setText(*boxState);
		}
	} 
}

