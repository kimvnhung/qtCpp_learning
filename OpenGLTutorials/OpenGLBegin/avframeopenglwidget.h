#ifndef AVFRAMEOPENGLWIDGET_H
#define AVFRAMEOPENGLWIDGET_H

#include <QOpenGLFunctions>
#include <QOpenGLTexture>
#include <QOpenGLWidget>
#include <QWidget>

extern "C"{
#include <libavutil/frame.h>
}

class AVFrameOpenGLWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT
public:
    explicit AVFrameOpenGLWidget(QWidget *parent = nullptr);
    ~AVFrameOpenGLWidget();

    void setViewPort(int width, int height);
    void updateFrame(AVFrame frame);
protected:
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int w, int h) override;
signals:
private:
    QSize m_viewPort;
    AVFrame *m_frame = NULL;
    QOpenGLTexture m_texture;

};

#endif // AVFRAMEOPENGLWIDGET_H
