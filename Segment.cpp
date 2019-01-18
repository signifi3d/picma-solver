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
		possibleSpans.append(Span(i+smallestSegmentBeginning, ((i+smallestSegmentBeginning)+segmentSize)-1));
		/*
		for ( int j = smallestSegmentBeginning+i; j < (possibleRange.getLowerBound()+i)+(possibleRange.range()/segmentSize); ++j ) {
			possibleSpans.append(Span(j, (j+segmentSize)-1));
		}
		*/
	}
}

int Segment::getSize() {
	return size;
}

bool Segment::isComplete() {
	return complete;
}

void Segment::setComplete(Span completeSpan) {
	complete = true;
	for (int i = 0; i < possibleSpans.size(); ++i) {
		if ( possibleSpans[i].getLowerBound() < completeSpan.getLowerBound() || possibleSpans[i].getUpperBound() > completeSpan.getUpperBound() ) {
			possibleSpans.remove(i); 
			--i;
		}
	}
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
			--i;
		}
	}

	return;
}

void Segment::removePossibleSpansWith( int gridPoint ) {
	
	for (int i = 0; i < possibleSpans.size(); ++i) {
		if ( possibleSpans[i].spanContains(gridPoint) ) {
			possibleSpans.remove(i);
			--i;
		}
	}
}

int Segment::numOfPossibleSpans() {
	return possibleSpans.size();
}

Span Segment::getPossibleSpanAt(int i) {
	return possibleSpans[i];
}

int Segment::getLowestPossibleBound() {
	int lowestBound = -1;
	for (int i = 0; i < possibleSpans.size(); ++i) {
		if ( possibleSpans[i].getLowerBound() < lowestBound || lowestBound == -1)
			lowestBound = possibleSpans[i].getLowerBound();
	}
	return lowestBound;
}

int Segment::getHighestPossibleBound() {
	int highestBound = 0;
	for (int i = 0; i < possibleSpans.size(); ++i) {
		if ( possibleSpans[i].getUpperBound() > highestBound )
			highestBound = possibleSpans[i].getUpperBound();
	}
	return highestBound;
}

bool Segment::noPossibleSpansContain(int position) {
	for (int i = 0; i < possibleSpans.size(); ++i)
		if ( possibleSpans[i].spanContains(position) ) return false;
	return true;
}
