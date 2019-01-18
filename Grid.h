#pragma once

#include <QVector>
#include <QHash>
#include <QPair>
#include <QLineEdit>
#include "PicmaBox.h"
#include "SegmentList.h"

class Grid {
	public:
		Grid(int, QVector<QVector<QLineEdit*>>&);
		void solve();
		boxState boxStateAt(int, int);
		void setStateAt(int, int, boxState);
	private:
		QVector<QVector<PicmaBox>> *board;
		QHash<QPair<int,int>, SegmentList> lineSegments;
		
		void checkPossibleSpanIntersections();
		void checkForCompleteSegments();
		void fillClosedBoxes();
		void checkInevitableCompletions();
		void pairLooseFilledBoxes();
		bool puzzleSolved();
		
		void debugBoardPrint();
};
