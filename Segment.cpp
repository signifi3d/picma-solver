#include <QVector>
#include "Segment.h"
#include "Span.h"

//DEBUG
#include <iostream>

Segment::Segment() {
	
}

Segment::Segment(int segmentSize, int lineSize, int smallestSegmentBeginning, int largestSegmentEnding) {

	possibleRange.setLowerBound(smallestSegmentBeginning);
	possibleRange.setUpperBound(lineSize-largestSegmentEnding);
	size = segmentSize;
	complete = false;

	for (int i = 0; i <= possibleRange.range()-segmentSize; ++i) {
		for ( int j = smallestSegmentBeginning+i; j < (possibleRange.getLowerBound()+i)+(possibleRange.range()/segmentSize); ++j ) {
			possibleSpans.append(Span(j, (j+segmentSize)-1));
		}
	}
}

int Segment::getSize() {
	return size;
}

bool Segment::isComplete() {
	return complete;
}

void Segment::setComplete(Span completeSpan) {
	complete = !complete;
	removeAllPossibleSpansWithout(completeSpan.getLowerBound());
	removeAllPossibleSpansWithout(completeSpan.getUpperBound());
}

bool Segment::allPossibleSpansIntersectAt( int gridPoint ) {
	for ( int i = 0; i < possibleSpans.size(); ++i ) {
		if ( !possibleSpans[i].spanContains(gridPoint) ) {
			return false;
		}	
	}
	
	return true;
}

void Segment::removePossibleSpansWithout( int gridPoint ) {

	for (int i = 0; i < possibleSpans.size(); ++i ) {
		if ( !possibleSpans[i].spanContains(gridPoint) ) {
			possibleSpans.remove(i);
		}
	}

	return;
}
