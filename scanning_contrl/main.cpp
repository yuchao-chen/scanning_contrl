#include "scanning_contrl.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	ScanningContrl w;
	w.show();
	return a.exec();
}
