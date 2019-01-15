#include "LineState.h"
#include "Span.h"
#include <QVector>

LineState::LineState() {}

void LineState::addSpan(Span inSpan) {
	filledBoxes.insert(inSpan);
}

int LineState::numOfSpans() {
	return filledBoxes.size();
}

Span getBoxSpanNum(int index) {
	return filledBoxes[index];
}
