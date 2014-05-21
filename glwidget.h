// Copyright (c) 2014 Andranik Abrahamyan

#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QtOpenGL/QGLWidget>

class StlFile;
class MdiChild;

class GLWidget : public QGLWidget 
{
Q_OBJECT

public:
  enum LeftMouseButtonMode {
    INACTIVE,
    ROTATE,
    PANNING
  };
  GLWidget(QWidget *parent = 0);
  ~GLWidget();
  QSize minimumSizeHint() const;
  QSize sizeHint() const;
  void makeObjectFromStlFile(StlFile*);
  void deleteObject();
  void setDefaultView();
  void zoom();
  void unzoom();
  void setBackView();
  void setFrontView();
  void setLeftView();
  void setRightView();
  void setTopView();
  void setBottomView();
  void setTopFrontLeftView();
  bool isWireframeModeActivated() const { return wireframeMode; };
  bool isRadialModeActivated() const { return m_showRadial; };
  int getXRot() const { return xRot; };
  int getYRot() const { return yRot; };
  int getZRot() const { return zRot; };

public slots:
  void setXRotation(int angle);
  void setYRotation(int angle);
  void setZRotation(int angle);
  void setXTranslation(const float distance);
  void setYTranslation(const float distance);
  void setZoom(const float zoom);
  void setLeftMouseButtonMode(const GLWidget::LeftMouseButtonMode);
  void setWireframeMode(const bool state);
  void	setRadialMode(const bool state);

signals:
  void xRotationChanged(const int angle) const;
  void yRotationChanged(const int angle) const;
  void zRotationChanged(const int angle) const;
  void xTranslationChanged(const float distance) const;
  void yTranslationChanged(const float distance) const;
  void zoomChanged(const float zoom);

protected:
  void initializeGL();
  void paintGL();
  void resizeGL(int width, int height);
  void mousePressEvent(QMouseEvent *event);
  void mouseReleaseEvent(QMouseEvent *event);
  void mouseMoveEvent(QMouseEvent *event);
  void wheelEvent(QWheelEvent *event);

private:
  void	triangle(GLdouble, GLdouble, GLdouble, GLdouble, GLdouble, GLdouble,
                GLdouble, GLdouble, GLdouble);
  void	normalizeAngle(int *angle);
  void	drawAxes();
  void	updateCursor();

  //GLfloat panMatrix[16];
  int		width, height;
  GLuint	m_object;
  bool		wireframeMode;
  LeftMouseButtonMode leftMouseButtonMode;
  int		xRot, yRot, zRot;
  int		xPos, yPos, zPos;
  float		xTrans, yTrans, zTrans;
  float		zoomFactor;
  float		zoomInc;
  float		defaultZoomFactor;
  QPoint	lastPos;
  QColor	grey, black, purple;
  bool		m_showRadial;
};

#endif  // GLWIDGET_H
