#include "bhxserver.h"
#include <QtWidgets/QApplication>
#include <QTextCodec>
#include <QFileInfo>
#include <QtPlugin> 
Q_IMPORT_PLUGIN(QWindowsIntegrationPlugin);


int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	BHXServer w;
	w.show();
	return a.exec();
}
