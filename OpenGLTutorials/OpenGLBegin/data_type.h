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


struct YUVFrame
{
    QByteArray data;
    int width = 0;
    int height = 0;
    int stride[3];

    bool isValid(){
        return width != 0 && height != 0;
    }
};
#endif // DATA_TYPE_H
