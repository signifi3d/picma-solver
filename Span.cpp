#include "Span.h"

Span::Span() {
	lowerBound = 0;
	upperBound = 0;
}

Span::Span(int lower=0, int upper=0) {
	lowerBound = lower;
	upperBound = upper;
}

int Span::getLowerBound() {
	return lowerBound;
}	

int Span::getUpperBound() {
	return upperBound;
}

int Span::range() {
	return upperBound - lowerBound;
}

void Span::setLowerBound(int lower) {
	lowerBound = lower;
}

void Span::setUpperBound(int upper) {
	upperBound = upper;
}

bool Span::spanContains(int test) {
	if ( test >= lowerBound && test <= upperBound ) {
		return true;
	}
	return false;
}
