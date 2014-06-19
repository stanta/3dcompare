// Copyright (c) 2014 Andranik Abrahamyan

#ifndef DIMENSIONSGROUPBOX_H
#define DIMENSIONSGROUPBOX_H

#include <QtGui/QGroupBox>
#include "stlsphere.h"

class QLabel;
/*! \brief  The DimensionsGroupBox window shows minimal, maximal and delta values of the mesh.
*
*/

class DimensionsGroupBox : public QGroupBox
{
	Q_OBJECT
public:
	DimensionsGroupBox(QWidget *parent = 0);
	~DimensionsGroupBox();
	/// reset values
	void reset();
	/// sets values
	void setValues(const StlSphere::UnitStats  stats);

private:
	QLabel *xMax; //!< maximal x position of the mesh
	QLabel *xMin; //!< minimal x position of the mesh
	QLabel *xDelta; //!< x delta value from maximal to minimal
	QLabel *yMax; //!< maximal y position of the mesh
	QLabel *yMin; //!< minimal y position of the mesh
	QLabel *yDelta; //!< y delta value from maximal to minimal
	QLabel *zMax; //!< maximal z position of the mesh
	QLabel *zMin; //!< minimal z position of the mesh
	QLabel *zDelta; //!< z delta value from maximal to minimal
};
#endif  // DIMENSIONSGROUPBOX_H
