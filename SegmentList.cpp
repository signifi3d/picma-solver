#include <QVector>
#include <QString>
#include <iostream>
#include "SegmentList.h"

SegmentList::SegmentList() {
	size = 0;
}

SegmentList::SegmentList(int inSize, QVector<int> inputVector) {
	size = inSize;
	for (int i = 0; i < inputVector.size(); ++i) {
		int prevSegmentSize = 0;
		int nextSegmentSize = 0;
		for (int j = 0; j < i; ++j) {
			prevSegmentSize += inputVector[j]+1;
		}
		for (int j = i + 1; j < inputVector.size(); ++j) {
			nextSegmentSize += inputVector[j]+1;
		}
		line.append(Segment(inputVector[i], size, prevSegmentSize, nextSegmentSize));
	}
}

void SegmentList::printSegments() {
	for (int i = 0; i < line.size(); i++) {
		std::cout << line[i].getSize() << " ";
	}
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

void SegmentList::setSegmentAsComplete(int segNo, Span completionSpan) {
	for (int i = 0; i < line.size(); ++i) {
		if ( i == segNo ) 
			line[i].setComplete(completionSpan);
		else {
			int lowerBound, upperBound;
			if ( i < segNo ) {
				lowerBound = completionSpan.getLowerBound();
				upperBound = size-1;
			} else if ( i > segNo ) {
				lowerBound = 0;
				upperBound = completionSpan.getUpperBound();
			} else {
				lowerBound = completionSpan.getLowerBound();
				upperBound = completionSpan.getUpperBound();
			}
			for (; lowerBound <= upperBound; ++lowerBound)
				line[i].removePossibleSpansWith(lowerBound);
		}
	}
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
					setSegmentAsComplete(i, currentState.getBoxSpanNum(j));
					latestPairedState = j;
					break;
				}
				if ( size - currentState.getBoxSpanNum(j).getUpperBound() < line[i].getSize() + 1 && i == line.size()-1 ) {
					setSegmentAsComplete(i, currentState.getBoxSpanNum(j));
					return;
				}
				if ( line[i].getHighestPossibleBound() - line[i].getLowestPossibleBound() + 1 - line[i].getSize() < line[i].getSize()) {
					setSegmentAsComplete(i, currentState.getBoxSpanNum(j));
					latestPairedState = j;
					break;
				}
				if ( i != 0 ) {
					if ( line[i].getLowestPossibleBound() == currentState.getBoxSpanNum(j).getLowerBound() && line[i-1].isComplete() ) {
						setSegmentAsComplete(i, currentState.getBoxSpanNum(j));
						break;
					}
				}
				if ( numOfLargestNonCompleteSegments(line[i].getSize()) == 1 ) {
					setSegmentAsComplete(i, currentState.getBoxSpanNum(j));
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

bool SegmentList::spanIsComplete(Span testSpan) {

	for (int i = 0; i < line.size(); ++i) {
		if ( line[i].isComplete() && line[i].containsSpan(testSpan) )
			return true;
	}
	
	return false;
}

/* returns count of largest segments in the line if segment size
   provided is the largest non-complete segment. Returns 0 
   otherwise.
*/
int SegmentList::numOfLargestNonCompleteSegments(int segmentSize) {
	int count = 0;

	for (int i = 0; i < line.size(); ++i) {
		if (!line[i].isComplete() && line[i].getSize() > segmentSize ) 
			return 0;
		else if ( !line[i].isComplete() && line[i].getSize() == segmentSize )
			++count;
	}
	return count;
}
