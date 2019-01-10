#pragma once

#include <QVector>
#include <QString>
#include "Segment.h"

class SegmentList {
	public:
		SegmentList();
		SegmentList(int,int, int, QString);
		void printSegments();
		int getRow();
		int getColumn();
		int numOfSegments();
		Segment& getSegment(int);
	private:
		QVector<Segment> line;
		int row;
		int column;
		int size;
};
