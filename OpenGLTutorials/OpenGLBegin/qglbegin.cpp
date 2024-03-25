#include "qglbegin.h"

#include <QOpenGLExtraFunctions>


QGLBegin::QGLBegin(QWidget *parent)
    : QOpenGLWidget{parent}
{
    xrot = yrot = zrot = 0.0;
}

void QGLBegin::initializeGL()
{
    QOpenGLExtraFunctions *f = context()->extraFunctions();
    f->glClearColor(0.0, 0.0, 0.0, 0.0);
    f->glEnable(GL_LIGHTING);
    f->glEnable(GL_LIGHT0);
}

void QGLBegin::paintGL()
{
    QOpenGLExtraFunctions *f = context()->extraFunctions();
    f->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

}

void QGLBegin::resizeGL(int widget, int height)
{

}

