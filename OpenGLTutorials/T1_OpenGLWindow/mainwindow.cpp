#include "mainwindow.h"

#include <QApplication>
#include <QOpenGLExtraFunctions>
#include <QScreen>
#include <QStyle>
#include <QTimer>

#define FPS 60

MainWindow::MainWindow()
{
    setGeometry(QStyle::alignedRect(
        Qt::LeftToRight,
        Qt::AlignCenter,
        QSize(500,500),
        QApplication::primaryScreen()->availableGeometry()
        ));

    if(m_timer == NULL){
        m_timer = new QTimer(this);
        m_timer->setInterval(1000/FPS);
        connect(m_timer,&QTimer::timeout,[this]{
            update();
        });
        m_timer->start();
    }
}

MainWindow::~MainWindow()
{

}

void MainWindow::initializeGL()
{
    qDebug()<<__FUNCTION__;
    QOpenGLExtraFunctions *f = QOpenGLContext::currentContext()->extraFunctions();

    f->glClearColor(0, 0, 0, 1);
    f->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

float red = 0.f,green = 0.f,blue = 0.f;

void MainWindow::paintGL()
{
    // qDebug()<<__FUNCTION__;
    QOpenGLExtraFunctions *f = QOpenGLContext::currentContext()->extraFunctions();
    red += 1.f/256.f;
    green += 1.f/256.f;
    blue += 1.f/256.f;
    if(red >= 1)
        red = 0.f;

    if(green >= 1)
        green = 0.f;

    if(blue >= 1)
        blue = 0.f;

    qDebug()<<"red "<<red;
    f->glClearColor(red, green, blue, 1);
    f->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void MainWindow::resizeGL(int w, int h)
{
    qDebug()<<__FUNCTION__;
}
