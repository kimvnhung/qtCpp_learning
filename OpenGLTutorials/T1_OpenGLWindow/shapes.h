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


class IShape {
public:
    virtual Vertex* getVertices() {return vertices;}
    virtual unsigned int* getIndices() {return indices;}
    virtual int indicesCount() = 0;
    virtual int verticesCount() = 0;
protected:
    Vertex *vertices;
    unsigned int *indices;
};

#define CIRCLE_SMOOTH 100
class Circle : public IShape
{
public:

    Circle(){}

    Circle(float x,float y,float radius = 0.5)
    {
        vertices = (Vertex*)malloc((CIRCLE_SMOOTH+1)*sizeof(Vertex));

        vertices[0] = Vertex(x,y);

        float alpha = 2*M_PI/CIRCLE_SMOOTH;
        for(int i=0;i<CIRCLE_SMOOTH;i++){
            vertices[i+1] = Vertex(radius*cos(i*alpha)+x,radius*sin(i*alpha)+y);
        }

        indices = (unsigned int*)malloc((CIRCLE_SMOOTH+1)*3*sizeof(unsigned int));
        for(int i=0;i<CIRCLE_SMOOTH;i++){
            indices[3*i] = 0;
            indices[3*i+1] = i+1;
            indices[3*i+2] = i+2;
        }
        indices[CIRCLE_SMOOTH*3] = 0;
        indices[CIRCLE_SMOOTH*3+1] = CIRCLE_SMOOTH;
        indices[CIRCLE_SMOOTH*3+2] = 1;
    }

    int indicesCount() override {return (CIRCLE_SMOOTH/2)*3;}
    int verticesCount() override {return (CIRCLE_SMOOTH+1);}

};

class Star: public IShape{
    Star(float x,float y,float radius = 0.5)
    {
        int numPoints = 10;
        vertices = (Vertex*)malloc(numPoints*sizeof(Vertex));
        indices = (unsigned int*)malloc((CIRCLE_SMOOTH+1)*3*sizeof(unsigned int));

        vertices[0] = Vertex(x,y);

        float alpha = 2*M_PI/CIRCLE_SMOOTH;
        for(int i=0;i<CIRCLE_SMOOTH;i++){
            vertices[i+1] = Vertex(radius*cos(i*alpha)+x,radius*sin(i*alpha)+y);
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
};


#endif // SHAPES_H
