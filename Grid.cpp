#include "Grid.h"
#include "PicmaBox.h"
#include "SegmentList.h"
#include <QVector>
#include <QLineEdit>

//debug
#include <iostream>

Grid::Grid(int size, QVector<QVector<QLineEdit*>> &inputGrid) {
	board = new QVector<QVector<PicmaBox>>(size-1, QVector<PicmaBox>(size, PicmaBox()));
	for (int i = 1; i < size; i++) {
		lineSegments.append(SegmentList(i-1, 0, size-1, inputGrid[0][i]->text()));
	}
	for (int i = 1; i < size; i++) {
		lineSegments.append(SegmentList(0, i-1, size-1, inputGrid[i][0]->text()));
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
}

void Grid::checkPossibleSpanIntersections() {
	for (int i = 0; i < lineSegments.size(); ++i) {
		for ( int j = 0; j < board->size(); ++j ) {
			for ( int k = 0; k < lineSegments[i].numOfSegments(); ++k) {
				if ( lineSegments[i].getSegment(k).allPossibleSpansIntersectAt(j) ) {
					if ( lineSegments[i].getRow() == 0) {
						setStateAt(lineSegments[i].getColumn(), j, FILLED);
					} else {
						setStateAt(j, lineSegments[i].getRow(), FILLED);
					}
					break;
				} 
			}	
		}
	}
}
