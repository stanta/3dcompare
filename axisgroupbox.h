// Copyright (c) 2014 Andranik Abrahamyan

#ifndef AXISGROUPBOX_H
#define AXISGROUPBOX_H

#include <QtGui/QGroupBox>

class QLabel;
class AxisGLWidget;

class AxisGroupBox : public QGroupBox {

  Q_OBJECT

 public:
  AxisGroupBox(QWidget *parent = 0);
  ~AxisGroupBox();
  void reset();

 public slots:
  void setXRotation(const int angle);
  void setYRotation(const int angle);
  void setZRotation(const int angle);

 private:
  AxisGLWidget *axisGLWidget;
  QLabel *xRot, *yRot, *zRot;
};

#endif  // DIMENSIONSGROUPBOX_H
