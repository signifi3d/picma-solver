#include "LineState.h"
#include "Span.h"
#include <QVector>

LineState::LineState() {}

void LineState::addSpan(Span inSpan) {
	filledBoxes.append(inSpan);
}

int LineState::numOfSpans() {
	return filledBoxes.size();
}

Span LineState::getBoxSpanNum(int index) {
	return filledBoxes[index];
}
