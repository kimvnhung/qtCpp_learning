#include "qopenglskyboxwidget.h"

#include <QMouseEvent>
#include <QWheelEvent>

QOpenGLSkyboxWidget::QOpenGLSkyboxWidget(const QString& frontImagePath,
                                         const QString& backImagePath,
                                         const QString& topImagePath,
                                         const QString& bottomImagePath,
                                         const QString& leftImagePath,
                                         const QString& rightImagePath,
                                         QWidget *parent):
    QOpenGLWidget{parent},
    mTexture(QOpenGLTexture::TargetCubeMap),
    mVertexBuf(QOpenGLBuffer::VertexBuffer),
    mSpeed(0.0f),
    mFrontImagePath(frontImagePath),
    mBackImagePath(backImagePath),
    mTopImagePath(topImagePath),
    mBottomImagePath(bottomImagePath),
    mLeftImagePath(leftImagePath),
    mRightImagePath(rightImagePath)
{
    mLookAt.eye =    {+0.0f, +0.0f, +0.0f};
    mLookAt.center = {+0.0f, +0.0f, -1.0f};
    mLookAt.up =     {+0.0f, +1.0f, +0.0f};
}

QImage normalizeSquaredImage(QImage image)
{
    int imageSize = std::min(image.width(),image.height());
    QRect rect = QRect(
        (image.width() - imageSize) / 2,
        (image.height() - imageSize) / 2,
        imageSize,
        imageSize
    );

    return image.copy(rect);
}

void QOpenGLSkyboxWidget::loadImages()
{
    const QImage posx = normalizeSquaredImage(QImage(mRightImagePath)).convertToFormat(QImage::Format_RGBA8888);
    const QImage negx = normalizeSquaredImage(QImage(mLeftImagePath)).convertToFormat(QImage::Format_RGBA8888);

    const QImage posy = normalizeSquaredImage(QImage(mTopImagePath)).convertToFormat(QImage::Format_RGBA8888);
    const QImage negy = normalizeSquaredImage(QImage(mBottomImagePath)).convertToFormat(QImage::Format_RGBA8888);

    const QImage posz = normalizeSquaredImage(QImage(mFrontImagePath)).convertToFormat(QImage::Format_RGBA8888);
    const QImage negz = normalizeSquaredImage(QImage(mBackImagePath)).convertToFormat(QImage::Format_RGBA8888);

    mTexture.create();
    mTexture.setSize(posx.width(),posx.height(),posx.depth());

    mTexture.setFormat(QOpenGLTexture::RGBA8_UNorm);
    mTexture.allocateStorage();

    mTexture.setData(0, 0, QOpenGLTexture::CubeMapPositiveX,
                     QOpenGLTexture::RGBA, QOpenGLTexture::UInt8,
                     posx.constBits(), Q_NULLPTR);

    mTexture.setData(0, 0, QOpenGLTexture::CubeMapPositiveY,
                     QOpenGLTexture::RGBA, QOpenGLTexture::UInt8,
                     posy.constBits(), Q_NULLPTR);

    mTexture.setData(0, 0, QOpenGLTexture::CubeMapPositiveZ,
                     QOpenGLTexture::RGBA, QOpenGLTexture::UInt8,
                     posz.constBits(), Q_NULLPTR);

    mTexture.setData(0, 0, QOpenGLTexture::CubeMapNegativeX,
                     QOpenGLTexture::RGBA, QOpenGLTexture::UInt8,
                     negx.constBits(), Q_NULLPTR);

    mTexture.setData(0, 0, QOpenGLTexture::CubeMapNegativeY,
                     QOpenGLTexture::RGBA, QOpenGLTexture::UInt8,
                     negy.constBits(), Q_NULLPTR);

    mTexture.setData(0, 0, QOpenGLTexture::CubeMapNegativeZ,
                     QOpenGLTexture::RGBA, QOpenGLTexture::UInt8,
                     negz.constBits(), Q_NULLPTR);

    mTexture.setWrapMode(QOpenGLTexture::ClampToEdge);
    mTexture.setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
    mTexture.setMagnificationFilter(QOpenGLTexture::LinearMipMapLinear);
}

