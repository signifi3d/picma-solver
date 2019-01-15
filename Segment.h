#pragma once

#include <QVector>
#include "Span.h"


class Segment {
	public:
		Segment();
		Segment(int,int,int,int);
		int getSize();
		bool allPossibleSpansIntersectAt(int);
		void removePossibleSpansWithout(int);
		bool isComplete();
		void setComplete(Span);
	private:
		QVector<Span> possibleSpans;
		int size;
		Span possibleRange;
		bool complete;
};
