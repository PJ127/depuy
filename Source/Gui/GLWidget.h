#pragma once

#include <QOpenGLWidget>

namespace dm {
class Mesh;
}

namespace gui {

class GLWidget : public QOpenGLWidget {
    Q_OBJECT

public:
    GLWidget(QWidget *parent = 0);
    ~GLWidget();

public:
    void updateObject();

public slots:
    void setRotation(int xAngle, int yAngle);
    void setTranslation(double x, double y);

protected:
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int width, int height) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;

private:
    int xRot;
    int yRot;
    int zRot;

    double xTranslation;
    double yTranslation;
    double zTranslation;

    double scaling;

    QPoint clickedPos;
    QPoint lastPos;

    GLuint object;

private:
    void drawAxes(const int axisLength);
    void drawWireframe(const dm::Mesh *mesh);
    void drawMesh(const dm::Mesh *mesh);
    void drawBox(const double *bounds);
    GLuint makeObject();

    void enableLight();
    void disableLight();
};

}; // namespace gui