void QOpenGLSkyboxWidget::initializeGL()
{
    initializeOpenGLFunctions();

    mProgram.addShaderFromSourceCode(
        QOpenGLShader::Vertex,
        R"(
                #version 330 core
                layout (location = 0) in vec3 Position;
                layout (location = 0) in vec2 TextCoord;

                uniform mat4 gWVP;

                out vec2 TextCoord0;

                void main()
                {
                    gl_Position = gWVP * vec4(aPosition, 1.0);
                    TextCoord0 = TextCoord;
                }
                )");

    mProgram.addShaderFromSourceCode(
        QOpenGLShader::Fragment,
        R"(
                #version 330 core
                in vec2 TextCoord0;

                out vec4 FragColor;

                uniform sampler2D gSampler;

                void main()
                {
                    FragColor = texture2D(gSampler, TextCoord0);
                }
                )");

    mProgram.link();
    mProgram.bind();

    loadImages();

    QVector3D vertices[] =
        {
            {-1.0f,  1.0f, -1.0f},
            {-1.0f, -1.0f, -1.0f},
            {+1.0f, -1.0f, -1.0f},
            {+1.0f, -1.0f, -1.0f},
            {+1.0f, +1.0f, -1.0f},
            {-1.0f, +1.0f, -1.0f},

            {-1.0f, -1.0f, +1.0f},
            {-1.0f, -1.0f, -1.0f},
            {-1.0f, +1.0f, -1.0f},
            {-1.0f, +1.0f, -1.0f},
            {-1.0f, +1.0f, +1.0f},
            {-1.0f, -1.0f, +1.0f},

            {+1.0f, -1.0f, -1.0f},
            {+1.0f, -1.0f, +1.0f},
            {+1.0f, +1.0f, +1.0f},
            {+1.0f, +1.0f, +1.0f},
            {+1.0f, +1.0f, -1.0f},
            {+1.0f, -1.0f, -1.0f},

            {-1.0f, -1.0f, +1.0f},
            {-1.0f, +1.0f, +1.0f},
            {+1.0f, +1.0f, +1.0f},
            {+1.0f, +1.0f, +1.0f},
            {+1.0f, -1.0f, +1.0f},
            {-1.0f, -1.0f, +1.0f},

            {-1.0f, +1.0f, -1.0f},
            {+1.0f, +1.0f, -1.0f},
            {+1.0f, +1.0f, +1.0f},
            {+1.0f, +1.0f, +1.0f},
            {-1.0f, +1.0f, +1.0f},
            {-1.0f, +1.0f, -1.0f},

            {-1.0f, -1.0f, -1.0f},
            {-1.0f, -1.0f, +1.0f},
            {+1.0f, -1.0f, -1.0f},
            {+1.0f, -1.0f, -1.0f},
            {-1.0f, -1.0f, +1.0f},
            {+1.0f, -1.0f, +1.0f}
        };

    mVertexBuf.create();
    mVertexBuf.bind();
    mVertexBuf.allocate(vertices, 36 * sizeof(QVector3D));

    mProgram.enableAttributeArray("aPosition");
    mProgram.setAttributeBuffer("aPosition",
                                GL_FLOAT,
                                0,
                                3,
                                sizeof(QVector3D));

    mProgram.setUniformValue("uTexture", 0);
}

void QOpenGLSkyboxWidget::paintGL()
{
    mTexture.bind();

    mModelMat.setToIdentity();

    mViewMat.setToIdentity();
    mViewMat.lookAt(mLookAt.eye,
                    mLookAt.center,
                    mLookAt.up);

    mProjectionMat.setToIdentity();
    mProjectionMat.perspective(mPerspective.verticalAngle,
                               mPerspective.aspectRatio,
                               mPerspective.nearPlane,
                               mPerspective.farPlane);

    mProgram.setUniformValue("mvpMatrix", mProjectionMat * mViewMat * mModelMat);

    glDrawArrays(GL_TRIANGLES,
                 0,
                 36);
}

void QOpenGLSkyboxWidget::resizeGL(int w, int h)
{
    mPerspective.verticalAngle = 60.0;
    mPerspective.nearPlane = 0.0;
    mPerspective.farPlane = 1.0;
    mPerspective.aspectRatio =
        static_cast<float>(w) / static_cast<float>(h ? h : 1.0f);
}

void QOpenGLSkyboxWidget::mouseMoveEvent(QMouseEvent *event)
{
    if(event->buttons() & Qt::LeftButton)
    {
        auto diff = QVector2D(event->localPos()) - mMousePressPosition;
        auto n = QVector3D(diff.y(), diff.x(), 0.0).normalized();
        mSpeed = diff.length() / 100.0f;
        if(mSpeed > 1.0f) mSpeed = 1.0f; // speed threshold
        mRotationAxis = (mRotationAxis + n * mSpeed).normalized();
    }
}

void QOpenGLSkyboxWidget::mousePressEvent(QMouseEvent *event)
{
    mMousePressPosition = QVector2D(event->localPos());
    mTimer.start(10, this);
}

void QOpenGLSkyboxWidget::mouseReleaseEvent(QMouseEvent*)
{
    mTimer.stop();
}

void QOpenGLSkyboxWidget::timerEvent(QTimerEvent *)
{
    mRotation = QQuaternion::fromAxisAndAngle(mRotationAxis, mSpeed) * mRotation;

    QMatrix4x4 mat;
    mat.setToIdentity();
    mat.rotate(mRotation);

    mLookAt.center = {+0.0f, +0.0f, -1.0f};
    mLookAt.center = mLookAt.center * mat;

    update();
}

void QOpenGLSkyboxWidget::wheelEvent(QWheelEvent *event)
{
    // float delta = event.> 0 ? -5.0f : +5.0f;
    // mPerspective.verticalAngle += delta;
    // if(mPerspective.verticalAngle < 10.0f)
    //     mPerspective.verticalAngle = 10.0f;
    // else if(mPerspective.verticalAngle > 120.0f)
    //     mPerspective.verticalAngle = 120.0f;

    // update();
}
