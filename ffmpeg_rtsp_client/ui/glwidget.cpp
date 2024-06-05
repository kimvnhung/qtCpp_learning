#include "glwidget.h"

GLWidget::GLWidget(QWidget *parent)
    : QOpenGLWidget{parent}
{}

GLWidget::~GLWidget()
{}

void GLWidget::bind()
{}

void GLWidget::bindPlane(int p)
{
    Q_UNUSED(p);
}

void GLWidget::initializedShader()
{}

void GLWidget::initTexttures()
{}

void GLWidget::initializeGL()
{}

void GLWidget::resizeGL(int w, int h)
{
    Q_UNUSED(w);
    Q_UNUSED(h);
}

void GLWidget::paintGL()
{}
