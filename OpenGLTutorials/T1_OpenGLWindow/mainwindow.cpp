#include "mainwindow.h"

#include <QApplication>
#include <QFile>
#include <QMatrix4x4>
#include <QOpenGLExtraFunctions>
#include <QScreen>
#include <QStyle>
#include <QTimer>
#include <QVector3D>


#define FPS 60

GLuint VBO;
GLuint IBO;
GLint gTranslation;
const char* vsPath = ":/assets/shaders/tutorial4.vert";
const char* fsPath = ":/assets/shaders/tutorial4.frag";


std::string ReadFile(const char* filePath)
{
    // Create a QFile object
    QFile file(filePath);

    // Try to open the file in Read-Only mode
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Failed to open file:" << file.errorString();
        return "";
    }

    // Read all contents of the file into a QByteArray
    QByteArray data = file.readAll();

    // Convert QByteArray to QString and print
    QString text = QString::fromUtf8(data);

    // Close the file
    file.close();
    return text.toStdString();
}

struct Vertex{
    QVector3D pos;
    QVector3D color;

    Vertex(){}

    Vertex(float x, float y)
    {
        pos = QVector3D(x,y,0.0f);

        float red = rand()*1.f/RAND_MAX;
        float green = rand()*1.f/RAND_MAX;
        float blue = rand()*1.f/RAND_MAX;

        color = QVector3D(red,green,blue);
    }
};

void CreateVertexBuffer()
{
    Vertex vertices[19];

    //Center
    vertices[0] = Vertex(0.0f,0.0f);

    //Top row
    vertices[1] = Vertex(-1.0f,1.0f);
    vertices[2] = Vertex(-0.75f,1.0f);
    vertices[3] = Vertex(-0.5f,1.0f);
    vertices[4] = Vertex(-0.25f,1.0f);
    vertices[5] = Vertex(0.0f,1.0f);
    vertices[6] = Vertex(0.25f,1.0f);
    vertices[7] = Vertex(0.5f,1.0f);
    vertices[8] = Vertex(0.75f,1.0f);
    vertices[9] = Vertex(1.0f,1.0f);

    //Bottom row
    vertices[10] = Vertex(-1.0f,-1.0f);
    vertices[11] = Vertex(-0.75f,-1.f);
    vertices[12] = Vertex(-0.5f,-1.0f);
    vertices[13] = Vertex(-0.25f,-1.0f);
    vertices[14] = Vertex(0.0f,-1.0f);
    vertices[15] = Vertex(0.25f,-1.0f);
    vertices[16] = Vertex(0.5f,-1.0f);
    vertices[17] = Vertex(0.75f,-1.0f);
    vertices[18] = Vertex(1.0f,-1.0f);

    QOpenGLExtraFunctions *f = QOpenGLContext::currentContext()->extraFunctions();

    f->glGenBuffers(1,&VBO);
    f->glBindBuffer(GL_ARRAY_BUFFER,VBO);
    f->glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);
}



void CreateIndexBuffer()
{
    unsigned int indices[] = {
        //Top triangle
        0,1,2,
        0,2,3,
        0,3,4,
        0,4,5,
        0,5,6,
        0,6,7,
        0,7,8,
        0,8,9,
        0,9,18,
        0,10,11,
        0,11,12,
        0,12,13,
        0,13,14,
        0,14,15,
        0,15,16,
        0,16,17,
        0,17,18,
        0,1,10
    };

    QOpenGLExtraFunctions *f = QOpenGLContext::currentContext()->extraFunctions();

    f->glGenBuffers(1,&IBO);
    f->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    f->glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
}

void AddShader(GLuint ShaderProgram, const char* pShaderText, GLenum ShaderType)
{
    QOpenGLExtraFunctions *f = QOpenGLContext::currentContext()->extraFunctions();
    GLuint shaderObj = f->glCreateShader(ShaderType);
    if (shaderObj == 0)
        exit(0);

    const GLchar* p[1];
    p[0] = pShaderText;

    GLint lengths[1];
    lengths[0] = strlen(pShaderText);

    f->glShaderSource(shaderObj,1,p,lengths);
    f->glCompileShader(shaderObj);

    GLint success;
    f->glGetShaderiv(shaderObj,GL_COMPILE_STATUS,&success);

    if (!success){
        GLchar infoLog[1024];
        f->glGetShaderInfoLog(shaderObj,1024,NULL,infoLog);
        qDebug()<<infoLog;
        exit(1);
    }

    f->glAttachShader(ShaderProgram,shaderObj);
}

