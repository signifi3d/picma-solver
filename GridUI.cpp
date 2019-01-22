#include <QGridLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QString>
#include <QLayoutItem>
#include <QLabel>
#include "GridUI.h"
#include "Grid.h"
#include "PicmaBox.h"
#include <iostream>


GridUI::GridUI(QWidget* parent) : QWidget(parent) {

	grid = new QGridLayout(this);
	grid->setSpacing(2);

	uiCell = new QGridLayout();
	vertInputCell = new QGridLayout();
	horInputCell = new QGridLayout();
	puzzleGridCell = new QGridLayout();
	
	grid->addItem(uiCell, UI_COL, UI_ROW);
	grid->addItem(vertInputCell, VERT_IN_COL, VERT_IN_ROW);
	grid->addItem(horInputCell, HOR_IN_COL, HOR_IN_ROW);
	grid->addItem(puzzleGridCell, PUZZ_GRID_COL, PUZZ_GRID_ROW); 

	inputGridOffset = (gridDimension / 2) + gridDimension % 2;

	QPushButton *solveButton = new QPushButton("solve", this);
	solveButton->setFixedSize(60, 20);
	uiCell->addWidget(solveButton, 1, 0);

	dimensionInput = new QLineEdit(QString("%1").arg(gridDimension), this);
	dimensionInput->setFixedWidth(25);
	QLabel *dimensionInputTitle = new QLabel("Puzzle Dimension:", this);
	uiCell->addWidget(dimensionInputTitle, 0, 0);
	uiCell->addWidget(dimensionInput, 0, 1);

	buildGrid();

	setLayout(grid);

	connect(solveButton, &QPushButton::clicked, this, &GridUI::solve);
	connect(dimensionInput, &QLineEdit::returnPressed, this, &GridUI::buildGrid);
}

void GridUI::buildGrid() {
	gridDimension = dimensionInput->text().toInt();
	inputGridOffset = (gridDimension / 2) + gridDimension % 2;


	QLayoutItem *toDelete;
	while ((toDelete = vertInputCell->takeAt(0)) != 0) {
		delete toDelete->widget();
	}
	while ((toDelete = horInputCell->takeAt(0)) != 0) {
		delete toDelete->widget();
	}
	while ((toDelete = puzzleGridCell->takeAt(0)) != 0) {
		delete toDelete->widget();
	}

	for (int inCol = 0; inCol < gridDimension; ++inCol) {
		for (int inBox = 0; inBox < inputGridOffset; ++inBox) {
			QLineEdit *temp = new QLineEdit("", this);
			temp->setFixedWidth(20);
			horInputCell->addWidget(temp, inCol, inBox);
			temp->show();
		}
		
	}
	
	for (int inRow = 0; inRow < gridDimension; ++inRow) {
		for (int inBox = 0; inBox < inputGridOffset ; ++inBox) {
			QLineEdit *temp = new QLineEdit(" ", this);
			temp->setFixedWidth(20);
			vertInputCell->addWidget(temp, inBox, inRow);
			temp->show();
		}
	}
	
	for (int puzzCol = 0; puzzCol < gridDimension; ++puzzCol) {
		for (int puzzRow = 0; puzzRow < gridDimension; ++puzzRow) {
			QLineEdit *puzzBox = new QLineEdit("", this);
			puzzBox->setReadOnly(true);
			puzzBox->setFixedWidth(20);
			puzzleGridCell->addWidget(puzzBox, puzzCol, puzzRow);
			puzzBox->show();
		}
	}

	resize(0,0);
	return;
}

bool GridUI::solve() {

	QVector<QVector<int>> segmentInputs;
	
	for (int col = 0; col < gridDimension; ++col) {
		QVector<int> rowSegments;
		
		for ( int row = 0; row < inputGridOffset; ++row) {
			int temp = static_cast<QLineEdit*>(horInputCell->itemAtPosition(col, row)->widget())->text().toInt();

			if ( temp != 0 )
				rowSegments.append(temp);
		}
		segmentInputs.append(rowSegments);
	}
	
	for (int row = 0; row < gridDimension; ++row) {
		QVector<int> colSegments;

		for (int col = 0; col < inputGridOffset; ++col) {
			int temp = static_cast<QLineEdit*>(vertInputCell->itemAtPosition(col, row)->widget())->text().toInt();
	
			if ( temp != 0 ) 
				colSegments.append(temp);
		}
		segmentInputs.append(colSegments);
	}

	std::cout<<"solve entered."<<std::endl;
	Grid solutionBoard(gridDimension, segmentInputs);
	std::cout<<"Board Made."<<std::endl;
	solutionBoard.solve();
	std::cout<<"Solve passed."<<std::endl;
	updateGrid(solutionBoard);
	std::cout<<"updateGrid passed."<<std::endl;
	return true;
}

void GridUI::updateGrid(Grid& newGrid) {
	for (int puzzCol = 0; puzzCol < gridDimension; ++puzzCol) {
		for (int puzzRow = 0; puzzRow < gridDimension; ++puzzRow) {
			switch (newGrid.boxStateAt(puzzCol,puzzRow)) {
				case OPEN:
					break;
				case FILLED:
					puzzleGridCell->itemAtPosition(puzzRow, puzzCol)->widget()->setStyleSheet("QLineEdit { background-color: blue }");
					break;
				case CLOSED:
					puzzleGridCell->itemAtPosition(puzzRow, puzzCol)->widget()->setStyleSheet("QLineEdit { background-color: red }");
					break;
			}
		}
	} 
}

