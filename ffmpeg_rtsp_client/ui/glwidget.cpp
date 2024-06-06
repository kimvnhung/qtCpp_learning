#include "glwidget.h"

#include <QWidget>
#include <QOpenGLFunctions>
#include <QOpenGLWidget>
#include <QOpenGLShaderProgram>
#include <QMutex>

static const QMatrix4x4 yuv2rgb_bt601 =
    QMatrix4x4(
        1.0f,  0.000f,  1.402f, 0.0f,
        1.0f, -0.344f, -0.714f, 0.0f,
        1.0f,  1.772f,  0.000f, 0.0f,
        0.0f,  0.000f,  0.000f, 1.0f)
    *
    QMatrix4x4(
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, -0.5f,
        0.0f, 0.0f, 1.0f, -0.5f,
        0.0f, 0.0f, 0.0f, 1.0f);


const GLfloat kVertices[] = {
    -1, 1,
    -1, -1,
    1, 1,
    1, -1,
};
const GLfloat kTexCoords[] = {
    0, 0,
    0, 1,
    1, 0,
    1, 1,
};

char const *const* attributes()
{
    static const char a0[] = {0x61, 0x5f, 0x50, 0x6f, 0x73, 0x0};
    static const char a1[] = {0x61, 0x5f, 0x54, 0x65, 0x78, 0x0};
    static const char a2[] = {0x00, 0x51, 0x74, 0x41, 0x56, 0x0};
    static const char* A[] = { a0, a1, a2};
    return A;
}

typedef struct {
    QImage::Format qfmt;
    GLint internal_fmt;
    GLenum fmt;
    GLenum type;
    int bpp;
} gl_fmt_entry_t;

#define glsl(x) #x
static const char kVertexShader[] = glsl(
    attribute vec4 a_Pos;
    attribute vec2 a_Tex;
    uniform mat4 u_MVP_matrix;
    varying vec2 v_TexCoords;
    void main() {
        gl_Position = u_MVP_matrix * a_Pos;
        v_TexCoords = a_Tex;
    });

static const char kFragmentShader[] = glsl(
    uniform sampler2D u_Texture0;
    uniform sampler2D u_Texture1;
    uniform sampler2D u_Texture2;
    varying mediump vec2 v_TexCoords;
    uniform mat4 u_colorMatrix;
    void main()
    {
        gl_FragColor = clamp(u_colorMatrix
                                 * vec4(
                                     texture2D(u_Texture0, v_TexCoords).r,
                                     texture2D(u_Texture1, v_TexCoords).r,
                                     texture2D(u_Texture2, v_TexCoords).r,
                                     1)
                             , 0.0, 1.0);
    });

static const char kFragmentShaderRGB[] = glsl(
    uniform sampler2D u_Texture0;
    varying mediump vec2 v_TexCoords;
    void main() {
        vec4 c = texture2D(u_Texture0, v_TexCoords);
        gl_FragColor = c.rgba;
    });
#undef glsl

class GLWidget::Private
{
public:
    Private(GLWidget *parent, bool update_res, bool upload_tex, QOpenGLShaderProgram* program)
        : q{parent}
        , update_res{update_res}
        , upload_tex{upload_tex}
        , program{program}
    {
        memset(tex,0,3);
    }

    GLWidget *q;

    bool update_res;
    bool upload_tex;
    int width;
    int height;
    float init_ratio;

    QByteArray m_data;
    QImage m_image;

    typedef struct {
        char* data;
        int stride;
        GLint internal_fmt;
        GLenum fmt;
        GLenum type;
        int bpp;
        QSize tex_size;
        QSize upload_size;
    } Plane;

    QVector<Plane> plane;

    //QSize tex_size[3], tex_upload_size[3];
    GLuint tex[3];
    int u_MVP_matrix, u_colorMatrix, u_Texture[3];
    QOpenGLShaderProgram *program;
    QMatrix4x4 mat;

    void setFrameData(const QByteArray& data)
    {
        upload_tex = true;
        m_data = data;
        plane[0].data = (char*)m_data.constData();
        if (plane.size() > 1) {
            plane[1].data = plane[0].data + plane[0].stride*height;
            plane[2].data = plane[1].data + plane[1].stride*height/2;
        }
        q->update();
    }

    void setImage(const QImage& img)
    {
        upload_tex = true;
        m_image = img;
        plane[0].data = (char*)m_image.constBits();
        q->update();
    }

    void setRGBFrame(const char* data)
    {
        upload_tex = true;
        // m_image = img;
        // using strcpy_s instead of strcpy for strcpy(plane[0].data,data);
        strcpy_s(plane[0].data, strlen(data) + 1, data);

        q->update();
    }

