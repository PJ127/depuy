// QT
#include <QOpenGLWidget>
#include <QtGui>

// Standard
#include <math.h>

// Project
#include "../Data/Data.h"
#include "../Data/Mesh.h"

#include "GLWidget.h"

//------------------------------------------------------------------------------
gui::GLWidget::GLWidget(QWidget *parent)
    : QOpenGLWidget(parent)
//------------------------------------------------------------------------------
{
    QSurfaceFormat format;
    format.setSamples(4);
    setFormat(format);

    object = 0;

    xRot = 0;
    yRot = 0;
    zRot = 0;

    m_scaling = 1.0;

    xTranslation = 0;
    yTranslation = 0;
    zTranslation = -10;
}

//------------------------------------------------------------------------------
gui::GLWidget::~GLWidget()
//------------------------------------------------------------------------------
{}

//------------------------------------------------------------------------------
static void qNormalizeAngle(int &angle)
//------------------------------------------------------------------------------
{
    while (angle < 0)
        angle += 360 * 16;
    while (angle > 360 * 16)
        angle -= 360 * 16;
}

//------------------------------------------------------------------------------
void gui::GLWidget::setXTranslation(double translation)
//------------------------------------------------------------------------------
{
    if (translation != xTranslation) {
        xTranslation = translation;
        emit xTranlationChanged(xTranslation);
        update();
    }
}

//------------------------------------------------------------------------------
void gui::GLWidget::setYTranslation(double translation)
//------------------------------------------------------------------------------
{
    if (translation != yTranslation) {
        yTranslation = translation;
        emit yTranlationChanged(yTranslation);
        update();
    }
}

//------------------------------------------------------------------------------
void gui::GLWidget::setXRotation(int angle)
//------------------------------------------------------------------------------
{
    qNormalizeAngle(angle);
    if (angle != xRot) {
        xRot = angle;
        emit xRotationChanged(angle);
        update();
    }
}

//------------------------------------------------------------------------------
void gui::GLWidget::setYRotation(int angle)
//------------------------------------------------------------------------------
{
    qNormalizeAngle(angle);
    if (angle != yRot) {
        yRot = angle;
        emit yRotationChanged(angle);
        update();
    }
}

//------------------------------------------------------------------------------
void gui::GLWidget::initializeGL()
//------------------------------------------------------------------------------
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    if (object != 0) {
        glDeleteLists(object, 1);
        object = 0;
    }

    object = makeObject();

    glEnable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    glEnable(GL_NORMALIZE);

    // glShadeModel(GL_SMOOTH);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
}

//------------------------------------------------------------------------------
void gui::GLWidget::paintGL()
//------------------------------------------------------------------------------
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();

    glTranslated(xTranslation, -yTranslation, zTranslation);
    glScalef(m_scaling, m_scaling, m_scaling);
    glRotated(xRot / 16.0, 1.0, 0.0, 0.0);
    glRotated(yRot / 16.0, 0.0, 1.0, 0.0);
    glRotated(zRot / 16.0, 0.0, 0.0, 1.0);

    if (object != 0)
        glCallList(object);
}

//------------------------------------------------------------------------------
GLuint gui::GLWidget::makeObject()
//------------------------------------------------------------------------------
{
    GLuint list = glGenLists(1);
    glNewList(list, GL_COMPILE);

    // Draw the coordinate system
    disableLight();
    drawCoorSyst(100);

    // Draw the wireframe
    disableLight();
    glColor3f(0.7f, 0.7f, 0.7f);
    drawWireframe(dm::Data::instance()->mesh());

    // Draw the triangle mesh
    enableLight();
    glColor3f(1.0f, 0.97f, 0.87f);
    drawMesh(dm::Data::instance()->mesh());

    glEndList();
    return list;
}

//------------------------------------------------------------------------------
void gui::GLWidget::updateObject()
//------------------------------------------------------------------------------
{
    makeCurrent();
    if (object != 0)
        glDeleteLists(object, 1);
    object = 0;
    object = makeObject();
}

//------------------------------------------------------------------------------
void gui::GLWidget::resizeGL(int width, int height)
//------------------------------------------------------------------------------
{
    static const float _zNear = -20000.f;
    static const float _zFar = +20000.f;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glViewport(0, 0, width, height);

    glOrtho(-width / 2., width / 2., -height / 2, height / 2, _zNear, _zFar);

    glMatrixMode(GL_MODELVIEW);
}

//------------------------------------------------------------------------------
void gui::GLWidget::mousePressEvent(QMouseEvent *event)
//------------------------------------------------------------------------------
{
    lastPos = event->pos();
}

