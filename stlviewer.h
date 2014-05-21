// Copyright (c) 2014 Andranik Abrahamyan

#ifndef STLVIEWER_H
#define STLVIEWER_H

#include <QtGui/QMainWindow>
#include <QtGui/QWidget>

#include "glmdichild.h"

class AxisGroupBox;
class DimensionsGroupBox;
class MeshInformationGroupBox;
class PropertiesGroupBox;
class QAction;
class QMenu;
class QLabel;
class QMdiArea;
class QMdiSubWindow;
class QSignalMapper;

class STLViewer : public QMainWindow {
Q_OBJECT
public:
	STLViewer(QWidget *parent = 0, Qt::WFlags flags = 0);
	~STLViewer();
signals:
  void	leftMouseButtonModeChanged(GLWidget::LeftMouseButtonMode mode);

protected:
	void	closeEvent(QCloseEvent *event);

private slots:
	void	newFile();  
	void	open();
	void	save();
	void	saveAs();
	void	saveImage();
	void	rotate();
	void	panning();
	void	zoom();
	void	unzoom();
	void	backView();	
	void	frontView();
	void	leftView();
	void	rightView();
	void	topView();
	void	bottomView();
	void	topFrontLeftView();
	void	wireframe();
	void	radialframe();
	void	about();
	void	updateMenus();
	void	updateWindowMenu();
	void	setMousePressed(Qt::MouseButtons button);
	void	setMouseReleased(Qt::MouseButtons button);
	void	setActiveSubWindow(QWidget *window);
	void	destroyGLMdiChild();
	GLMdiChild *	createGLMdiChild();

private:
	void	createActions();
	void	createMenus();
	void	createToolBars();
	void	createStatusBar();
	void	createDockWindows();
	// Reads persistent platform-independent application settings
	void	readSettings();
	// Writes persistent platform-independent application settings
	void	writeSettings();
	GLMdiChild *	activeGLMdiChild();
	QMdiSubWindow *	findGLMdiChild(const QString &fileName);
	QMdiArea *		mdiArea;
	QSignalMapper*	windowMapper;
	QMenu *		fileMenu;
	QMenu *		windowMenu;
	QMenu *		viewMenu;
	QMenu *		defaultViewsMenu;
	QMenu *		helpMenu;
	QToolBar *	m_fileToolBar;
	QToolBar *	m_viewToolBar;
	QAction *	m_newAct;
	QAction *	openAct;
	QAction *	saveAct;
	QAction *	saveAsAct;
	QAction *	saveImageAct;
	QAction *	closeAct;
	QAction *	closeAllAct;
	QAction *	tileAct;
	QAction *	cascadeAct;
	QAction *	nextAct;
	QAction *	previousAct;
	QAction *	separatorAct;
	QAction *	zoomAct;
	QAction *	rotateAct;
	QAction *	panningAct;
	QAction *	unzoomAct;
	QAction *	backViewAct;
	QAction *	m_RadialViewAct;
	QAction *	frontViewAct;
	QAction *	leftViewAct;
	QAction *	rightViewAct;
	QAction *	topViewAct;
	QAction *	bottomViewAct;
	QAction *	topFrontLeftViewAct;
	QAction *	wireframeAct;
	QAction *	exitAct;
	QAction *	aboutAct;
	QString		curDir;
	GLWidget::LeftMouseButtonMode leftMouseButtonMode;
	AxisGroupBox *	axisGroupBox;
	DimensionsGroupBox *	 dimensionsGroupBox;
	MeshInformationGroupBox* meshInformationGroupBox;
	PropertiesGroupBox *	 propertiesGroupBox;
};
#endif // STLVIEWER_H