// Copyright (c) 2014 Andranik Abrahamyan

#include <QtGui/QtGui>

#include <iostream>
#include <fstream>

#include "stlviewer.h"
#include "axisgroupbox.h"
#include "dimensionsgroupbox.h"
#include "meshinformationgroupbox.h"
#include "propertiesgroupbox.h"
#include "stlcompare.h"

STLViewer::STLViewer(QWidget *parent, Qt::WFlags flags)
    : QMainWindow(parent, flags) {
  mdiArea = new QMdiArea;
  mdiArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
  mdiArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
  setCentralWidget(mdiArea);
  connect(mdiArea, SIGNAL(subWindowActivated(QMdiSubWindow *)),
       this, SLOT(updateMenus()));
  windowMapper = new QSignalMapper(this);
  connect(windowMapper, SIGNAL(mapped(QWidget *)),
       this, SLOT(setActiveSubWindow(QWidget *)));
  createActions();
  createMenus();
  createToolBars();
  createStatusBar();
  createDockWindows();
  updateMenus();
  // Read persistent application settings
  readSettings();
  // Deactivate the left mouse button when manipulating objects
  leftMouseButtonMode = GLWidget::INACTIVE;
  setWindowTitle(tr("STL Searcher"));
  setUnifiedTitleAndToolBarOnMac(true);
}

STLViewer::~STLViewer() {}

void STLViewer::closeEvent(QCloseEvent *event) {
  mdiArea->closeAllSubWindows();
  if (activeGLMdiChild()) {
    event->ignore();
  } else {
    writeSettings();
    event->accept();
  }
}

void STLViewer::newFile() 
{
	GLMdiChild *child = createGLMdiChild();
	child->newFile();
	child->show();
	// Reset all informations
	axisGroupBox->reset();
	dimensionsGroupBox->reset();
	meshInformationGroupBox->reset();
	propertiesGroupBox->reset();
}

void STLViewer::open()
{
	QString fileName = QFileDialog::getOpenFileName(this, tr("Open a file"),
				curDir, tr("STL Files (*.stl);;All Files (*.*)"));
	if (!fileName.isEmpty()) {
		curDir = QFileInfo(fileName).filePath();
		QMdiSubWindow *existing = findGLMdiChild(fileName);
		if (existing) {
			mdiArea->setActiveSubWindow(existing);
			return;
		}
		GLMdiChild *child = createGLMdiChild();
		if (child->loadFile(fileName)) {
			statusBar()->showMessage(tr("File loaded"), 2000);
			child->show();
		} else {
			setActiveSubWindow(child);
			mdiArea->closeActiveSubWindow();
			//child->close();
		}
	}
}

void STLViewer::save() 
{
	if (activeGLMdiChild() && activeGLMdiChild()->save())
		statusBar()->showMessage(tr("File saved"), 2000);
}

void STLViewer::saveAs()
{
	if (activeGLMdiChild() && activeGLMdiChild()->saveAs())
		statusBar()->showMessage(tr("File saved"), 2000);
}

void STLViewer::saveAsSphere()
{
	if (activeGLMdiChild() && activeGLMdiChild()->saveAsSphere())
		statusBar()->showMessage(tr("File saved as Sphere"), 2000);
}

void STLViewer::saveAsNormalFrequency()
{
	if (activeGLMdiChild() && activeGLMdiChild()->saveAsNormalFrequency())
		statusBar()->showMessage(tr("File saved as Normal Frequency"), 2000);
}

void STLViewer::compareLastMeshes()
{
	QList<QMdiSubWindow *> subList = mdiArea->subWindowList();
	int subSize = subList.size();
	if (subSize > 1) {
		statusBar()->showMessage(tr("Compare last two meshes"), 2000);

		GLMdiChild *child1 = qobject_cast<GLMdiChild *>(subList.at(subSize-1)->widget());
		GLMdiChild *child2 = qobject_cast<GLMdiChild *>(subList.at(subSize-2)->widget());
		StlCompare	compare;
		
		char res = compare.init (child1->m_stlFile, child2->m_stlFile);
		if (res != -1) {
			char buf[4];
			_itoa(res, buf, 10);
			QMessageBox msgBox;
			QRect geo = msgBox.geometry();
			msgBox.resize (geo.width () + 40, geo.height () + 20);
			msgBox.setText("Result of the Huffman Code.");

			QString	str = "first file: "; 			
			str += child1->m_curFile ;
			str += "\n"; 
			str += "second file: "; 			
			str += child2->m_curFile;
			str += "\n\n"; 
			str += "The percent equality of two mesh is ";
			str += buf;
			str += "%";
			msgBox.setInformativeText(str);			
			msgBox.exec();
		}

	}
}



