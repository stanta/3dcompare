// Copyright (c) 2014 Andranik Abrahamyan

#ifndef MESHINFORMATIONGROUPBOX_H
#define MESHINFORMATIONGROUPBOX_H

#include <QtGui/QGroupBox>

#include "stlfile.h"

class QLabel;

class MeshInformationGroupBox : public QGroupBox {

  Q_OBJECT

 public:
  MeshInformationGroupBox(QWidget *parent = 0);
  ~MeshInformationGroupBox();
  void reset();
  void setValues(const StlFile::Stats stats);

 private:
  QLabel *numFacets, *numPoints;
};

#endif  // MESHINFORMATIONGROUPBOX_H
