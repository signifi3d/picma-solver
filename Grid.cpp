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
		lineSegments.insert(QPair<int,int>(0,i), SegmentList(i-1, 0, size-1, inputGrid[0][i]->text()));
	}
	for (int i = 1; i < size; i++) {
		lineSegments.insert(QPair<int,int>(i,0), SegmentList(0, i-1, size-1, inputGrid[i][0]->text()));
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
	if ( newState == CLOSED ) {
		SegmentList curr = lineSegments[QPair<int,int>(0, y+1)];
		for (int i = 0; i < curr.numOfSegments(); ++i) 
			curr.getSegment(i).isComplete() ? continue : curr.getSegment(i).removePossibleSpansWith(x);

		curr = lineSegments[QPair<int,int>(x+1, 0)];
		for (int i = 0; i < curr.numOfSegments(); ++i)
			curr.getSegment(i).isComplete() ? continue : curr.getSegment(i).removePossibleSpansWith(y);
	}
	(*board)[x][y].currentState = newState;
}

void Grid::solve() {
	checkPossibleSpanIntersections();
	checkForCompleteSegments();
	/*
	for (int x = 0; x < board->size(); ++x) {
		SegmentList curr = lineSegments[QPair<int,int>(x+1,0)];
		for (int i = 0; i < curr.numOfSegments(); ++i) {
			if ( curr.getSegment(i).isComplete() ) {
				std::cout << x << ", 0 segment " << i << " is complete " << curr.getSegment(i).getLowestPossibleBound() << "-"<<curr.getSegment(i).getHighestPossibleBound() <<  std::endl;
			} else {
				std::cout << x << ", 0 segment " << i << " not complete" << std::endl;
			}
		}
	}
	for (int y = 0; y < board->size(); ++y) {
		SegmentList curr = lineSegments[QPair<int,int>(0,y+1)];
		for (int i = 0; i < curr.numOfSegments(); ++i) {
			if ( curr.getSegment(i).isComplete() ) {
				std::cout << "0, " << y << " segment " << i << " is complete " << curr.getSegment(i).getLowestPossibleBound() <<"-"<<curr.getSegment(i).getHighestPossibleBound() << std::endl;
			} else {
				std::cout << "0, " << y << " segment " << i << " not complete" << std::endl;
			}
		}
	}*/

	fillClosedBoxes();
	debugBoardPrint();
}

void Grid::checkForCompleteSegments() {
	for (int x = 0; x < board->size(); ++x) {
		LineState currentLineState;
		Span filledBoxSpan(-1,-1);
		for (int y = 0; y < board->size(); ++y) {
			if ( boxStateAt(x,y) == FILLED && filledBoxSpan.getLowerBound() == -1 ) {
				filledBoxSpan.setLowerBound(y);
			}
			if ( (boxStateAt(x,y) != FILLED || y == board->size()-1) && filledBoxSpan.getLowerBound() != -1 ) {
				filledBoxSpan.setUpperBound(boxStateAt(x,y)!=FILLED?y-1:y);
				currentLineState.addSpan(filledBoxSpan);
				filledBoxSpan.setLowerBound(-1);
				filledBoxSpan.setUpperBound(-1);
			}
		}
		/* DEBUG 
		std::cout << "0, " << x << " state: ";
		for (int i = 0; i < currentLineState.numOfSpans(); ++i) {
			std::cout << "Range " << i+1 << " " << currentLineState.getBoxSpanNum(i).getLowerBound() << "-" << currentLineState.getBoxSpanNum(i).getUpperBound() << " ";
		}
		std::cout << std::endl;
		 */
		lineSegments[QPair<int,int>(x+1,0)].compareWithLineState(currentLineState);
	}
	for (int y = 0; y < board->size(); ++y) {
		LineState currentLineState;
		Span filledBoxSpan(-1,-1);
		for (int x = 0; x < board->size(); ++x) {
			if ( boxStateAt(x,y) == FILLED && filledBoxSpan.getLowerBound() == -1) {
				filledBoxSpan.setLowerBound(x);
			}
			if ((boxStateAt(x,y) != FILLED || x == board->size()-1 ) && filledBoxSpan.getLowerBound() != -1 ) {
				filledBoxSpan.setUpperBound(boxStateAt(x,y)!=FILLED ? x-1 : x);
				currentLineState.addSpan(filledBoxSpan);
				filledBoxSpan.setLowerBound(-1);
				filledBoxSpan.setUpperBound(-1);
			}
		}
		lineSegments[QPair<int,int>(0,y+1)].compareWithLineState(currentLineState);
	}
}