void STLViewer::saveImage()
{
	if (activeGLMdiChild() && activeGLMdiChild()->saveImage())
		statusBar()->showMessage(tr("Image saved"), 2000);
}

void STLViewer::rotate() 
{
	if (rotateAct->isChecked()) {
		panningAct->setChecked(false);
		leftMouseButtonMode = GLWidget::ROTATE;
	} else {
		leftMouseButtonMode = GLWidget::INACTIVE;
	}
	emit leftMouseButtonModeChanged(leftMouseButtonMode);
}

void STLViewer::panning()
{
	if (panningAct->isChecked()) {
		rotateAct->setChecked(false);
		leftMouseButtonMode = GLWidget::PANNING;
	} else {
		leftMouseButtonMode = GLWidget::INACTIVE;
	}
	emit leftMouseButtonModeChanged(leftMouseButtonMode);
}

void STLViewer::wireframe() 
{
  activeGLMdiChild()->setWireframeMode(wireframeAct->isChecked());
  //emit wireframeStatusChanged(wireframeAct->isChecked());
}

void STLViewer::radialframe()
{
	activeGLMdiChild()->setRadialMode(m_RadialViewAct->isChecked ());
}


void STLViewer::zoom()
{
	activeGLMdiChild()->zoom();
}

void STLViewer::unzoom() 
{
	activeGLMdiChild()->unzoom();
}

void STLViewer::backView()
{
	activeGLMdiChild()->setBackView();
}


void STLViewer::frontView() 
{
	activeGLMdiChild()->setFrontView();
}

void STLViewer::leftView()
{
	activeGLMdiChild()->setLeftView();
}

void STLViewer::rightView()
{
	activeGLMdiChild()->setRightView();
}

void STLViewer::topView() 
{
	activeGLMdiChild()->setTopView();
}

void STLViewer::bottomView()
{
	activeGLMdiChild()->setBottomView();
}

void STLViewer::topFrontLeftView() 
{
	activeGLMdiChild()->setTopFrontLeftView();
}

void STLViewer::about()
{
  QMessageBox::about(this, tr("About STLSearcher"), tr(
      "<p align=\"center\">STLSearcher</p>"
      "<p align=\"center\">v%1</p>"
      "<p align=\"center\">Copyright (c) 2014 Andranik Abrahamyan</p>"
      "<p align=\"center\">Web: <a href=\"http://www.Ayvah.com\">"
      "www.Ayvah.com</a></p>"
      "<p align=\"center\">Mail: <a href=\"mailto:Ayvah@gmail.com\">"
      "Ayvah@gmail.com</a></p>"
      ).arg(QCoreApplication::applicationVersion()));
}

