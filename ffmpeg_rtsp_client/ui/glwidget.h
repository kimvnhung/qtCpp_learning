#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QOpenGLFunctions>
#include <QOpenGLWidget>
#include <QWidget>

class GLWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT
public:
    explicit GLWidget(QWidget *parent = nullptr);
    ~GLWidget();

protected:
    void bind();
    void bindPlane(int p);
    void initializedShader();
    void initTexttures();
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

signals:
};

#endif // GLWIDGET_H