    void setYUV420pParameters(int w, int h, int* strides)
    {
        update_res = true;
        m_data.clear();
        m_image = QImage();
        width = w;
        height = h;
        plane.resize(3);
        Plane &p = plane[0];
        p.data = 0;
        p.stride = strides && strides[0] ? strides[0] : w;
        p.tex_size.setWidth(p.stride);
        p.upload_size.setWidth(p.stride);
        p.tex_size.setHeight(h);
        p.upload_size.setHeight(h);
        p.internal_fmt = p.fmt = GL_LUMINANCE;
        p.type = GL_UNSIGNED_BYTE;
        p.bpp = 1;
        for (int i = 1; i < plane.size(); ++i) {
            Plane &p = plane[i];
            p.stride = strides && strides[i] ? strides[i] : w/2;
            p.tex_size.setWidth(p.stride);
            p.upload_size.setWidth(p.stride);
            p.tex_size.setHeight(h/2);
            p.upload_size.setHeight(h/2);
            p.internal_fmt = p.fmt = GL_LUMINANCE;
            p.type = GL_UNSIGNED_BYTE;
            p.bpp = 1;
            qDebug() << p.tex_size;
        }
    }

    void setQImageParameters(QImage::Format fmt, int w, int h, int stride)
    {
        update_res = true;
        m_data.clear();
        m_image = QImage();
        width = w;
        height = h;
        plane.resize(1);
        Plane &p = plane[0];
        p.data = 0;
        p.stride = stride ? stride : QImage(w, h, fmt).bytesPerLine();
        static const gl_fmt_entry_t fmts[] = {
            { QImage::Format_RGB888, GL_RGB, GL_RGB, GL_UNSIGNED_BYTE, 3},
            { QImage::Format_Invalid, 0, 0, 0, 0}
        };
        for (int i = 0; fmts[i].bpp; ++i) {
            if (fmts[i].qfmt == fmt) {
                Plane &p = plane[0];
                p.internal_fmt = fmts[i].internal_fmt;
                p.fmt = fmts[i].fmt;
                p.type = fmts[i].type;
                p.internal_fmt = fmts[i].internal_fmt;
                p.bpp = fmts[i].bpp;

                p.tex_size.setWidth(p.stride/p.bpp);
                p.upload_size.setWidth(p.stride/p.bpp);
                p.tex_size.setHeight(h);
                p.upload_size.setHeight(h);
                return;
            }
        }
        qFatal("Unsupported QImage format %d!", fmt);
    }

    void setRGBParameters(int w, int h)
    {
        if(width == w && height == h){
            return;
        }
        update_res = true;
        m_data.clear();
        m_image = QImage();
        width = w;
        height = h;
        init_ratio = w*1.f/h;
        mat.setToIdentity();
        plane.resize(1);
        Plane &p = plane[0];
        p.data = 0;
        p.stride = QImage(w, h, QImage::Format_RGB888).bytesPerLine();
        static const gl_fmt_entry_t fmts[] = {
            { QImage::Format_RGB888, GL_RGB, GL_RGB, GL_UNSIGNED_BYTE, 3},
            { QImage::Format_Invalid, 0, 0, 0, 0}
        };
        for (int i = 0; fmts[i].bpp; ++i) {
            if (fmts[i].qfmt == QImage::Format_RGB888) {
                Plane &p = plane[0];
                p.internal_fmt = fmts[i].internal_fmt;
                p.fmt = fmts[i].fmt;
                p.type = fmts[i].type;
                p.internal_fmt = fmts[i].internal_fmt;
                p.bpp = fmts[i].bpp;

                p.tex_size.setWidth(p.stride/p.bpp);
                p.upload_size.setWidth(p.stride/p.bpp);
                p.tex_size.setHeight(h);
                p.upload_size.setHeight(h);
                return;
            }
        }
    }

    void initializeShader()
    {
        if (program) {
            program->release();
            delete program;
            program = 0;
        }
        program = new QOpenGLShaderProgram(q);

        program->addShaderFromSourceCode(QOpenGLShader::Vertex, kVertexShader);
        QByteArray frag;
        if (plane.size() > 1)
            frag = QByteArray(kFragmentShader);
        else
            frag = QByteArray(kFragmentShaderRGB);
        frag.prepend("#ifdef GL_ES\n"
                     "precision mediump int;\n"
                     "precision mediump float;\n"
                     "#else\n"
                     "#define highp\n"
                     "#define mediump\n"
                     "#define lowp\n"
                     "#endif\n");
        program->addShaderFromSourceCode(QOpenGLShader::Fragment, frag);

        char const *const *attr = attributes();
        for (int i = 0; attr[i][0]; ++i) {
            program->bindAttributeLocation(attr[i], i);
        }
        if (!program->link()) {
            qWarning("QSGMaterialShader: Shader compilation failed:");
            qWarning() << program->log();
            qDebug("frag: %s", plane.size() > 1 ? kFragmentShader : kFragmentShaderRGB);
        }

        u_MVP_matrix = program->uniformLocation("u_MVP_matrix");
        // fragment shader
        u_colorMatrix = program->uniformLocation("u_colorMatrix");
        for (int i = 0; i < plane.size(); ++i) {
            QString tex_var = QString("u_Texture%1").arg(i);
            u_Texture[i] = program->uniformLocation(tex_var);
            // qDebug("glGetUniformLocation(\"%s\") = %d", tex_var.toUtf8().constData(), u_Texture[i]);
        }
        // qDebug("glGetUniformLocation(\"u_MVP_matrix\") = %d", u_MVP_matrix);
        // qDebug("glGetUniformLocation(\"u_colorMatrix\") = %d", u_colorMatrix);
    }
};

