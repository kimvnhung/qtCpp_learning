#ifndef QGLBEGIN_H
#define QGLBEGIN_H

#include <QWidget>
#include <QOpenGLWidget>

class QGLBegin : public QOpenGLWidget
{
    Q_OBJECT
public:
    explicit QGLBegin(QWidget *parent = nullptr);

    void setRotation(float x,float y, float z);

protected:
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int width, int height) override;

private:
    float xrot,yrot,zrot;
};

#endif // QGLBEGIN_H
