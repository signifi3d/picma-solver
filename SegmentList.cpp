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
		std::cout << line[i].getSize() << " ";
	}
	std::cout << std::endl;
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
