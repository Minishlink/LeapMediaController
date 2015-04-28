#include "leapmediacontroller.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	LeapMediaController w;
	w.show();

	return a.exec();
}