#pragma once

#include <QVector>
#include "Span.h"


class Segment {
	public:
		Segment();
		Segment(int,int,int,int);
		int getSize();
		bool allPossibleSpansIntersectAt(int);
		bool containsSpan(Span);
		bool noPossibleSpansContain(int);
		void removePossibleSpansWithout(int);
		void removePossibleSpansWith(int);
		bool isComplete();
		void setComplete(Span);
		int numOfPossibleSpans();
		Span getPossibleSpanAt(int);
		int getLowestPossibleBound();
		int getHighestPossibleBound();
	private:
		QVector<Span> possibleSpans;
		int size;
		Span possibleRange;
		bool complete;
};
