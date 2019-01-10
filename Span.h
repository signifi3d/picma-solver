#pragma once

class Span {
	public:
		Span();
		Span(int,int);
		int getLowerBound();
		int getUpperBound();
		int range();
		void setLowerBound(int);
		void setUpperBound(int);
		bool spanContains(int);
	private:
		int lowerBound;
		int upperBound;
	
	
};
