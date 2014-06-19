// Copyright (c) 2014 Andranik Abrahamyan

#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QtOpenGL/QGLWidget>

class StlFile;
class MdiChild;
/*! \brief  The GLWidget class is a widget for rendering OpenGL graphics..
*
*	QGLWidget provides functionality for displaying OpenGL graphics integrated into a Qt application.
*/

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
	QSize minimumSizeHint() const; //!< set minimum size hint for the window
	QSize sizeHint() const; //!< set size hint for the window
	void makeObjectFromStlFile(StlFile*); //!< create the mesh with window
	void deleteObject(); //!< delete the mesh with window
	void setDefaultView(); //!< set default view
	void zoom(); //!< zoom the mesh
	void unzoom(); //!< unzoom the mesh
	void setBackView(); //!< The mesh back view  action  
	void setFrontView(); //!< The mesh front view  action 
	void setLeftView(); //!< The mesh left view  action 
	void setRightView(); //!< The mesh right view  action 
	void setTopView(); //!< The mesh top view  action
	void setBottomView(); //!< The mesh bottom view  action
	void setTopFrontLeftView(); //!< The mesh top front left view  action
	bool isWireframeModeActivated() const { return wireframeMode; }; //!< is wire frame mode activated
	bool isRadialModeActivated() const { return m_showRadial; }; //!< is radial frame mode activated
	int getXRot() const { return xRot; }; //!< get x rotation value
	int getYRot() const { return yRot; }; //!< get y rotation value
	int getZRot() const { return zRot; }; //!< get z rotation value

public slots:
	void setXRotation(int angle); //!< set x rotation value
	void setYRotation(int angle); //!< set y rotation value
	void setZRotation(int angle); //!< set z rotation value
	void setXTranslation(const float distance); //!< set x translation value
	void setYTranslation(const float distance); //!< set y translation value
	void setZoom(const float zoom); //!< set zoom factor
	void setLeftMouseButtonMode(const GLWidget::LeftMouseButtonMode); //!< set left mouse button mode
	void setWireframeMode(const bool state); //!< set wire frame mode
	void setRadialMode(const bool state); //!< set radial frame mode

signals:
	void xRotationChanged(const int angle) const; //!< x rotation changed
	void yRotationChanged(const int angle) const; //!< y rotation changed
	void zRotationChanged(const int angle) const; //!< z rotation changed
	void xTranslationChanged(const float distance) const; //!< x translation changed
	void yTranslationChanged(const float distance) const; //!< y translation changed
	void zoomChanged(const float zoom); //!< initialize the window

protected:
	void initializeGL(); //!< initialize the window
	void paintGL(); //!< paint the window
	void resizeGL(int width, int height); //!< resize the window
	void mousePressEvent(QMouseEvent *event); //!< mouse press event
	void mouseReleaseEvent(QMouseEvent *event); //!< mouse release event
	void mouseMoveEvent(QMouseEvent *event); //!< mouse move event
	void wheelEvent(QWheelEvent *event); //!< mouse wheel event

private:
	void	triangle(GLdouble, GLdouble, GLdouble, GLdouble, GLdouble, GLdouble,
                GLdouble, GLdouble, GLdouble); //!< create the triangle
	void	normalizeAngle(int *angle); //!< normalize the angle
	void	drawAxes(); //!< draw axes
	void	updateCursor(); //!< update Cursor position

	//GLfloat panMatrix[16];
	int		width;  //!< the window width
	int		height; //!< the window height
	GLuint	m_object; //!< the OpenGL GLuint object

	LeftMouseButtonMode leftMouseButtonMode; //!< left mouse button mode
	int		xRot; //!< window rotation x position
	int		yRot; //!< window rotation y position
	int		zRot; //!< window rotation x position
	int		xPos; //!< mesh x position in the window
	int		yPos; //!< mesh y position in the window
	int		zPos; //!< mesh z position in the window

	float	xTrans; //!< x Translation value
	float	yTrans; //!< y Translation value
	float	zTrans; //!< z Translation value
	float	zoomFactor; //!< zoom factor
	float	zoomInc;	//!< zoom increment value
	float	defaultZoomFactor; //!< default zoom factor
	QPoint	lastPos; //!< last position of the mouse
	QColor	grey; //!< grey color  for drawing
	QColor	black; //!< black color  for drawing
	QColor	purple; //!< purple color  for drawing
	bool	wireframeMode; //!< showing wire frame
	bool	m_showRadial; //!< showing radial frame
};

#endif  // GLWIDGET_H
