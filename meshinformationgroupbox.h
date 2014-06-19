// Copyright (c) 2014 Andranik Abrahamyan

#ifndef MESHINFORMATIONGROUPBOX_H
#define MESHINFORMATIONGROUPBOX_H

#include <QtGui/QGroupBox>

#include "stlfile.h"

class QLabel;
/*! \brief  The MeshInformationGroupBox window shows number of facets and number of points volumes .
*
*/

class MeshInformationGroupBox : public QGroupBox 
{
  Q_OBJECT
public:
	MeshInformationGroupBox(QWidget *parent = 0);
	~MeshInformationGroupBox();
	/// reset values
	void reset();
	/// sets values
	void setValues(const Stl_Stats stats);

private:
	QLabel *numFacets; //!< number of facets 
	QLabel *numPoints; //!< number of points
};

#endif  // MESHINFORMATIONGROUPBOX_H