void STLViewer::updateMenus() 
{
	bool hasGLMdiChild = (activeGLMdiChild() != 0);
	if (hasGLMdiChild && !activeGLMdiChild()->m_isUntitled) {
		saveAct->setEnabled(true);
		saveAsAct->setEnabled(true);
		saveAsSphereAct->setEnabled(true);
		saveAsNormalFreqAct->setEnabled(true);
		compareLastMeshesAct->setEnabled(true);
	} else {
		saveAct->setEnabled(false);
		saveAsAct->setEnabled(false);
		saveAsSphereAct->setEnabled(false);
		saveAsNormalFreqAct->setEnabled(false);
		compareLastMeshesAct->setEnabled(false);
	}
	saveImageAct->setEnabled(hasGLMdiChild);
	closeAct->setEnabled(hasGLMdiChild);
	closeAllAct->setEnabled(hasGLMdiChild);
	zoomAct->setEnabled(hasGLMdiChild);
	rotateAct->setEnabled(hasGLMdiChild);
	panningAct->setEnabled(hasGLMdiChild);
	unzoomAct->setEnabled(hasGLMdiChild);
	wireframeAct->setEnabled(hasGLMdiChild);
	if (hasGLMdiChild)
		wireframeAct->setChecked(activeGLMdiChild()->isWireframeModeActivated());
	else
		wireframeAct->setChecked(false);

	m_RadialViewAct->setEnabled(hasGLMdiChild);
	if (hasGLMdiChild)
		m_RadialViewAct->setChecked(activeGLMdiChild()->isRadialModeActivated());
	else
		m_RadialViewAct->setChecked(false);

	backViewAct->setEnabled(hasGLMdiChild);
	backViewAct->setEnabled(m_RadialViewAct);
	
	m_RadialViewAct->setEnabled(hasGLMdiChild);
	frontViewAct->setEnabled(hasGLMdiChild);
	leftViewAct->setEnabled(hasGLMdiChild);
	rightViewAct->setEnabled(hasGLMdiChild);
	topViewAct->setEnabled(hasGLMdiChild);
	bottomViewAct->setEnabled(hasGLMdiChild);
	topFrontLeftViewAct->setEnabled(hasGLMdiChild);
	tileAct->setEnabled(hasGLMdiChild);
	cascadeAct->setEnabled(hasGLMdiChild);
	nextAct->setEnabled(hasGLMdiChild);
	previousAct->setEnabled(hasGLMdiChild);
	separatorAct->setVisible(hasGLMdiChild);
	if (hasGLMdiChild && !activeGLMdiChild()->m_isUntitled) {
		axisGroupBox->setXRotation(activeGLMdiChild()->getXRot());
		axisGroupBox->setYRotation(activeGLMdiChild()->getYRot());
		axisGroupBox->setZRotation(activeGLMdiChild()->getZRot());
		dimensionsGroupBox->setValues(activeGLMdiChild()->getSearcherStats());
		meshInformationGroupBox->setValues(activeGLMdiChild()->getStats());
		propertiesGroupBox->setValues(activeGLMdiChild()->getSearcherStats());
	} else {
		axisGroupBox->reset();
		dimensionsGroupBox->reset();
		meshInformationGroupBox->reset();
		propertiesGroupBox->reset();
	}
}

void STLViewer::updateWindowMenu()
{
	windowMenu->clear();
	windowMenu->addAction(closeAct);
	windowMenu->addAction(closeAllAct);
	windowMenu->addSeparator();
	windowMenu->addAction(tileAct);
	windowMenu->addAction(cascadeAct);
	windowMenu->addSeparator();
	windowMenu->addAction(nextAct);
	windowMenu->addAction(previousAct);
	windowMenu->addAction(separatorAct);
	QList<QMdiSubWindow *> windows = mdiArea->subWindowList();
	separatorAct->setVisible(!windows.isEmpty());
	for (int i = 0; i < windows.size(); ++i) {
		GLMdiChild *child = qobject_cast<GLMdiChild *>(windows.at(i)->widget());
		QString text;
		if (i < 9)
			text = tr("&%1 %2").arg(i + 1).arg(child->userFriendlyCurrentFile());
		else
			text = tr("%1 %2").arg(i + 1).arg(child->userFriendlyCurrentFile());

		QAction *action  = windowMenu->addAction(text);
		action->setCheckable(true);
		action ->setChecked(child == activeGLMdiChild());
		connect(action, SIGNAL(triggered()), windowMapper, SLOT(map()));
		windowMapper->setMapping(action, windows.at(i));
	}
}

void STLViewer::setMousePressed(Qt::MouseButtons button) {
	if (button & Qt::RightButton) {
		rotateAct->setChecked(true);
	} else if (button & Qt::MidButton) {
		panningAct->setChecked(true);
	}
}

void STLViewer::setMouseReleased(Qt::MouseButtons button) {
	if (button & Qt::RightButton) {
		if (leftMouseButtonMode != GLWidget::ROTATE)
			rotateAct->setChecked(false);
	} else if (button & Qt::MidButton) {
		if (leftMouseButtonMode != GLWidget::PANNING)
			panningAct->setChecked(false);
	}
}

