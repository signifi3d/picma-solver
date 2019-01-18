#pragma once

#include <QVector>
#include <QString>
#include "Segment.h"
#include "LineState.h"

class SegmentList {
	public:
		SegmentList();
		SegmentList(int,int, int, QString);
		void printSegments();
		int getRow();
		int getColumn();
		int numOfSegments();
		Segment& getSegment(int);
		bool mustBeFilled(int);
		bool hasAsAPossibility(int);
		void setSegmentAsComplete(int,Span);
		void compareWithLineState(LineState);
		void seekSolePositionOwnership(int);
		bool isLargestUniqueSegment(int);
	private:
		QVector<Segment> line;
		int row;
		int column;
		int size;
};
