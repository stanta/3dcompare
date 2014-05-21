// Copyright (c) 2014 Andranik Abrahamyan

#ifndef DIMENSIONSGROUPBOX_H
#define DIMENSIONSGROUPBOX_H

#include <QtGui/QGroupBox>

#include "stlfile.h"

class QLabel;

class DimensionsGroupBox : public QGroupBox {

  Q_OBJECT

 public:
  DimensionsGroupBox(QWidget *parent = 0);
  ~DimensionsGroupBox();
  void reset();
  void setValues(const StlFile::Stats stats);

 private:
  QLabel *xMax, *xMin, *xDelta;
  QLabel *yMax, *yMin, *yDelta;
  QLabel *zMax, *zMin, *zDelta;
};

#endif  // DIMENSIONSGROUPBOX_H
