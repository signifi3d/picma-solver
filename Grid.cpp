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
#include <assert.h>

Grid::Grid(int size, QVector<QVector<int>> &inputGrid) {
	board = new QVector<QVector<PicmaBox>>(size, QVector<PicmaBox>(size, PicmaBox()));

	for (int i = 0; i < inputGrid.size() / 2; ++i) 
		lineSegments.insert(QPair<int,int>(0,i+1), SegmentList(size, inputGrid[i]));
	for (int i = inputGrid.size() / 2; i < inputGrid.size(); ++i) 
		lineSegments.insert(QPair<int,int>((i-(inputGrid.size()/2))+1,0), SegmentList(size, inputGrid[i]));
} 

boxState Grid::boxStateAt(int x, int y) {
	return (*board)[x][y].currentState;
}

void Grid::setStateAt(int x, int y, boxState newState) {
	if ( newState == (*board)[x][y].currentState ) return;
	if ( newState == CLOSED ) {
		SegmentList *curr = &lineSegments[QPair<int,int>(0, y+1)];
		for (int i = 0; i < curr->numOfSegments(); ++i) { 
			if ( !curr->getSegment(i).isComplete() ) {
				 curr->getSegment(i).removePossibleSpansWith(x);
			}
		}

		curr = &lineSegments[QPair<int,int>(x+1, 0)];
		for (int i = 0; i < curr->numOfSegments(); ++i) {
			if ( !curr->getSegment(i).isComplete() ) {
				 curr->getSegment(i).removePossibleSpansWith(y);
			}
		}
	}
	(*board)[x][y].currentState = newState;
}

void Grid::solve() {
	for (int i = 0; i < 100; ++i) {
		checkPossibleSpanIntersections();
		checkForCompleteSegments();
		std::cout << "+ fillclosed " << std::endl;
		fillClosedBoxes();
		std::cout << "+ inevitable " << std::endl;
		checkInevitableCompletions();
		std::cout << "+ 2ndcompletes " << std::endl;
		checkForCompleteSegments();
		std::cout << "+ 2ndfill " << std::endl;
		fillClosedBoxes();
		std::cout << "+ loose " << std::endl;
		pairLooseFilledBoxes();
		std::cout << "+ 2ndinevitable " << std::endl;
		checkInevitableCompletions();
		std::cout << "+ 3rdcompletes " << std::endl;
		checkForCompleteSegments();
		std::cout << "+ 3rdfill " << std::endl;
		fillClosedBoxes();
		
		if ( puzzleSolved() )
			break;
	}	
	for (int x = 0; x < board->size(); ++x) {
		SegmentList curr = lineSegments[QPair<int,int>(x+1,0)];
		for (int i = 0; i < curr.numOfSegments(); ++i) {
			if (!curr.getSegment(i).isComplete())
				std::cout << x << ", 0 : " << curr.getSegment(i).getSize() << std::endl;
		}
	}
	for (int y = 0; y < board->size(); ++y) {
		SegmentList curr = lineSegments[QPair<int,int>(0,y+1)];
		for (int i = 0; i < curr.numOfSegments(); ++i) {
			if (!curr.getSegment(i).isComplete())
				std::cout << "0, " << y << ": " << curr.getSegment(i).getSize() << std::endl;
		}
	}
}

void Grid::pairLooseFilledBoxes() {
	for (int x = 0; x < board->size(); ++x) {
		for (int y = 0; y < board->size(); ++y) {
			if ( boxStateAt(x,y) == FILLED ) {
				lineSegments[QPair<int,int>(x+1,0)].seekSolePositionOwnership(y);
				lineSegments[QPair<int,int>(0,y+1)].seekSolePositionOwnership(x);
			}	
		}
	}
}

void Grid::checkInevitableCompletions() {
	for ( int x = 0; x < board->size(); ++x) {
		SegmentList *curr = &lineSegments[QPair<int,int>(x+1, 0)];
		for (int segNo = 0; segNo < curr->numOfSegments(); ++segNo) {
			if ( !curr->getSegment(segNo).isComplete() && curr->getSegment(segNo).numOfPossibleSpans() == 1) {
				for (int y = curr->getSegment(segNo).getLowestPossibleBound(); y <= curr->getSegment(segNo).getHighestPossibleBound(); ++y) {
					setStateAt(x,y,FILLED);
					curr->setSegmentAsComplete(segNo,Span(curr->getSegment(segNo).getLowestPossibleBound(),curr->getSegment(segNo).getHighestPossibleBound()));
				}
			}
		}
	}
	for (int y = 0; y < board->size(); ++y) {
		SegmentList *curr = &lineSegments[QPair<int,int>(0, y+1)];
		for (int segNo = 0; segNo < curr->numOfSegments(); ++segNo) {
			if ( !curr->getSegment(segNo).isComplete() && curr->getSegment(segNo).numOfPossibleSpans() == 1) {
				for (int x = curr->getSegment(segNo).getLowestPossibleBound(); x <= curr->getSegment(segNo).getHighestPossibleBound(); ++x) {
					setStateAt(x,y,FILLED);
					curr->setSegmentAsComplete(segNo, Span(curr->getSegment(segNo).getLowestPossibleBound(),curr->getSegment(segNo).getHighestPossibleBound()));
				}
			}
		}
	}
}

