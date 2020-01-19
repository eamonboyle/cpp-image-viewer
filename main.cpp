#include <QtWidgets/QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	MainWindow  window;
	window.setWindowTitle("ImageViewer");
	window.show();

	return a.exec();
}