GLMdiChild *STLViewer::createGLMdiChild()
{
  GLMdiChild *child = new GLMdiChild;
  mdiArea->addSubWindow(child);
  child->setLeftMouseButtonMode(leftMouseButtonMode);
  connect(child, SIGNAL(mouseButtonPressed(Qt::MouseButtons)), this,
    SLOT(setMousePressed(Qt::MouseButtons)));
  connect(child, SIGNAL(mouseButtonReleased(Qt::MouseButtons)), this,
    SLOT(setMouseReleased(Qt::MouseButtons)));
  connect(this,
    SIGNAL(leftMouseButtonModeChanged(GLWidget::LeftMouseButtonMode)), child,
    SLOT(setLeftMouseButtonMode(GLWidget::LeftMouseButtonMode)));
  connect(child, SIGNAL(destroyed()), this, SLOT(destroyGLMdiChild()));
  connect(child, SIGNAL(xRotationChanged(const int)), axisGroupBox,
          SLOT(setXRotation(const int)));
  connect(child, SIGNAL(yRotationChanged(const int)), axisGroupBox,
          SLOT(setYRotation(const int)));
  connect(child, SIGNAL(zRotationChanged(const int)), axisGroupBox,
          SLOT(setZRotation(const int)));
  return child;
}

void STLViewer::setActiveSubWindow(QWidget *window)
{
  if (!window)
		return;
  mdiArea->setActiveSubWindow(qobject_cast<QMdiSubWindow *>(window));
}

void STLViewer::destroyGLMdiChild()
{
  if (activeGLMdiChild() == 0) {
    panningAct->setChecked(false);
    rotateAct->setChecked(false);
    leftMouseButtonMode = GLWidget::INACTIVE;
    emit leftMouseButtonModeChanged(leftMouseButtonMode);
  }
}

