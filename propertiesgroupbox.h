// Copyright (c) 2014 Andranik Abrahamyan

#ifndef PROPERTIESGROUPBOX_H
#define PROPERTIESGROUPBOX_H

#include <QtGui/QGroupBox>
#include "stlsearcher.h"

class QLabel;

class PropertiesGroupBox : public QGroupBox
{
  Q_OBJECT
public:
	PropertiesGroupBox(QWidget *parent = 0);
	~PropertiesGroupBox();
	void reset();
	void setValues(const StlSearcher::UnitStats stats);

	private:
	QLabel *volume, *surface;//, *diameter;
	QLabel *box_length, *box_width, *box_height;
};

#endif  // PROPERTIESGROUPBOX_H
