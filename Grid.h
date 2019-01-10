#pragma once

#include <QVector>
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
		QVector<SegmentList> lineSegments;
		
		void checkPossibleSpanIntersections();
};
