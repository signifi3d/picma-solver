#include <QVector>
#include <QString>
#include <iostream>
#include "SegmentList.h"

SegmentList::SegmentList() {
	row = 0;
	column = 0;
	size = 0;
}

SegmentList::SegmentList(int x, int y, int inSize, QString inputString) {
	row = x;
	column = y;
	size = inSize;
	if (inputString.size() > 0) {
		QStringList splitInput = inputString.split(' ');
		for (int i = 0; i < splitInput.size(); ++i) {
			int prevSegmentSize = 0;
			int nextSegmentSize = 0;
			for (int j = 0; j < i; ++j) {
				prevSegmentSize += splitInput[j].toInt()+1;
			}
			for (int j = i + 1; j < splitInput.size(); ++j) {
				nextSegmentSize += splitInput[j].toInt()+1;
			}
			line.append(Segment(splitInput[i].toInt(), size, prevSegmentSize, nextSegmentSize));
		}
	}
}

void SegmentList::printSegments() {
	for (int i = 0; i < line.size(); i++) {
	}
}

int SegmentList::getRow() {
	return row;
}

int SegmentList::getColumn() {
	return column;
}

Segment& SegmentList::getSegment(int segmentIndex) {
	return line[segmentIndex];
}

int SegmentList::numOfSegments() {
	return line.size();
}

bool SegmentList::mustBeFilled(int position) {
	for (int i = 0; i < line.size(); ++i) {
		if ( line[i].allPossibleSpansIntersectAt(position) ) {
			return true;
		}
	}
	return false;
}

bool SegmentList::hasAsAPossibility(int position) {
	for (int i = 0; i < line.size(); ++i) {
		if ( !line[i].noPossibleSpansContain(position) )
			return true;
	}
	return false;
}

void SegmentList::compareWithLineState(LineState currentState) {
	if ( currentState.numOfSpans() == 0 ) return;
	int latestPairedState = 0;
	for (int i = 0; i < line.size(); ++i) {
		if ( line[i].isComplete() ) continue;
		for (int j = latestPairedState; j < currentState.numOfSpans(); ++j) {
			if ( currentState.getBoxSpanNum(j).getLowerBound() < line[i].getLowestPossibleBound() ) continue;
			if ( line[i].getHighestPossibleBound() < currentState.getBoxSpanNum(j).getUpperBound() ) {
				break;
			}
			if ( currentState.getBoxSpanNum(j).range()+1 == line[i].getSize() ) {
				if ( currentState.getBoxSpanNum(j).getLowerBound() < line[i].getSize() + 1 && i == 0 ) {
					line[i].setComplete(currentState.getBoxSpanNum(j));
					latestPairedState = j;
					break;
				}
				if ( size - currentState.getBoxSpanNum(j).getUpperBound() < line[i].getSize() + 1 && i == line.size()-1 ) {
					line[i].setComplete(currentState.getBoxSpanNum(j));
					return;
				}
				if ( line[i].getHighestPossibleBound() - line[i].getLowestPossibleBound() + 1 - line[i].getSize() < line[i].getSize()) {
					line[i].setComplete(currentState.getBoxSpanNum(j));
					latestPairedState = j;
					break;
				}
				if ( isLargestUniqueSegment(i) ) {
					line[i].setComplete(currentState.getBoxSpanNum(j));
					latestPairedState = j;
					break;
				}
			} 
		}		
	}
}

void SegmentList::seekSolePositionOwnership(int position) {
	int positionOwnerIndex = -1;
	for (int i = 0; i < line.size(); ++i) {
		if ( !line[i].noPossibleSpansContain(position) ) {
			if ( positionOwnerIndex == -1 ) {
				positionOwnerIndex = i;
			} else {
				return;
			}
		} 
	}
	if ( positionOwnerIndex == -1 ) return;
	if ( line[positionOwnerIndex].isComplete() ) return;
	line[positionOwnerIndex].removePossibleSpansWithout(position);
}

bool SegmentList::isLargestUniqueSegment(int segmentIndex) {
	if ( size - line[segmentIndex].getSize() < size / 2 ) return true;
	for (int i = 0; i < line.size(); ++i) {
		if (!line[i].isComplete() && (line[i].getSize() > line[segmentIndex].getSize() || (line[i].getSize() == line[segmentIndex].getSize() && i != segmentIndex)) ) return false;
	}
	return true;
}
