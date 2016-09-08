#include "bhxserver.h"
#include <QtWidgets/QApplication>
#include <QTextCodec>
#include <QFileInfo>
#include <QtPlugin> 
Q_IMPORT_PLUGIN(QWindowsIntegrationPlugin);
Q_IMPORT_PLUGIN(QSQLiteDriverPlugin);
#pragma execution_character_set("utf-8")


int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	BHXServer w;
	w.setWindowTitle("³¬ËÙ-·þÎñÆ÷");
	w.show();



	return a.exec();
}
