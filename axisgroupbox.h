// Copyright (c) 2014 Andranik Abrahamyan

#ifndef AXISGROUPBOX_H
#define AXISGROUPBOX_H

#include <QtGui/QGroupBox>

class QLabel;
class AxisGLWidget;
/*! \brief  The AxisGroupBox window shows axis OpenGL Widget and  rotation positions.
*
*/

class AxisGroupBox : public QGroupBox {

  Q_OBJECT

 public:
  AxisGroupBox(QWidget *parent = 0);
  ~AxisGroupBox();
  /// reset values
  void reset();

 public slots:
  void setXRotation(const int angle); //!< set axis rotation x position
  void setYRotation(const int angle); //!< set axis rotation y position
  void setZRotation(const int angle); //!< set axis rotation z position

 private:
  AxisGLWidget *axisGLWidget; //!< axis OpenGL Widget
  QLabel *xRot; //!< axis rotation x position
  QLabel *yRot; //!< axis rotation y position
  QLabel *zRot; //!< axis rotation z position
};

#endif  // DIMENSIONSGROUPBOX_H
