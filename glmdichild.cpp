// Copyright (c) 2014 Andranik Abrahamyan

#include <QtGui/QtGui>
#include <exception>
#include "glmdichild.h"

GLMdiChild::GLMdiChild(QWidget *parent) : GLWidget(parent)
{
	m_stlSphere = new StlSphere;
	m_stlFile = new StlFile(m_stlSphere);

	setAttribute(Qt::WA_DeleteOnClose);
	m_isUntitled = true;	
}

GLMdiChild::~GLMdiChild()
{
  delete	m_stlFile;
  delete	m_stlSphere;
}

void GLMdiChild::newFile()
{
	static int sequenceNumber = 1;
	m_isUntitled = true;
	m_curFile = tr("untitled%1.stl").arg(sequenceNumber++);
	setWindowTitle(m_curFile);
}

bool GLMdiChild::loadFile(const QString &fileName)
{
	try {
		QApplication::setOverrideCursor(Qt::WaitCursor);
		// Open the file and make an object from its content
		std::string utf8_fileName = fileName.toUtf8().constData();
		m_stlFile->open(utf8_fileName);
		makeObjectFromStlFile(m_stlFile);
		updateGL();
		setCurrentFile(fileName);
		QApplication::restoreOverrideCursor();
		return true;
	} catch (::std::bad_alloc) {
		QApplication::restoreOverrideCursor();
		QMessageBox msgBox;
		msgBox.setText("Problem allocating memory.");
		msgBox.exec();
		return false;
	} catch (StlFile::wrong_header_size) {
		QApplication::restoreOverrideCursor();
		QMessageBox msgBox;
		msgBox.setText("The file " + fileName + " has a wrong size.");
		msgBox.exec();
		return false;
	} catch (StlFile::error_opening_file) { // ::std::ios_base::failure
		QApplication::restoreOverrideCursor();
		QMessageBox msgBox;
		msgBox.setText("The file " + fileName + " could not be opened.");
		msgBox.exec();
		return false;
	} catch (...) {
		QApplication::restoreOverrideCursor();
		QMessageBox msgBox;
		msgBox.setText("Error unknown.");
		msgBox.exec();
		return false;
	}
}

bool GLMdiChild::save()
{
	if (m_isUntitled) 
		return saveAs();
	else
		return saveFile(m_curFile);
}

bool GLMdiChild::saveAsSphere()
{
	try {
		QApplication::setOverrideCursor(Qt::WaitCursor);
		// Write the current object into a file
		std::string utf8_fileName = m_curFile.toUtf8().constData();
		utf8_fileName.erase (utf8_fileName.end() -3, utf8_fileName.end()); 
		utf8_fileName.append ("sph");
		m_stlFile->writeAsSphere(utf8_fileName);
		QApplication::restoreOverrideCursor();
		//setCurrentFile(fileName);
		return true;
	} catch (StlFile::error_opening_file) {  // ::std::ios_base::failure
		QApplication::restoreOverrideCursor();
		QMessageBox msgBox;
		msgBox.setText("Unable to write as sphere in  " + m_curFile + ".");
		msgBox.exec();
		return false;
	} catch (...) {
		QApplication::restoreOverrideCursor();
		QMessageBox msgBox;
		msgBox.setText("Error unknown.");
		msgBox.exec();
		return false;
	}	
}

bool GLMdiChild::saveAs()
{
	if (!m_isUntitled) {
		QString filterBin = tr("STL Files, binary (*.stl)");
		QString filterAscii = tr("STL Files, ASCII (*.stl)");
		QString filterAll = tr("All files (*.*)");
		QString filterSel;
		// Set the current file type as default
		if (m_stlFile->getStats().type == ASCII)
			filterSel = filterAscii;
		else
			filterSel = filterBin;
		QString fileName = QFileDialog::getSaveFileName(this, tr("Save As"), m_curFile,
			filterBin + ";;" + filterAscii + ";;" + filterAll, &filterSel);
		if (fileName.isEmpty())
			return false;
		// Change the current file type to the one chosen by the user
		if (filterSel == filterBin)
			m_stlFile->setFormat(BINARY);
		else if (filterSel == filterAscii)
			m_stlFile->setFormat(ASCII);
		// Save the file
		return saveFile(fileName);
	}
	return false;
}

bool GLMdiChild::saveFile(const QString &fileName) {
	try {
		QApplication::setOverrideCursor(Qt::WaitCursor);
		// Write the current object into a file
		std::string utf8_fileName = fileName.toUtf8().constData();
		m_stlFile->write(utf8_fileName);
		QApplication::restoreOverrideCursor();
		setCurrentFile(fileName);
		return true;
	} catch (StlFile::error_opening_file) {  // ::std::ios_base::failure
		QApplication::restoreOverrideCursor();
		QMessageBox msgBox;
		msgBox.setText("Unable to write in " + fileName + ".");
		msgBox.exec();
		return false;
	} catch (...) {
		QApplication::restoreOverrideCursor();
		QMessageBox msgBox;
		msgBox.setText("Error unknown.");
		msgBox.exec();
		return false;
	}
}

bool GLMdiChild::saveImage() {
	QFileInfo fi(m_curFile);
	QString imFile = fi.path() + "/" + fi.completeBaseName() + ".png";
	QString filterPng = tr("PNG Files (*.png)");
	QString filterBmp = tr("BMP Files (*.bmp)");
	QString filterAll = tr("All files (*.*)");
	QString filterSel = filterPng;  // Default image type is PNG
	QString fileName = QFileDialog::getSaveFileName(this, tr("Save Image"), imFile,
			filterPng + ";;" + filterBmp + ";;" + filterAll, &filterSel);
	if (fileName.isEmpty())
		return false;
	QString format = "png";  // Default image type is PNG if none was specified
	if (filterSel == filterBmp || QFileInfo(fileName).suffix() == ".bmp")
		format = "bmp";  
	QApplication::setOverrideCursor(Qt::WaitCursor);
	QImage screenshot = this->grabFrameBuffer();
	screenshot.save(fileName, format.toAscii());
	QApplication::restoreOverrideCursor();
	return true;
}

QString GLMdiChild::userFriendlyCurrentFile()
{
	return strippedName(m_curFile);
}

void GLMdiChild::closeEvent(QCloseEvent *event) {
	m_stlFile->close();
	m_stlSphere->close();
	if (maybeSave())
		event->accept();
	else 
		event->ignore();
}

void GLMdiChild::mousePressEvent(QMouseEvent *event)
{
	// Emit a signal if a mouse button is pressed on this widget
	emit mouseButtonPressed(event->buttons());
	GLWidget::mousePressEvent(event);
}

void GLMdiChild::mouseReleaseEvent(QMouseEvent *event)
{
	// Emit a signal if a mouse button is released on this widget
	emit mouseButtonReleased(event->button());
	GLWidget::mouseReleaseEvent(event);
}

bool GLMdiChild::maybeSave() 
{
	return true;
}

void GLMdiChild::setCurrentFile(const QString &fileName)
{
	m_curFile = QFileInfo(fileName).canonicalFilePath();
	m_isUntitled = false;
	setWindowModified(false);
	setWindowTitle(userFriendlyCurrentFile() + "[*]");
}

QString GLMdiChild::strippedName(const QString &fullFileName) 
{
	return QFileInfo(fullFileName).fileName();
}