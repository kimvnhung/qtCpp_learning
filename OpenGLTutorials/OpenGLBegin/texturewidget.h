#ifndef TEXTUREWIDGET_H
#define TEXTUREWIDGET_H

#include <QOpenGLBuffer>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLWidget>
#include <QWidget>

class TextureWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT
public:
    explicit TextureWidget(QWidget *parent = nullptr);
protected:
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int w,int h) override;

signals:
private:
    QOpenGLBuffer m_buffer;
    QOpenGLShaderProgram m_shaderProgram;
};

#endif // TEXTUREWIDGET_H
