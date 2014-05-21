// Copyright (c) 2014 Andranik Abrahamyan

#ifndef AXISGLWIDGET_H
#define AXISGLWIDGET_H

#include <QtGui/QGroupBox>
#include <QtOpenGL/QGLWidget>

#include "stlfile.h"

class AxisGLWidget;

class AxisGLWidget : public QGLWidget  {

  Q_OBJECT

 public:
  AxisGLWidget(QWidget *parent = 0);
  ~AxisGLWidget();
  void reset();
  void rotateAxis();
  QSize minimumSizeHint() const;
  QSize sizeHint() const;

 public slots:
  void setXRotation(const int angle);
  void setYRotation(const int angle);
  void setZRotation(const int angle);

 signals:
  void xRotationChanged(int angle) const;
  void yRotationChanged(int angle) const;
  void zRotationChanged(int angle) const;

 protected:
  void initializeGL();
  void paintGL();
  void resizeGL(int width, int height);

 private:
  void drawAxes();
  int xRot, yRot, zRot;
  QColor black, purple;
};

#endif  // AXISGLWIDGET_H
