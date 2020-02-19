#pragma once

#include <QVector>
#include <QString>
#include "Segment.h"
#include "LineState.h"

class SegmentList {
	public:
		SegmentList();
		SegmentList(int, QVector<int>);
		void printSegments();
		int numOfSegments();
		Segment& getSegment(int);
		bool mustBeFilled(int);
		bool hasAsAPossibility(int);
		void setSegmentAsComplete(int,Span);
		void compareWithLineState(LineState);
		void seekSolePositionOwnership(int);
		bool spanIsComplete(Span);
		int numOfLargestNonCompleteSegments(int);
	private:
		QVector<Segment> line;
		int size;
};
