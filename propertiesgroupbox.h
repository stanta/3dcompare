// Copyright (c) 2014 Andranik Abrahamyan

#ifndef PROPERTIESGROUPBOX_H
#define PROPERTIESGROUPBOX_H

#include <QtGui/QGroupBox>

#include "stlfile.h"

class QLabel;

class PropertiesGroupBox : public QGroupBox {

  Q_OBJECT

 public:
  PropertiesGroupBox(QWidget *parent = 0);
  ~PropertiesGroupBox();
  void reset();
  void setValues(const StlFile::Stats stats);

 private:
  QLabel *volume, *surface;
};

#endif  // PROPERTIESGROUPBOX_H
