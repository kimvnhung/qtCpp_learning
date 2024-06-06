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

public slots:
    // YUV420P
    /*!
     * \brief setYUV420pParameters
     * call once before setFrameData() if parameters changed
     * \param w frame width
     * \param h frame height
     * \param strides strides of each plane. If null, it's equals to {w, w/2, w/2}.
     */
    void setYUV420pParameters(int w, int h, int* strides = NULL);
    void setFrameData(const QByteArray& data);
    // QImage
    /*!
     * \brief setQImageParameters
     * call once before setImage() if parameters changed
     * \param fmt only RGB888 is supported
     * \param stride QImage.bytesPerLine()
     */
    void setQImageParameters(QImage::Format fmt, int w, int h, int stride);
    void setImage(const QImage& img);
    // TODO: only init(w,h,strides) init(QImage::Format, w, h, strides)
    void setRGBParameters(int w, int h);
    void setRGBFrame(const char* data);

protected:
    void bind();
    void bindPlane(int p);
    void initializedShader();
    void initTexttures();
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

signals:
private:
    class Private;
    QSharedPointer<Private> d;
};

#endif // GLWIDGET_H