void STLViewer::createActions() 
{
	QIcon newAct_icon(":STLSearcher/Images/new_doc.png");
	m_newAct = new QAction(newAct_icon, tr("&New"), this);
	m_newAct->setShortcuts(QKeySequence::New);
	m_newAct->setStatusTip(tr("Create a new file"));
	connect(m_newAct, SIGNAL(triggered()), this, SLOT(newFile()));

	openAct = new QAction(QIcon(":STLSearcher/Images/folder.png"), tr("&Open..."), this);
	openAct->setShortcuts(QKeySequence::Open);
	openAct->setStatusTip(tr("Open an existing file"));
	 connect(openAct, SIGNAL(triggered()), this, SLOT(open()));

	saveAct = new QAction(QIcon(":STLSearcher/Images/save.png"), tr("&Save"), this);
	saveAct->setShortcuts(QKeySequence::Save);
	saveAct->setStatusTip(tr("Save the document to disk"));
	connect(saveAct, SIGNAL(triggered()), this, SLOT(save()));

	saveAsAct = new QAction(tr("Save &As..."), this);
	saveAsAct->setShortcut(QKeySequence::SaveAs);
	saveAsAct->setStatusTip(tr("Save the document under a new name"));
	connect(saveAsAct, SIGNAL(triggered()), this, SLOT(saveAs()));

	saveAsSphereAct = new QAction(tr("Save as Spherical"), this);
	saveAsSphereAct->setShortcut(QKeySequence::UnknownKey);
	saveAsSphereAct->setStatusTip(tr("Save as Spherical "));
	connect(saveAsSphereAct, SIGNAL(triggered()), this, SLOT(saveAsSphere()));

	saveAsNormalFreqAct = new QAction(tr("Save as Normal Frequency"), this);
	saveAsNormalFreqAct->setShortcut(QKeySequence::UnknownKey);
	saveAsNormalFreqAct->setStatusTip(tr("Save as Normal Frequency "));
	connect(saveAsNormalFreqAct, SIGNAL(triggered()), this, SLOT(saveAsNormalFrequency()));

	compareLastMeshesAct = new QAction(tr("Compare last two meshes"), this);
	compareLastMeshesAct->setShortcut(QKeySequence::UnknownKey);
	compareLastMeshesAct->setStatusTip(tr("Compare last two meshes "));
	connect(compareLastMeshesAct, SIGNAL(triggered()), this, SLOT(compareLastMeshes()));

	saveImageAct = new QAction(tr("Save Image..."), this);
	saveImageAct->setShortcut(tr("Ctrl+I"));
	saveImageAct->setStatusTip(tr("Save the current view to disk"));
	connect(saveImageAct, SIGNAL(triggered()), this, SLOT(saveImage()));

	closeAct = new QAction(tr("Cl&ose"), this);
	closeAct->setShortcut(tr("Ctrl+W"));
	closeAct->setStatusTip(tr("Close the active window"));
	connect(closeAct, SIGNAL(triggered()),  mdiArea, SLOT(closeActiveSubWindow()));

  closeAllAct = new QAction(tr("Close &All"), this);
  closeAllAct->setStatusTip(tr("Close all the windows"));
  connect(closeAllAct, SIGNAL(triggered()),
    mdiArea, SLOT(closeAllSubWindows()));

  tileAct = new QAction(tr("&Tile"), this);
  tileAct->setStatusTip(tr("Tile the windows"));
  connect(tileAct, SIGNAL(triggered()), mdiArea, SLOT(tileSubWindows()));

  cascadeAct = new QAction(tr("&Cascade"), this);
  cascadeAct->setStatusTip(tr("Cascade the windows"));
  connect(cascadeAct, SIGNAL(triggered()), mdiArea, SLOT(cascadeSubWindows()));

  nextAct = new QAction(tr("Ne&xt"), this);
  nextAct->setShortcuts(QKeySequence::NextChild);
  nextAct->setStatusTip(tr("Move the focus to the next window"));
  connect(nextAct, SIGNAL(triggered()),
      mdiArea, SLOT(activateNextSubWindow()));

  previousAct = new QAction(tr("Pre&vious"), this);
  previousAct->setShortcuts(QKeySequence::PreviousChild);
  previousAct->setStatusTip(tr("Move the focus to the previous "
      "window"));
  connect(previousAct, SIGNAL(triggered()),
      mdiArea, SLOT(activatePreviousSubWindow()));

  separatorAct = new QAction(this);
  separatorAct->setSeparator(true);

  rotateAct = new QAction(QIcon(":STLSearcher/Images/arrow_rotate_clockwise.png"),
    tr("&Rotate"), this);
  rotateAct->setShortcut(tr("R"));
  rotateAct->setStatusTip(tr("Rotate the object"));
  rotateAct->setCheckable(true);
  connect(rotateAct, SIGNAL(triggered()), this, SLOT(rotate()));
  rotateAct->setChecked(false);
    
  panningAct = new QAction(QIcon(":STLSearcher/Images/arrow_out.png"), tr("&Pan"),
    this);
  panningAct->setShortcut(tr("P"));
  panningAct->setStatusTip(tr("Drag the object around"));
  panningAct->setCheckable(true);
  connect(panningAct, SIGNAL(triggered()), this, SLOT(panning()));
  panningAct->setChecked(false);

  zoomAct = new QAction(QIcon(":STLSearcher/Images/magnifier_zoom_in.png"),
    tr("&Zoom In"), this);
  zoomAct->setShortcut(tr("Z"));
  zoomAct->setStatusTip(tr("Zoom in"));
  connect(zoomAct, SIGNAL(triggered()), this, SLOT(zoom()));

  unzoomAct =
    new QAction(QIcon(":STLSearcher/Images/magnifier_zoom_out.png"),
      tr("&Unzoom"), this);
  unzoomAct->setShortcut(tr("U"));
  unzoomAct->setStatusTip(tr("Unzoom"));
  connect(unzoomAct, SIGNAL(triggered()), this, SLOT(unzoom()));

  backViewAct =
    new QAction(QIcon(":STLSearcher/Images/back_view.png"),
      tr("&Back View"), this);
  backViewAct->setStatusTip(tr("Back view"));
  connect(backViewAct, SIGNAL(triggered()), this, SLOT(backView()));
  
  frontViewAct =
    new QAction(QIcon(":STLSearcher/Images/front_view.png"),
      tr("&Front View"), this);
  frontViewAct->setStatusTip(tr("Front view"));
  connect(frontViewAct, SIGNAL(triggered()), this, SLOT(frontView()));

  leftViewAct =
    new QAction(QIcon(":STLSearcher/Images/left_view.png"),
      tr("&Left View"), this);
  leftViewAct->setStatusTip(tr("Left view"));
  connect(leftViewAct, SIGNAL(triggered()), this, SLOT(leftView()));

  rightViewAct =
    new QAction(QIcon(":STLSearcher/Images/right_view.png"),
      tr("&Right View"), this);
  rightViewAct->setStatusTip(tr("Right view"));
  connect(rightViewAct, SIGNAL(triggered()), this, SLOT(rightView()));

  topViewAct =
    new QAction(QIcon(":STLSearcher/Images/top_view.png"),
      tr("&Top View"), this);
  topViewAct->setStatusTip(tr("Top view"));
  connect(topViewAct, SIGNAL(triggered()), this, SLOT(topView()));

  bottomViewAct =
    new QAction(QIcon(":STLSearcher/Images/bottom_view.png"),
      tr("&Bottom View"), this);
  bottomViewAct->setStatusTip(tr("Bottom view"));
  connect(bottomViewAct, SIGNAL(triggered()), this, SLOT(bottomView()));

  topFrontLeftViewAct =
    new QAction(QIcon(":STLSearcher/Images/top_front_left_view.png"),
      tr("&Top Front Left View"), this);
  topFrontLeftViewAct->setStatusTip(tr("Top Front Left view"));
  connect(topFrontLeftViewAct, SIGNAL(triggered()), this,
          SLOT(topFrontLeftView()));

  wireframeAct = new QAction(QIcon(":STLSearcher/Images/wireframe.png"),
                             tr("&Wireframe"), this);
  wireframeAct->setShortcut(tr("W"));
  wireframeAct->setStatusTip(tr("Wireframe view"));
  wireframeAct->setCheckable(true);
  connect(wireframeAct, SIGNAL(triggered()), this, SLOT(wireframe()));
  wireframeAct->setChecked(false);

  m_RadialViewAct = new QAction(QIcon(":STLSearcher/Images/radial.png"),
					tr("& minimal Radial View"), this);
  m_RadialViewAct->setShortcut(tr("R"));
  m_RadialViewAct->setStatusTip(tr("minimal Radial View"));
  m_RadialViewAct->setCheckable(true);  
  connect(m_RadialViewAct, SIGNAL(triggered()), this, SLOT(radialframe()));
  m_RadialViewAct->setChecked(false);

  exitAct = new QAction(tr("E&xit"), this);
  exitAct->setShortcut(tr("Ctrl+Q"));
  exitAct->setStatusTip(tr("Exit the application"));
  connect(exitAct, SIGNAL(triggered()), qApp, SLOT(closeAllWindows()));

  aboutAct = new QAction(tr("&About"), this);
  aboutAct->setStatusTip(tr("About STLSearcher"));
  connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));
}

