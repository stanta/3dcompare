// Copyright (c) 2014 Andranik Abrahamyan

#ifndef GLMDICHILD_H
#define GLMDICHILD_H

#include "glwidget.h"
#include "stlfile.h"
#include "stlsearcher.h"

class GLMdiChild : public GLWidget
{
  Q_OBJECT
public:
	GLMdiChild(QWidget *parent = 0);
	~GLMdiChild();
	void newFile();
	bool loadFile(const QString &fileName);
	bool save();
	bool saveAs();
	bool saveFile(const QString &fileName);
	bool saveImage();
	QString userFriendlyCurrentFile();
	QString currentFile() { return m_curFile; }
	Stl_Stats getStats() const { return m_stlFile->getStats(); }
	StlSearcher::UnitStats getSearcherStats() const
	{ return m_stlSearcher->getStats(); }

signals:
	void	mouseButtonPressed(Qt::MouseButtons button);
	void	mouseButtonReleased(Qt::MouseButtons button);

protected:
	void	closeEvent(QCloseEvent *event);
	void	mousePressEvent(QMouseEvent *event);
	void	mouseReleaseEvent(QMouseEvent *event);

private:
	bool	maybeSave();
	void	setCurrentFile(const QString &fileName);
	QString strippedName(const QString &fullFileName);

public:
	bool	m_isUntitled;
private:
	StlFile *		m_stlFile;
	QString			m_curFile;
	StlSearcher*	m_stlSearcher;
};

#endif  // GLMDICHILD_H