void Grid::fillClosedBoxes() {
	for (int x = 0; x < board->size(); ++x) {
		SegmentList columnSegments = lineSegments[QPair<int,int>(x+1,0)];
		for (int segNo = 0; segNo < columnSegments.numOfSegments(); ++segNo) {
			if ( columnSegments.getSegment(segNo).isComplete() ) {
				if ( segNo == 0 ) {
					for (int lineSpace = 0; lineSpace < columnSegments.getSegment(segNo).getLowestPossibleBound(); ++lineSpace) {
						setStateAt(x,lineSpace,CLOSED);
					}
					setStateAt(x, columnSegments.getSegment(segNo).getHighestPossibleBound()+1, CLOSED);
				} else if ( segNo == columnSegments.numOfSegments()-1 ) {
					for (int lineSpace = columnSegments.getSegment(segNo).getHighestPossibleBound()+1; lineSpace < board->size(); ++lineSpace) {
						setStateAt(x,lineSpace,CLOSED);
					}
					setStateAt(x, columnSegments.getSegment(segNo).getLowestPossibleBound()-1, CLOSED);
					continue;
				} 
				if ( columnSegments.getSegment(segNo+1).isComplete() ) {
					for (int lineSpace = columnSegments.getSegment(segNo).getHighestPossibleBound()+1; lineSpace < columnSegments.getSegment(segNo+1).getLowestPossibleBound(); ++lineSpace) {
						setStateAt(x,lineSpace,CLOSED);
					}
				}
				setStateAt(x, columnSegments.getSegment(segNo).getLowestPossibleBound()-1, CLOSED);
				setStateAt(x, columnSegments.getSegment(segNo).getHighestPossibleBound()+1, CLOSED);
			}
		}
	}
	for (int y = 0; y < board->size(); ++y) {
		SegmentList rowSegments = lineSegments[QPair<int,int>(0,y+1)];
		for (int segNo = 0; segNo < rowSegments.numOfSegments(); ++segNo) {
			if ( rowSegments.getSegment(segNo).isComplete() ) {
				if ( segNo == 0 ) {
					for (int lineSpace = 0; lineSpace < rowSegments.getSegment(segNo).getLowestPossibleBound(); ++lineSpace) {
						setStateAt(lineSpace,y,CLOSED);
					}
					setStateAt(rowSegments.getSegment(segNo).getHighestPossibleBound()+1,y, CLOSED);
				} else if ( segNo == rowSegments.numOfSegments()-1 ) {
					for (int lineSpace = rowSegments.getSegment(segNo).getHighestPossibleBound()+1; lineSpace < board->size(); ++lineSpace) {
						setStateAt(lineSpace,y,CLOSED);
					}
					setStateAt(rowSegments.getSegment(segNo).getLowestPossibleBound()-1,y, CLOSED);
					break;
				} else {
					setStateAt(rowSegments.getSegment(segNo).getLowestPossibleBound()-1,y, CLOSED);
					setStateAt(rowSegments.getSegment(segNo).getHighestPossibleBound()+1,y, CLOSED);
				}
				if ( rowSegments.getSegment(segNo+1).isComplete() ) {
					for (int lineSpace = rowSegments.getSegment(segNo).getHighestPossibleBound()+1; lineSpace < rowSegments.getSegment(segNo+1).getLowestPossibleBound(); ++lineSpace) {
						setStateAt(lineSpace,y,CLOSED);
					}
				}
			}
		}
	}
}

void Grid::checkPossibleSpanIntersections() {
	for (int x = 0; x < board->size(); ++x) {
		for (int y = 0; y < board->size(); ++y) {
			if (boxStateAt(x,y) == OPEN) {
				if ( lineSegments[QPair<int,int>(x+1,0)].mustBeFilled(y) ) {
					setStateAt(x, y, FILLED);
					continue;
				} else if ( lineSegments[QPair<int,int>(0,y+1)].mustBeFilled(x) ) {
					setStateAt(x, y, FILLED);
					continue;
				}
			}
		}
	}
}

void Grid::debugBoardPrint() {
	for (int y = 0; y < board->size(); ++y) {
		for (int x = 0; x < board->size(); ++x) {
			switch (boxStateAt(x,y)) {
				case OPEN:
					std::cout << "? ";
					break;
				case CLOSED:
					std::cout << "x ";
					break;
				case FILLED:
					std::cout << "+ ";
					break;
				default:
					std::cout << "E ";
			}
		}
		std::cout << std::endl;
	}
}