//------------------------------------------------------------------------------
void gui::GLWidget::mouseMoveEvent(QMouseEvent *event)
//------------------------------------------------------------------------------
{
    int dx = event->x() - lastPos.x();
    int dy = event->y() - lastPos.y();

    if (event->buttons() & Qt::LeftButton) {
        setXRotation(xRot + 8 * dy);
        setYRotation(yRot + 8 * dx);
    } else if (event->buttons() & Qt::RightButton) {
        setXTranslation(xTranslation + dx);
        setYTranslation(yTranslation + dy);
    }
    lastPos = event->pos();
}

//------------------------------------------------------------------------------
void gui::GLWidget::wheelEvent(QWheelEvent *event)
//------------------------------------------------------------------------------
{
    double numDegrees = -event->angleDelta().y() / 8.0;
    double numSteps = numDegrees / 15.0;
    m_scaling *= std::pow(1.125, numSteps);
    update();
}

//------------------------------------------------------------------------------
void gui::GLWidget::drawBox(const double *bounds)
//------------------------------------------------------------------------------
{
    glBegin(GL_LINES);
    glVertex3d(bounds[0], bounds[2], bounds[4]);
    glVertex3d(bounds[0], bounds[2], bounds[5]);
    glVertex3d(bounds[0], bounds[3], bounds[4]);
    glVertex3d(bounds[0], bounds[3], bounds[5]);

    glVertex3d(bounds[1], bounds[2], bounds[4]);
    glVertex3d(bounds[1], bounds[2], bounds[5]);
    glVertex3d(bounds[1], bounds[3], bounds[4]);
    glVertex3d(bounds[1], bounds[3], bounds[5]);
    glEnd();

    glBegin(GL_LINE_LOOP);
    glVertex3d(bounds[0], bounds[2], bounds[4]);
    glVertex3d(bounds[1], bounds[2], bounds[4]);
    glVertex3d(bounds[1], bounds[3], bounds[4]);
    glVertex3d(bounds[0], bounds[3], bounds[4]);
    glEnd();

    glBegin(GL_LINE_LOOP);
    glVertex3d(bounds[0], bounds[2], bounds[5]);
    glVertex3d(bounds[1], bounds[2], bounds[5]);
    glVertex3d(bounds[1], bounds[3], bounds[5]);
    glVertex3d(bounds[0], bounds[3], bounds[5]);
    glEnd();
}

//------------------------------------------------------------------------------
void gui::GLWidget::drawMesh(const dm::Mesh *p_mesh)
//------------------------------------------------------------------------------
{
    if (!p_mesh)
        return;

    glBegin(GL_TRIANGLES);
    size_t nbT = p_mesh->m_triangles.size();
    for (size_t i = 0; i < nbT; i++) {
        const dm::Triangle &tr = p_mesh->m_triangles[i];
        for (unsigned int s = 0; s < 3; s++) {
            glNormal3dv(tr.normal);
            glVertex3dv(tr.nodes[s]->coo);
        }
    }
    glEnd();
}

//------------------------------------------------------------------------------
void gui::GLWidget::drawWireframe(const dm::Mesh *p_mesh)
//------------------------------------------------------------------------------
{
    if (!p_mesh)
        return;

    size_t nbT = p_mesh->m_triangles.size();
    for (size_t i = 0; i < nbT; i++) {
        const dm::Triangle &tr = p_mesh->m_triangles[i];
        glBegin(GL_LINE_LOOP);
        glVertex3dv(tr.nodes[0]->coo);
        glVertex3dv(tr.nodes[1]->coo);
        glVertex3dv(tr.nodes[2]->coo);
        glEnd();
    }
}

//------------------------------------------------------------------------------
void gui::GLWidget::drawCoorSyst(const int p_size)
//------------------------------------------------------------------------------
{
    glBegin(GL_LINES);

    glColor3f(1.f, 0.f, 0.f);
    glVertex3d(0., 0., 0.);
    glVertex3d(p_size, 0., 0.);

    glColor3f(0.f, 1.f, 0.f);
    glVertex3d(0., 0., 0.);
    glVertex3d(0., p_size, 0.);

    glColor3f(0.f, 0.f, 1.f);
    glVertex3d(0., 0., 0.);
    glVertex3d(0., 0., p_size);

    glEnd();
}

//------------------------------------------------------------------------------
void gui::GLWidget::disableLight()
//------------------------------------------------------------------------------
{
    glDisable(GL_COLOR_MATERIAL);
    glDisable(GL_LIGHT0);
    glDisable(GL_LIGHTING);
}

//------------------------------------------------------------------------------
void gui::GLWidget::enableLight()
//------------------------------------------------------------------------------
{
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHTING);
}
