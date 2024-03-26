#include "avframeopenglwidget.h"

AVFrameOpenGLWidget::AVFrameOpenGLWidget(QWidget *parent)
    :
    QOpenGLWidget{parent},
    m_texture(QOpenGLTexture::Target2D)
{
    m_texture.create();
}

GLuint g_texture;

void AVFrameOpenGLWidget::initializeGL()
{
    initializeOpenGLFunctions();
    glGenTextures(1,&g_texture);

    glBindTexture(GL_TEXTURE_2D, g_texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); /* We will use linear interpolation for magnification filter */
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); /* We will use linear interpolation for minifying filter */

    glEnable(GL_TEXTURE_2D);     // Enable 2D texturing

    glMatrixMode(GL_PROJECTION);     // Make a simple 2D projection on the entire window

    glMatrixMode(GL_MODELVIEW);    // Set the matrix mode to object modeling
    //glTranslatef( 0, 0, -15 );

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClearDepth(0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear the window
}

void AVFrameOpenGLWidget::setViewPort(int width, int height)
{
    m_texture.setSize(width,height,)
}

void AVFrameOpenGLWidget::paintGL()
{
    // Clear color and depth buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);     // Operate on model-view matrix

    glEnable(GL_TEXTURE_2D);
    GLuint texture = g_texture;
    glBindTexture(GL_TEXTURE_2D, texture);

    if(m_frame != NULL){
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_frame->width,
                     m_frame->height, 0, GL_RGB, GL_UNSIGNED_BYTE,
                     m_frame->data[0]);
    }

    // Draw a quad
    glBegin(GL_QUADS);
    glTexCoord2f(0, 0);
    glVertex2i(0, 0); // top left
    glTexCoord2f(1, 0);
    glVertex2i(1024, 0); // top right
    glTexCoord2f(1, 1);
    glVertex2i(1024, 1024); // bottom right
    glTexCoord2f(0, 1);
    glVertex2i(0, 1024); // bottom left
    glEnd();

    glDisable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);

    glFlush();
}

void AVFrameOpenGLWidget::resizeGL(int w, int h)
{

}



