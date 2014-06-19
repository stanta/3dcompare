// Copyright (c) 2014 Andranik Abrahamyan

#ifndef AXISGLWIDGET_H
#define AXISGLWIDGET_H

#include <QtGui/QGroupBox>
#include <QtOpenGL/QGLWidget>

#include "stlfile.h"

class AxisGLWidget;

/*! \brief  The AxisGLWidget window shows axis rotations.
*
*/
class AxisGLWidget : public QGLWidget  {

  Q_OBJECT

 public:
  AxisGLWidget(QWidget *parent = 0);
  ~AxisGLWidget();
  void reset(); //!< reset the axis window
  void rotateAxis(); //!< rotate the axis window

  QSize minimumSizeHint() const; //!< set minimum size Hint for the axis window
  QSize sizeHint() const; //!< set size Hint for the axis window

 public slots:
  void setXRotation(const int angle); //!< set the x Rotation angle
  void setYRotation(const int angle); //!< set the y Rotation angle
  void setZRotation(const int angle); //!< set the z Rotation angle

 signals:
  void xRotationChanged(int angle) const; //!< the x Rotation Changed signal
  void yRotationChanged(int angle) const; //!< the y Rotation Changed signal
  void zRotationChanged(int angle) const; //!< the z Rotation Changed signal

 protected:
  void initializeGL(); //!< initialize the axises window
  void paintGL(); //!< paint the axises window
  void resizeGL(int width, int height); //!< resize the axises window

 private:
  void drawAxes();  //!< draw the axises 

  int xRot; //!< axis rotation x position
  int yRot; //!< axis rotation y position
  int zRot; //!< axis rotation y position
  QColor black; //!< black color  for drawing
  QColor purple; //!< purple color  for drawing
};

#endif  // AXISGLWIDGET_H