void STLViewer::createMenus()
{
  fileMenu = menuBar()->addMenu(tr("&File"));
  fileMenu->addAction(m_newAct);
  fileMenu->addAction(openAct);
  fileMenu->addAction(saveAct);
  fileMenu->addAction(saveAsAct);
  fileMenu->addAction(saveAsSphereAct);
  fileMenu->addAction(saveAsNormalFreqAct);
  fileMenu->addAction(compareLastMeshesAct);
  fileMenu->addAction(saveImageAct);
  fileMenu->addSeparator();
  fileMenu->addAction(exitAct);

  viewMenu = menuBar()->addMenu(tr("&View"));
  viewMenu->addAction(rotateAct);
  viewMenu->addAction(panningAct);
  viewMenu->addAction(zoomAct);
  viewMenu->addAction(unzoomAct);
  viewMenu->addAction(wireframeAct);
  viewMenu->addAction(m_RadialViewAct);

  defaultViewsMenu = viewMenu->addMenu(tr("&Default Views"));
  defaultViewsMenu->addAction(backViewAct);  
  defaultViewsMenu->addAction(frontViewAct);
  defaultViewsMenu->addAction(leftViewAct);
  defaultViewsMenu->addAction(rightViewAct);
  defaultViewsMenu->addAction(topViewAct);
  defaultViewsMenu->addAction(bottomViewAct);
  defaultViewsMenu->addAction(topFrontLeftViewAct);

  viewMenu->addSeparator();

  windowMenu = menuBar()->addMenu(tr("&Window"));
  updateWindowMenu();
  connect(windowMenu, SIGNAL(aboutToShow()), this, SLOT(updateWindowMenu()));

  menuBar()->addSeparator();

  helpMenu = menuBar()->addMenu(tr("&Help"));
  helpMenu->addAction(aboutAct);
}

