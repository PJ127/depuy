#pragma once

//Qt
#include <QOpenGLWidget>

namespace dm
{
     class Mesh;
}

namespace gui
{

class GLWidget : public QOpenGLWidget
{
     Q_OBJECT

 public:
     GLWidget(QWidget* parent = 0);
     ~GLWidget();

 public:
     void updateObject();

 public slots:
     void setXRotation(int angle);
     void setYRotation(int angle);

     void setXTranslation(double translation);
     void setYTranslation(double translation);

 signals:
     void xRotationChanged(int angle);
     void yRotationChanged(int angle);

     void xTranlationChanged(double translation);
     void yTranlationChanged(double translation);

 protected:
     void initializeGL();
     void paintGL();
     void resizeGL(int width, int height);
     void mousePressEvent(QMouseEvent* event);
     void mouseMoveEvent(QMouseEvent* event);
     void wheelEvent (QWheelEvent* event);

 private:
     int xRot;
     int yRot;
     int zRot;

     double xTranslation;
     double yTranslation;
     double zTranslation;

     double m_scaling;

     QPoint clickedPos;
     QPoint lastPos;

     GLuint object;

 private:
     void drawCoorSyst(const int p_size);
     void drawWireframe(const dm::Mesh* p_mesh);
     void drawMesh(const dm::Mesh* p_mesh);
     void drawBox(const double* bounds);
     GLuint makeObject();

     void enableLight();
     void disableLight();

};

};
