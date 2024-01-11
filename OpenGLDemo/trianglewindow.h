#ifndef TRIANGLEWINDOW_H
#define TRIANGLEWINDOW_H

#include <QObject>
#include "openglwindow.h"
#include <QtOpenGL/QOpenGLShaderProgram>

class TriangleWindow : public OpenGLWindow
{
    Q_OBJECT
public:
    using OpenGLWindow::OpenGLWindow;

    void initialize() override;
    void render() override;

private:
    GLint m_posAttr = 0;
    GLint m_colAttr = 0;
    GLint m_matrixUniform = 0;

    QOpenGLShaderProgram *m_program = nullptr;
    int m_frame = 0;
signals:
};

#endif // TRIANGLEWINDOW_H