void STLViewer::createToolBars()
{
	m_fileToolBar = addToolBar(tr("File"));
	m_fileToolBar->addAction(m_newAct);

	m_fileToolBar->addAction(openAct);
	m_fileToolBar->addAction(saveAct);
	m_fileToolBar->setMaximumHeight (28);
	m_fileToolBar->setIconSize(QSize(24, 24));
	m_fileToolBar->setSizePolicy(QSizePolicy::MinimumExpanding,QSizePolicy::Maximum);


	m_viewToolBar = addToolBar(tr("View"));
	m_viewToolBar->addAction(rotateAct);
	m_viewToolBar->addAction(panningAct);
	m_viewToolBar->addAction(zoomAct);
	m_viewToolBar->addAction(unzoomAct);
	m_viewToolBar->addAction(wireframeAct);
	m_viewToolBar->addAction(m_RadialViewAct);
	m_viewToolBar->addAction(backViewAct);	
	
	m_viewToolBar->addAction(frontViewAct);
	m_viewToolBar->addAction(leftViewAct);
	m_viewToolBar->addAction(rightViewAct);
	m_viewToolBar->addAction(topViewAct);
	m_viewToolBar->addAction(bottomViewAct);
	m_viewToolBar->addAction(topFrontLeftViewAct);
	m_viewToolBar->setMaximumHeight (28);
}

void STLViewer::createStatusBar()
{
  statusBar()->showMessage(tr("Ready"));
}

void STLViewer::createDockWindows()
{
  // Create a DockWidget named "Informations"
  QDockWidget *dock = new QDockWidget(tr("Model Informations"), this);
  dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
  // Create one GroupBox for each type of data
  dimensionsGroupBox = new DimensionsGroupBox(this);
  meshInformationGroupBox = new MeshInformationGroupBox(this);
  propertiesGroupBox = new PropertiesGroupBox(this);
  // Create a layout inside a widget to display all GroupBoxes in one layout
  QWidget *wi = new QWidget;
  wi->setSizePolicy(QSizePolicy(QSizePolicy::MinimumExpanding,
                                QSizePolicy::Fixed));
  QVBoxLayout *layout = new QVBoxLayout;
  layout->addWidget(dimensionsGroupBox);
  layout->addWidget(meshInformationGroupBox);
  layout->addWidget(propertiesGroupBox);
  wi->setLayout(layout);
  // Embed the widget that contains all GroupBoxes into the DockWidget
  dock->setWidget(wi);
  // Add the DockWidget at the right side of the main layout
  addDockWidget(Qt::RightDockWidgetArea, dock);
  // Add a button in the view menu to show/hide the DockWidget
  viewMenu->addAction(dock->toggleViewAction());

  // Create a DockWidget named "View Informations"
  dock = new QDockWidget(tr("View Informations"), this);
  dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
  // Create one GroupBox to display the axis
  axisGroupBox = new AxisGroupBox(this);
  // Create a layout inside a widget to display all GroupBoxes in one layout
  wi = new QWidget;
  wi->setSizePolicy(QSizePolicy(QSizePolicy::MinimumExpanding,
                                QSizePolicy::Fixed));
  layout = new QVBoxLayout;
  layout->addWidget(axisGroupBox);
  wi->setLayout(layout);
  // Embed the widget that contains all GroupBoxes into the DockWidget
  dock->setWidget(wi);
  // Add the DockWidget at the right side of the main layout
  addDockWidget(Qt::RightDockWidgetArea, dock);
  // Add a button in the view menu to show/hide the DockWidget
  viewMenu->addAction(dock->toggleViewAction());
}

void STLViewer::readSettings()
{
  QSettings settings("Ayvah", "STLSearcher");
  curDir = settings.value("dir", QString()).toString();
  QPoint pos = settings.value("pos", QPoint(200, 200)).toPoint();
  QSize size = settings.value("size", QSize(400, 400)).toSize();
  resize(size);
  move(pos);
}

void STLViewer::writeSettings() {
  QSettings settings("Ayvah", "STLSearcher");
  settings.setValue("dir", curDir);
  settings.setValue("pos", pos());
  settings.setValue("size", size());
}

GLMdiChild *STLViewer::activeGLMdiChild()
{
  if (QMdiSubWindow *activeSubWindow = mdiArea->activeSubWindow())
    return qobject_cast<GLMdiChild *>(activeSubWindow->widget());
  return 0;
}

QMdiSubWindow *STLViewer::findGLMdiChild(const QString &fileName)
{
  QString canonicalFilePath = QFileInfo(fileName).canonicalFilePath();

  foreach (QMdiSubWindow *window, mdiArea->subWindowList()) {
    GLMdiChild *glMdiChild = qobject_cast<GLMdiChild *>(window->widget());
    if (glMdiChild->currentFile() == canonicalFilePath)
      return window;
  }
  return 0;
}
