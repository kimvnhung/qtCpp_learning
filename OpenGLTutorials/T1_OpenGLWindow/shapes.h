#ifndef SHAPES_H
#define SHAPES_H

#include <QVector3D>



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


#define CIRCLE_SMOOTH 100
class Circle
{
public:

    Circle(){}

    Circle(float x,float y,float radius = 0.5)
    {
        vertices[0] = Vertex(0,0);

        float alpha = 2*M_PI/CIRCLE_SMOOTH;
        for(int i=0;i<CIRCLE_SMOOTH;i++){
            vertices[i+1] = Vertex(radius*cos(i*alpha),radius*sin(i*alpha));
        }

        for(int i=0;i<CIRCLE_SMOOTH;i++){
            indices[3*i] = 0;
            indices[3*i+1] = i+1;
            indices[3*i+2] = i+2;
        }
        indices[CIRCLE_SMOOTH*3] = 0;
        indices[CIRCLE_SMOOTH*3+1] = CIRCLE_SMOOTH;
        indices[CIRCLE_SMOOTH*3+2] = 1;
    }

    Vertex vertices[CIRCLE_SMOOTH+1];

    unsigned int indices[(CIRCLE_SMOOTH+1)*3];

    int indicesCount(){return (CIRCLE_SMOOTH+1)*3;}
};



#endif // SHAPES_H
