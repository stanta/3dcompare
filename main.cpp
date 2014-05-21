
#include "stlviewer.h"
#include <QtGui/QApplication>

int main(int argc, char *argv[]) 
{
	Q_INIT_RESOURCE(stlsearcher);
	QApplication::setStyle("cleanlooks");
	QApplication a(argc, argv);
	a.setWindowIcon(QIcon(":STLSearcher/Images/stl.png"));
	a.setApplicationVersion("1.0");
	STLViewer w;
	w.show();
  return a.exec();
}