void Grid::checkForCompleteSegments() {
	for (int x = 0; x < board->size(); ++x) 
		lineSegments[QPair<int,int>(x+1,0)].compareWithLineState(getVerticalLineState(x));
	for (int y = 0; y < board->size(); ++y) 
		lineSegments[QPair<int,int>(0,y+1)].compareWithLineState(getHorizontalLineState(y));
}

void Grid::fillClosedBoxes() {
	for (int x = 0; x < board->size(); ++x) {
		SegmentList columnSegments = lineSegments[QPair<int,int>(x+1,0)];
		for (int segNo = 0; segNo < columnSegments.numOfSegments(); ++segNo) {
			if ( columnSegments.getSegment(segNo).isComplete() && columnSegments.getSegment(segNo).getSize() != board->size()) {
				if ( segNo == 0 ) {
					for (int lineSpace = 0; lineSpace < columnSegments.getSegment(segNo).getLowestPossibleBound(); ++lineSpace) {
						setStateAt(x,lineSpace,CLOSED);
					}
					if ( !(columnSegments.getSegment(segNo).getHighestPossibleBound()+1 >= board->size()) )
						setStateAt(x, columnSegments.getSegment(segNo).getHighestPossibleBound()+1, CLOSED);
				} else if ( segNo == columnSegments.numOfSegments()-1 ) {
					for (int lineSpace = columnSegments.getSegment(segNo).getHighestPossibleBound()+1; lineSpace < board->size(); ++lineSpace) {
						setStateAt(x,lineSpace,CLOSED);
					}
					setStateAt(x, columnSegments.getSegment(segNo).getLowestPossibleBound()-1, CLOSED);
					break;
				} else {
					setStateAt(x, columnSegments.getSegment(segNo).getLowestPossibleBound()-1, CLOSED);
					setStateAt(x, columnSegments.getSegment(segNo).getHighestPossibleBound()+1, CLOSED);
				}
				if ( segNo+1 >= columnSegments.numOfSegments() ) break;
				else if ( columnSegments.getSegment(segNo+1).isComplete() ) {
					for (int lineSpace = columnSegments.getSegment(segNo).getHighestPossibleBound()+1; lineSpace < columnSegments.getSegment(segNo+1).getLowestPossibleBound(); ++lineSpace) {
						setStateAt(x,lineSpace,CLOSED);
					}
				}
			}
		}
	}
	for (int y = 0; y < board->size(); ++y) {
		SegmentList rowSegments = lineSegments[QPair<int,int>(0,y+1)];
		for (int segNo = 0; segNo < rowSegments.numOfSegments(); ++segNo) {
			if ( rowSegments.getSegment(segNo).isComplete() && rowSegments.getSegment(segNo).getSize() != board->size()) {
				if ( segNo == 0 ) {
					for (int lineSpace = 0; lineSpace < rowSegments.getSegment(segNo).getLowestPossibleBound(); ++lineSpace) {
						setStateAt(lineSpace,y,CLOSED);
					}
					if ( !(rowSegments.getSegment(segNo).getHighestPossibleBound()+1 >= board->size()) )
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
				if ( segNo+1 >= rowSegments.numOfSegments() ) break;
				else if ( rowSegments.getSegment(segNo+1).isComplete() ) {
					for (int lineSpace = rowSegments.getSegment(segNo).getHighestPossibleBound()+1; lineSpace < rowSegments.getSegment(segNo+1).getLowestPossibleBound(); ++lineSpace) {
						setStateAt(lineSpace,y,CLOSED);
					}
				}
			}
		}
	}
	for (int x = 0; x < board->size(); ++x) {
		for (int y = 0; y < board->size(); ++y) {
			if ( boxStateAt(x,y) == OPEN ) {
				if ( !lineSegments[QPair<int,int>(x+1,0)].hasAsAPossibility(y) || !lineSegments[QPair<int,int>(0,y+1)].hasAsAPossibility(x) )
					setStateAt(x,y,CLOSED); 
			}
		}
	}
	//Check horizontal and vertical line states, look for largest spans that aren't complete, if those spans are the largest unfound spans then close sides
	for (int x = 0; x < board->size(); ++x) {
		QVector<Span> largestIncompleteSpans;
		LineState currState = getVerticalLineState(x);

		for (int i = 0; i < currState.numOfSpans(); ++i) {
			if ( !lineSegments[QPair<int,int>(x+1, 0)].spanIsComplete( currState.getBoxSpanNum(i) ) ) {
				if ( largestIncompleteSpans.size() == 0 ) {
					largestIncompleteSpans.append(currState.getBoxSpanNum(i));
				} else {
					if ( currState.getBoxSpanNum(i).range() > largestIncompleteSpans[0].range() ) {
						largestIncompleteSpans.clear();
						largestIncompleteSpans.append(currState.getBoxSpanNum(i));
					} else if ( currState.getBoxSpanNum(i).range() == largestIncompleteSpans[0].range() ) {
						largestIncompleteSpans.append(currState.getBoxSpanNum(i));
					}
				}
			}
		}
		if ( largestIncompleteSpans.size() > 0 ) {
			if ( lineSegments[QPair<int,int>(x+1,0)].numOfLargestNonCompleteSegments( largestIncompleteSpans[0].range()+1 ) > 0 ) {
				for (int i = 0; i < largestIncompleteSpans.size(); ++i) {
					if ( largestIncompleteSpans[i].getLowerBound() != 0 ) 
						setStateAt(x, largestIncompleteSpans[i].getLowerBound()-1, CLOSED);
					if ( largestIncompleteSpans[i].getUpperBound() != board->size()-1 )
						setStateAt(x, largestIncompleteSpans[i].getUpperBound()+1, CLOSED);	
				}
			}
		}
		
	}
	for (int y = 0; y < board->size(); ++y) {
		QVector<Span> largestIncompleteSpans;
		LineState currState = getHorizontalLineState(y);

		for (int i = 0; i < currState.numOfSpans(); ++i) {
			if ( !lineSegments[QPair<int,int>(0, y+1)].spanIsComplete( currState.getBoxSpanNum(i) ) ) {
				if ( largestIncompleteSpans.size() == 0 ) {
					largestIncompleteSpans.append(currState.getBoxSpanNum(i));
				} else {
					if ( currState.getBoxSpanNum(i).range() > largestIncompleteSpans[0].range() ) {
						largestIncompleteSpans.clear();
						largestIncompleteSpans.append(currState.getBoxSpanNum(i));
					} else if ( currState.getBoxSpanNum(i).range() == largestIncompleteSpans[0].range() ) {
						largestIncompleteSpans.append(currState.getBoxSpanNum(i));
					}
				}
			}
		}

		if ( largestIncompleteSpans.size() > 0 ) {
			if ( lineSegments[QPair<int,int>(0,y+1)].numOfLargestNonCompleteSegments( largestIncompleteSpans[0].range()+1 ) > 0 ) {
				for (int i = 0; i < largestIncompleteSpans.size(); ++i) {
					if ( largestIncompleteSpans[i].getLowerBound() != 0 ) 
						setStateAt(largestIncompleteSpans[i].getLowerBound()-1, y, CLOSED);
					if ( largestIncompleteSpans[i].getUpperBound() != board->size()-1 )
						setStateAt(largestIncompleteSpans[i].getUpperBound()+1, y, CLOSED);	
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

bool Grid::puzzleSolved() {
	for (int x = 0; x < board->size(); ++x) {
		for (int y = 0; y < board->size(); ++y) {
			if ( boxStateAt(x,y) == OPEN )
				return false;
		}
	}
	return true;
}

LineState Grid::getVerticalLineState(int colNum) {
	LineState currLineState;
	Span filledBoxSpan(-1,-1);

	for (int y = 0; y < board->size(); ++y) {
		if ( boxStateAt(colNum,y) == FILLED && filledBoxSpan.getLowerBound() == -1 ) {
			filledBoxSpan.setLowerBound(y);
		}
		if ( (boxStateAt(colNum,y) != FILLED || y == board->size()-1) && filledBoxSpan.getLowerBound() != -1 ) {
			filledBoxSpan.setUpperBound(boxStateAt(colNum,y)!=FILLED?y-1:y);
			currLineState.addSpan(filledBoxSpan);
			filledBoxSpan.setLowerBound(-1);
			filledBoxSpan.setUpperBound(-1);
		}
	}

	return currLineState;

}

LineState Grid::getHorizontalLineState(int rowNum) {
	LineState currLineState;
	Span filledBoxSpan(-1,-1);

	for (int x = 0; x < board->size(); ++x) {
		if ( boxStateAt(x, rowNum) == FILLED && filledBoxSpan.getLowerBound() == -1 ) {
			filledBoxSpan.setLowerBound(x);
		}
		if ( (boxStateAt(x, rowNum) != FILLED || x == board->size()-1) && filledBoxSpan.getLowerBound() != -1 ) {
			filledBoxSpan.setUpperBound(boxStateAt(x,rowNum)!=FILLED?x-1:x);
			currLineState.addSpan(filledBoxSpan);
			filledBoxSpan.setLowerBound(-1);
			filledBoxSpan.setUpperBound(-1);
		}
	}

	return currLineState;
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
