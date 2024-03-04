#include "mainwindow.h"
#include "shapes.h"

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
GLuint crossVBO;
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


Circle circle(0,0,0.3);

void CreateVertexBuffer()
{
    Vertex crossVertices[4];
    crossVertices[0] = Vertex(0,1);
    crossVertices[1] = Vertex(0,-1);
    crossVertices[2] = Vertex(-1,0);
    crossVertices[3] = Vertex(1,0);

    QOpenGLExtraFunctions *f = QOpenGLContext::currentContext()->extraFunctions();

    f->glGenBuffers(1,&VBO);
    f->glBindBuffer(GL_ARRAY_BUFFER,VBO);
    f->glBufferData(GL_ARRAY_BUFFER,sizeof(Vertex)*circle.verticesCount(),circle.getVertices(),GL_STATIC_DRAW);

    f->glGenBuffers(1,&crossVBO);
    f->glBindBuffer(GL_ARRAY_BUFFER,crossVBO);
    f->glBufferData(GL_ARRAY_BUFFER,sizeof(crossVertices),crossVertices,GL_STATIC_DRAW);
}



void CreateIndexBuffer()
{

    QOpenGLExtraFunctions *f = QOpenGLContext::currentContext()->extraFunctions();

    f->glGenBuffers(1,&IBO);
    f->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    f->glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)*circle.indicesCount(), circle.getIndices(), GL_STATIC_DRAW);
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
    srand(static_cast<unsigned int>(time(nullptr)));

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

    static float angle = 0;
    angle += 5;
    if (angle >= 360)
        angle = 0;

    static float translate = 0;
    translate += 0.1;
    if (translate >= 1)
        translate = -1;

    transaltion.translate(QVector3D(translate,0.0f,0.0f));
    transaltion.rotate(angle,QVector3D(0.0f,0.0f,1.0f));


    f->glUniformMatrix4fv(gTranslation,1,GL_TRUE,transaltion.constData()); // GL_TRUE because the matrix is row major order

    f->glBindBuffer(GL_ARRAY_BUFFER,VBO);
    f->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,IBO);

    //position
    f->glEnableVertexAttribArray(0);//reference with layout (location = 0) in .vert file
    f->glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,6*sizeof(float),0); // vec3 pos and vec3 color

    //color
    f->glEnableVertexAttribArray(1);// reference with 'layout (location = 1) in .vert file'
    f->glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,6*sizeof(float),(void*)(3*sizeof(float)));// 3*sizeof(float) is offset to first float of color

    f->glDrawElements(GL_TRIANGLES,circle.indicesCount(),GL_UNSIGNED_INT,0);

    f->glDisableVertexAttribArray(0);
    f->glDisableVertexAttribArray(1);
}

void MainWindow::resizeGL(int w, int h)
{
    qDebug()<<__FUNCTION__;
}


