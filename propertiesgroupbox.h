// Copyright (c) 2014 Andranik Abrahamyan

#ifndef PROPERTIESGROUPBOX_H
#define PROPERTIESGROUPBOX_H

#include <QtGui/QGroupBox>
#include "stlsphere.h"

class QLabel;
/*! \brief  The PropertiesGroupBox window shows volume,surface etc volumes .
*
*	The window has unit mesh mass,surface, bounding box , average radial  and minimal radial values
*/

class PropertiesGroupBox : public QGroupBox
{
  Q_OBJECT
public:
	PropertiesGroupBox(QWidget *parent = 0);
	~PropertiesGroupBox();
	/// reset values
	void reset();
	/// sets values
	void setValues(const StlSphere::UnitStats stats);

	private:
	QLabel	*volume; //!< unit mesh volume(or mass)
	QLabel	*surface; //!< unit mesh surface
	QLabel	*box_length; //!< unit mesh bounding box length
	QLabel	*box_width ; //!< unit mesh bounding box width
	QLabel	*box_height; //!< unit mesh bounding box height
	QLabel	*average_radial; //!< unit mesh average radial value
	QLabel	*min_radial; //!< unit mesh minimal radial value
};

#endif  // PROPERTIESGROUPBOX_H
