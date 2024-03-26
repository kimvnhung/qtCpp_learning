#ifndef DATA_TYPE_H
#define DATA_TYPE_H

#include <QVector3D>


struct Vertex
{
    QVector3D pos;
    QVector2D tex;

    Vertex(){}

    Vertex(const QVector3D& _pos,const QVector2D& _tex){
        pos = _pos;
        tex = _tex;
    }
};

#endif // DATA_TYPE_H