void CompileShaders()
{
    QOpenGLExtraFunctions *f = QOpenGLContext::currentContext()->extraFunctions();
    GLuint shaderProgram = f->glCreateProgram();

    if(shaderProgram == 0)
        exit(0);

    std::string vs,fs;
    vs = ReadFile(vsPath);
    qDebug()<<vs;
    fs = ReadFile(fsPath);
    qDebug()<<fs;

    //add vertex shader
    AddShader(shaderProgram,vs.c_str(),GL_VERTEX_SHADER);
    //add fragment shader
    AddShader(shaderProgram,fs.c_str(),GL_FRAGMENT_SHADER);

    GLint success = 0;
    GLchar errorLog[1024] = {0};

    f->glLinkProgram(shaderProgram);
    f->glGetProgramiv(shaderProgram,GL_LINK_STATUS,&success);
    if (!success){
        f->glGetProgramInfoLog(shaderProgram,sizeof(errorLog),NULL,errorLog);
        qDebug()<<errorLog;
        exit(1);
    }

    gTranslation = f->glGetUniformLocation(shaderProgram,"gTranslation");

    f->glValidateProgram(shaderProgram);
    f->glGetProgramiv(shaderProgram,GL_VALIDATE_STATUS,&success);
    if (!success){
        f->glGetProgramInfoLog(shaderProgram,sizeof(errorLog),NULL,errorLog);
        qDebug()<<errorLog;
        exit(1);
    }

    f->glUseProgram(shaderProgram);
}

MainWindow::MainWindow()
{
    setGeometry(QStyle::alignedRect(
        Qt::LeftToRight,
        Qt::AlignCenter,
        QSize(500,500),
        QApplication::primaryScreen()->availableGeometry()
        ));

    if(m_timer == NULL){
        m_timer = new QTimer(this);
        m_timer->setInterval(1000/FPS);
        connect(m_timer,&QTimer::timeout,[this]{
            update();
        });
        m_timer->start();
    }
}

MainWindow::~MainWindow()
{

}

void MainWindow::initializeGL()
{
    qDebug()<<__FUNCTION__;
    QOpenGLExtraFunctions *f = QOpenGLContext::currentContext()->extraFunctions();

    f->glClearColor(0, 0, 0, 1);
    f->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    //
    CreateVertexBuffer();
    CreateIndexBuffer();
    CompileShaders();

}

float red = 0.f,green = 0.f,blue = 0.f;

void MainWindow::paintGL()
{
    // qDebug()<<__FUNCTION__;
    QOpenGLExtraFunctions *f = QOpenGLContext::currentContext()->extraFunctions();

    f->glClear(GL_COLOR_BUFFER_BIT);

    static float Scale = 0.0f;
    static float Delta = 0.01f;
    // Scale += Delta;
    if (Scale >= 2*M_PI)
        Scale -= 2*M_PI;

    float cosA = cos(Scale);
    float sinA = sin(Scale);

    QMatrix4x4 transaltion(
        cosA,-sinA,0,0,
        sinA,cosA,0,0,
        0,0,1,0,
        0,0,0,1
    );

    f->glUniformMatrix4fv(gTranslation,1,GL_TRUE,transaltion.constData()); // GL_TRUE because the matrix is row major order

    f->glBindBuffer(GL_ARRAY_BUFFER,VBO);
    f->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,IBO);

    //position
    f->glEnableVertexAttribArray(0);//reference with layout (location = 0) in .vert file
    f->glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,6*sizeof(float),0); // vec3 pos and vec3 color

    //color
    f->glEnableVertexAttribArray(1);// reference with 'layout (location = 1) in .vert file'
    f->glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,6*sizeof(float),(void*)(3*sizeof(float)));// 3*sizeof(float) is offset to first float of color

    f->glDrawElements(GL_TRIANGLES,18*3,GL_UNSIGNED_INT,0);

    f->glDisableVertexAttribArray(0);
    f->glDisableVertexAttribArray(1);
}

void MainWindow::resizeGL(int w, int h)
{
    qDebug()<<__FUNCTION__;
}


