// Copyright (c) 2014 Andranik Abrahamyan

#ifndef GLMDICHILD_H
#define GLMDICHILD_H

#include "glwidget.h"
#include "stlfile.h"
#include "stlsphere.h"

/*! \brief  The GLMdiChild class provides a OpenGL MDI child window implementation.
*
*	GLMdiChild has referance to the associated StlFile object and current stl file path 
*/

class GLMdiChild : public GLWidget
{
  Q_OBJECT
public:
	GLMdiChild(QWidget *parent = 0);
	~GLMdiChild();
	void	newFile(); //!< Creates a new file
	bool	loadFile(const QString &fileName); //!< Load the stl file 
	bool	save(); //!< Write the current mesh into a current stl file 
	bool	saveAs(); //!< Write the current mesh into a binary or ASCII stl file 
	bool	saveAsSphere(); //!< Write the current object into a sphere file
	bool	saveAsNormalFrequency(); //!< File save as normals frequency action
	bool	saveFile(const QString &fileName); //!< Write the current object into a file
	bool	saveImage(); //!< get snapshot from the mesh
	QString userFriendlyCurrentFile();
	QString currentFile() { return m_curFile; } //!< return current file path
	Stl_Stats getStats() const { return m_stlFile->getStats(); } //!< return StlFile states
	StlSphere::UnitStats getSearcherStats() const 
	{ return m_stlFile->m_stlSphere->getStats(); } //!< return StlSphere states

signals:
	void	mouseButtonPressed(Qt::MouseButtons button); //!< mouse button pressed signal
	void	mouseButtonReleased(Qt::MouseButtons button); //!< mouse button released signal

protected:
	void	closeEvent(QCloseEvent *event); //!< close Event signal
	void	mousePressEvent(QMouseEvent *event); //!< mouse press Event signal
	void	mouseReleaseEvent(QMouseEvent *event); //!< mouse release Event signal

private:
	bool	maybeSave(); //!< may be save the stl file
	void	setCurrentFile(const QString &fileName); //!< set current file path 
	QString strippedName(const QString &fullFileName); //!< get stripped current file name

public:
	bool	m_isUntitled; //!< is window untitled
//private:
	StlFile *		m_stlFile; //!< referance to the associated StlFile object
	QString			m_curFile; //!< current file path 
};

#endif  // GLMDICHILD_H
