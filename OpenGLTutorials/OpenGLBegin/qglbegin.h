#ifndef QGLBEGIN_H
#define QGLBEGIN_H

#include <QWidget>
#include <QOpenGLWidget>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>

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
    QOpenGLBuffer m_buffer;
    QOpenGLShaderProgram m_shaderProgram;
};

#endif // QGLBEGIN_H
