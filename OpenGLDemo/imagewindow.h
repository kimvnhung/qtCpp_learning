#ifndef IMAGEWINDOW_H
#define IMAGEWINDOW_H

#include <QObject>
#include <QOpenGLWidget>

class ImageWindow : public QOpenGLWidget
{
    Q_OBJECT
public:
    explicit ImageWindow(QWidget *parent = nullptr);

    // virtual void initializeGL();
    // virtual void paintGL();
    // virtual void resizeGL( int width, int heigh);
};

#endif // IMAGEWINDOW_H