GLWidget::GLWidget(QWidget *parent)
    : QOpenGLWidget{parent}
    , d{new Private{this, true, true, 0}}
{

}

GLWidget::~GLWidget()
{
    d.clear();
}

//slot
void GLWidget::setYUV420pParameters(int w, int h, int* strides)
{
    QMutexLocker locker(&mutex);
    Q_UNUSED(locker);
    if(d)
        d->setYUV420pParameters(w, h, strides);
}

void GLWidget::setQImageParameters(QImage::Format fmt, int w, int h, int stride)
{
    QMutexLocker locker(&mutex);
    Q_UNUSED(locker);
    if(d)
        d->setQImageParameters(fmt, w, h, stride);
}

void GLWidget::setRGBParameters(int w, int h)
{
    QMutexLocker locker(&mutex);
    Q_UNUSED(locker);
    if(d)
        d->setRGBParameters(w, h);
}

void GLWidget::setFrameData(const QByteArray& data)
{
    QMutexLocker locker(&mutex);
    Q_UNUSED(locker);
    if(d)
        d->setFrameData(data);
}

void GLWidget::setImage(const QImage& img)
{
    QMutexLocker locker(&mutex);
    Q_UNUSED(locker);
    if(d)
        d->setImage(img);
}

void GLWidget::setRGBFrame(const char* data)
{
    QMutexLocker locker(&mutex);
    Q_UNUSED(locker);
    if(d)
        d->setRGBFrame(data);
}

void GLWidget::bind()
{
    for (int i = 0; i < d->plane.size(); ++i) {
        bindPlane((i + 1) % d->plane.size());
    }
    d->upload_tex = false;
}

void GLWidget::bindPlane(int p)
{
    glActiveTexture(GL_TEXTURE0 + p);
    glBindTexture(GL_TEXTURE_2D, d->tex[p]);
    if (!d->upload_tex)
        return;
    // This is necessary for non-power-of-two textures
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    const Private::Plane &P = d->plane[p];
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, P.upload_size.width(), P.upload_size.height(), P.fmt, P.type, P.data);
}

void GLWidget::initializedShader()
{
    d->initializeShader();
}

void GLWidget::initTexttures()
{
    glDeleteTextures(3, d->tex);
    memset(d->tex, 0, 3);
    glGenTextures(d->plane.size(), d->tex);
    //qDebug("init textures...");
    for (int i = 0; i < d->plane.size(); ++i) {
        const Private::Plane &P = d->plane[i];
        //qDebug("tex[%d]: %u", i, tex[i]);
        glBindTexture(GL_TEXTURE_2D, d->tex[i]);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        // This is necessary for non-power-of-two textures
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexImage2D(GL_TEXTURE_2D, 0, P.internal_fmt, P.tex_size.width(), P.tex_size.height(), 0/*border, ES not support*/, P.fmt, P.type, NULL);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
}

void GLWidget::initializeGL()
{
    initializeOpenGLFunctions();
}

void GLWidget::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
    float new_ratio = w*1.f/h;
    float scaleX,scaleY;
    if(new_ratio > d->init_ratio){
        scaleX = d->init_ratio/new_ratio;
        scaleY = 1;
    }else {
        scaleX = 1;
        scaleY = new_ratio/d->init_ratio;
    }

    d->mat = QMatrix4x4(
        scaleX,0.f,0.f,0.f,
        0.f,scaleY,0.f,0.f,
        0.f,0.f,1.f,0.f,
        0.f,0.f,0.f,1.f
        );
}

void GLWidget::paintGL()
{
    QMutexLocker lock(&mutex);
    Q_UNUSED(lock);
    if(d->plane.size()==0){
        return;
    }
    if (!d->plane[0].data)
        return;
    if (d->update_res || !d->tex[0]) {
        initializedShader();
        initTexttures();
        d->update_res = false;
    }
    bind();
    d->program->bind();
    for (int i = 0; i < d->plane.size(); ++i) {
        d->program->setUniformValue(d->u_Texture[i], (GLint)i);
    }
    d->program->setUniformValue(d->u_colorMatrix, yuv2rgb_bt601);
    d->program->setUniformValue(d->u_MVP_matrix, d->mat);
    // uniform end. attribute begin
    // kVertices ...
    // normalize?
    d->program->setAttributeArray(0, GL_FLOAT, kVertices, 2);
    d->program->setAttributeArray(1, GL_FLOAT, kTexCoords, 2);
    char const *const *attr = attributes();
    for (int i = 0; attr[i][0]; ++i) {
        d->program->enableAttributeArray(i); //TODO: in setActiveShader
    }
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    for (int i = 0; attr[i][0]; ++i) {
        d->program->disableAttributeArray(i); //TODO: in setActiveShader
    }
}
