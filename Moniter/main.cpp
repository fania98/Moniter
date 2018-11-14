#include "videoview.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	videoview w;
	w.show();
	return a.exec();
}
