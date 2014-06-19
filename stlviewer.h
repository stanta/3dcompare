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

/*! \brief  The STLViewer class provides a main application window.
*
*	STLViewer has its own layout to which added QToolBars, QMenuBar, QStatusBar, 
*     AxisGroupBox, DimensionsGroupBox,MeshInformationGroupBox and and PropertiesGroupBox objects
*	  STLViewer have multiple (MDI) document interface*/

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
	void	newFile();  //!< new File create action 
	void	open(); //!< File open action
	void	save(); //!< File save action
	void	saveAs(); //!< File save as action
	void	saveAsSphere(); //!< File save as sphere action	
	void	saveAsNormalFrequency(); //!< File save as normals frequency action
	void	compareLastMeshes(); //!< compare last 2 meshes action	
	void	saveImage(); //!< get snapshot from mesh action
	void	rotate(); //!< rotate mesh action	
	void	panning();	//!< panning the mesh action
	void	zoom();	//!< zoom mesh action
	void	unzoom(); //!< unzoom mesh action
	void	backView();	 //!< The mesh back view  action
	void	frontView(); //!< The mesh front view  action
	void	leftView(); //!< The mesh left view  action
	void	rightView(); //!< The mesh right view  action
	void	topView(); //!< The mesh top view  action
	void	bottomView(); //!< The mesh bootom view  action
	void	topFrontLeftView(); //!< The mesh top front left view  action
	/// wire frame view mesh action
	void	wireframe();
	/// radial frame view mesh action
	void	radialframe();
	/// about program action
	void	about();
	/// update menus for each OpenGL child window
	void	updateMenus();
	/// update menus for the window
	void	updateWindowMenu();
	/// sets mouse pressed state
	void	setMousePressed(Qt::MouseButtons button);
	/// sets mouse released state
	void	setMouseReleased(Qt::MouseButtons button);
	/// sets set  active sub window
	void	setActiveSubWindow(QWidget *window);
	/// destroy OpenGL Child window
	void	destroyGLMdiChild();
	/// creates OpenGL Child window
	GLMdiChild *	createGLMdiChild();

private:
	/// creates actions
	void	createActions();
	/// creates nenus
	void	createMenus();
	/// creates tool bars
	void	createToolBars();
	/// creates status bar
	void	createStatusBar();
	/// creates dock windows
	void	createDockWindows();
	/// Reads persistent platform-independent application settings
	void	readSettings();
	/// Writes persistent platform-independent application settings
	void	writeSettings();
	/// Gets the active OpenGL window
	GLMdiChild *	activeGLMdiChild();
	/// finds the child OpenGL window
	QMdiSubWindow *	findGLMdiChild(const QString &fileName);
	/// Mdi area , QMenu, QToolBar, QAction objects
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
	QAction *	saveAsSphereAct;
	QAction *	saveAsNormalFreqAct;
	QAction *	compareLastMeshesAct;
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
	/// current directory
	QString		curDir;
	GLWidget::LeftMouseButtonMode leftMouseButtonMode;
	/// AxisGroupBox dock window
	AxisGroupBox *			axisGroupBox;
	/// DimensionsGroupBox dock window
	DimensionsGroupBox *	 dimensionsGroupBox;
	/// MeshInformationGroupBox dock window
	MeshInformationGroupBox* meshInformationGroupBox;
	/// PropertiesGroupBox dock window
	PropertiesGroupBox *	 propertiesGroupBox;
};
#endif // STLVIEWER_H