#include "imagewindow.h"

#include <QPainter>

ImageWindow::ImageWindow(QWidget *parent)
    : QOpenGLWidget{parent}
{}

void ImageWindow::initializeGL(){
}

void ImageWindow::paintGL(){
    QPainter p(this);
    QImage image(":/image1.jpg");
    QRect imageRec = image.rect();
    QRect target((width()-500)/2,200,500,imageRec.height()*500/imageRec.width());
    p.drawImage(target,image);

}

void ImageWindow::resizeGL(int width, int height)
{

}
