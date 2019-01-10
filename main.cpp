#include <QApplication>
#include "GridUI.h"

int main( int argc, char* argv[] ) {

	QApplication app(argc, argv);

	GridUI window;

	window.move(300,300);
	window.setWindowTitle("Picma Solver");
	window.show();

	return app.exec();

}
