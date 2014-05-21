TEMPLATE = app
TARGET = ./bin/STLSearcher
#DEPENDPATH += ./src
#INCLUDEPATH += ./src
QT += sql
QT += network
QT += opengl
# QT += qt3support
#win32:CONFIG  += qaxcontainer 

HEADERS += axisglwidget.h \
		axisgroupbox.h \
        dimensionsgroupbox.h \
 		glmdichild.h \
		glwidget.h \
		meshinformationgroupbox.h \
		propertiesgroupbox.h \
		stlfile.h \
		stlviewer.h\
		vector.h  \
		stlsearcher.h  

SOURCES += axisglwidget.cpp \
		axisgroupbox.cpp \
		dimensionsgroupbox.cpp \
		glmdichild.cpp \
		main.cpp \
		glwidget.cpp \
		meshinformationgroupbox.cpp \
		propertiesgroupbox.cpp \
		stlfile.cpp \
		stlviewer.cpp \
		vector.cpp \
		stlsearcher.cpp  
		
#FORMS += 	src/mainwindow.ui \

CONFIG += warn_on \
	thread \
	qt \
	debug 
	
RESOURCES += stlsearcher.qrc
#RC_FILE += GameControl.rc

#include(stl_source/stl_source.pri)

# set mysql plugin path
#unix:MYSQL_DIR = /usr/include/mysql 
#win32:MYSQL_DIR = "C:\Program Files\MySQL\MySQL Server 5.5"

unix {
	use_libmysqlclient_r:LIBS     *= -lmysqlclient_r
	else:LIBS    *= -lmysqlclient
	LIBS += -L/usr/lib
}
win32 {
#	LIBS += $$MYSQL_DIR/lib/mysqlclient.lib  #-L$$MYSQL_DIR/lib/opt
	LIBS += C:/QtSDK/Desktop/Qt/4.7.3/msvc2008/plugins/sqldrivers/qsqlite4.lib
#	LIBS += C:/QtSDK/Desktop/Qt/4.7.3/msvc2008/plugins/sqldrivers/qsqlmysql4.lib
}