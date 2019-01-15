#pragma once
#include "Span.h"
#include <QVector>

class LineState {
	public:
		LineState();
		void addSpan(Span);
		int numOfSpans();
		Span getBoxSpanNum(int);

	private:
		QVector<Span> filledBoxes;
};
