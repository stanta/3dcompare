// Copyright (c) 2014 Andranik Abrahamyan

#ifndef PROPERTIESGROUPBOX_H
#define PROPERTIESGROUPBOX_H

#include <QtGui/QGroupBox>
#include "stlsphere.h"

class QLabel;

class PropertiesGroupBox : public QGroupBox
{
  Q_OBJECT
public:
	PropertiesGroupBox(QWidget *parent = 0);
	~PropertiesGroupBox();
	void reset();
	void setValues(const StlSphere::UnitStats stats);

	private:
	QLabel *volume, *surface;//, *diameter;
	QLabel *box_length, *box_width, *box_height;
	QLabel *average_radial , *min_radial;
};

#endif  // PROPERTIESGROUPBOX_H
