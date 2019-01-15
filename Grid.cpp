#include "Grid.h"
#include "PicmaBox.h"
#include "SegmentList.h"
#include "LineState.h"
#include <QVector>
#include <QLineEdit>
#include <QHash>
#include <QPair>

//debug
#include <iostream>

Grid::Grid(int size, QVector<QVector<QLineEdit*>> &inputGrid) {
	board = new QVector<QVector<PicmaBox>>(size-1, QVector<PicmaBox>(size, PicmaBox()));
	for (int i = 1; i < size; i++) {
		lineSegments.insert(QPair<int,int>(0,i-1), SegmentList(i-1, 0, size-1, inputGrid[0][i]->text()));
	}
	for (int i = 1; i < size; i++) {
		lineSegments.insert(QPair<int,int>(i-1,0), SegmentList(0, i-1, size-1, inputGrid[i][0]->text()));
	}
	/*DEBUG
	for (int i = 0; i < lineSegments.size(); i++) {
		lineSegments[i].printSegments();
	}
	*/
} 

boxState Grid::boxStateAt(int x, int y) {
	return (*board)[x][y].currentState;
}

void Grid::setStateAt(int x, int y, boxState newState) {
	(*board)[x][y].currentState = newState;
}

void Grid::solve() {
	checkPossibleSpanIntersections();
	checkForCompleteSegments();
	fillClosedBoxes();
}

void Grid::checkForCompleteSegments() {
	for (int x = 0; x < board->size(); ++x) {
		LineState currentLineState;
		Span filledBoxSpan(-1,-1);
		for (int y = 0; y < board->size(); ++y) {
			if ( boxStateAt(x,y) == FILLED && filledBoxSpan.getLowerBound() == -1 ) {
				filledBoxSpan.setLowerBound(y);
			} else if ( boxStateAt(x,y) != FILLED && filledBoxSpan.getLowerBound() != -1 ) {
				filledBoxSpan.setUpperBound(y-1);
				currentLineState.addSpan(filledBoxSpan);
				filledBoxSpan.setLowerBound(-1);
				filledBoxSpan.setUpperBound(-1);
			}
		}
		lineSegments[QPair<int,int>(x,0)].compareWithLineState(currentLineState);
	}
	for (int y = 0; y < board->size(); ++y) {
		LineState currentLineState;
		Span filledBoxSpan(-1,-1);
		for (int x = 0; x < board->size(); ++x) {
			if ( boxStateAt(x,y) == FILLED && filledBoxSpan.getLowerBound() == -1) {
				filledBoxSpan.setLowerBound(x);
			} else if (boxStateAt(x,y) != FILLED && filledBoxSpan.getLowerBound() != -1 ) {
				filledBoxSpan.setUpperBound(x-1);
				currentLineState.addSpan(filledBoxSpan);
				filledBoxSpan.setLowerBound(-1);
				filledBoxSpan.setUpperBound(-1);
			}
		}
		lineSegments[QPair<int,int>(0,y)].compareWithLineState(currentLineState);
	}
}

void Grid::fillClosedBoxes() {

}

void Grid::checkPossibleSpanIntersections() {
	for (int x = 0; x < board->size(); ++x) {
		for (int y = 0; y < board->size(); ++y) {
			if (boxStateAt(x,y) == OPEN) {
				if ( lineSegments[QPair<int,int>(x,0)].mustBeFilled(y) ) {
					setStateAt(x, y, FILLED);
					continue;
				} else if ( lineSegments[QPair<int,int>(0,y)].mustBeFilled(x) ) {
					setStateAt(x, y, FILLED);
					continue;
				}
			}
		}
	}
}
