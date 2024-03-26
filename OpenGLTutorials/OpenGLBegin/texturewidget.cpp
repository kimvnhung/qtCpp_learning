#include "data_type.h"
#include "texturewidget.h"

TextureWidget::TextureWidget(QWidget *parent):
    QOpenGLWidget{parent}
{

}

void TextureWidget::initializeGL()
{
    initializeOpenGLFunctions();
    Vertex vertices[8];

    QVector2D bottomLeft = QVector2D(0.0f,0.0f);
    QVector2D topLeft = QVector2D(0.0f, 1.0f);
    QVector2D bottomRight = QVector2D(1.0f, 0.0f);
    QVector2D topRight = QVector2D(1.0f, 1.0f);

    vertices[0] = Vertex(QVector3D(0.5f, 0.5f, 0.5f),bottomLeft);
    vertices[1] = Vertex(QVector3D(0.5f, 0.5f, 0.5f),bottomLeft);
    vertices[0] = Vertex(QVector3D(0.5f, 0.5f, 0.5f),bottomLeft);
    vertices[0] = Vertex(QVector3D(0.5f, 0.5f, 0.5f),bottomLeft);
    vertices[0] = Vertex(QVector3D(0.5f, 0.5f, 0.5f),bottomLeft);
    vertices[0] = Vertex(QVector3D(0.5f, 0.5f, 0.5f),bottomLeft);
    vertices[0] = Vertex(QVector3D(0.5f, 0.5f, 0.5f),bottomLeft);
    vertices[0] = Vertex(QVector3D(0.5f, 0.5f, 0.5f),bottomLeft);
    vertices[0] = Vertex(QVector3D(0.5f, 0.5f, 0.5f),bottomLeft);

    m_buffer = QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);

}

void TextureWidget::paintGL()
{

}

void TextureWidget::resizeGL(int w, int h)
{

}
