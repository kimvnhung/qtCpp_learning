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
    GLfloat vertices[6][2] =
        {
            { -0.90f, -0.90f }, // Triangle 1
            { +0.85f, -0.90f },
            { -0.90f, +0.85f },

            { +0.90f, -0.85f }, // Triangle 2
            { +0.90f, +0.90f },
            { -0.85f, +0.90f }
        };

    m_buffer = QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    m_buffer.setUsagePattern(QOpenGLBuffer::StaticDraw);
    Q_ASSERT(m_buffer.create());
    Q_ASSERT(m_buffer.bind());
    m_buffer.allocate(sizeof(vertices));
    m_buffer.write(0, vertices, sizeof(vertices));

    m_shaderProgram.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/begin.vert");
    m_shaderProgram.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/begin.frag");
    Q_ASSERT(m_shaderProgram.link());
    Q_ASSERT(m_shaderProgram.bind());

    const int vPosition = 0;

    f->glVertexAttribPointer(vPosition, 2, GL_FLOAT, GL_FALSE, 0, NULL);

    f->glEnableVertexAttribArray(vPosition);

    f->glClear(GL_COLOR_BUFFER_BIT);

    f->glClearColor(0.25f, 0.45f, 0.65f, 1.0f);
}

void QGLBegin::paintGL()
{
    QOpenGLExtraFunctions *f = context()->extraFunctions();
    f->glDrawArrays(GL_TRIANGLES, 0, 6);
    f->glFlush();

}

void QGLBegin::resizeGL(int widget, int height)
{
    Q_UNUSED(widget);
    Q_UNUSED(height);
}

