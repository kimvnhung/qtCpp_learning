#ifndef GRAPHITEM_H
#define GRAPHITEM_H

#include <QObject>

class GraphItem : public QObject
{
    Q_OBJECT
    using base_type = QObject;
public:
    GraphItem(int x = 0, int y = 0, QObject *parent = NULL);
    virtual ~GraphItem() = 0;

    static int DISTANCE_UNIT;

    int xUnit() const;
    int yUnit() const;

    void setX(int x);
    void setY(int y);
    void setCordinate(int x, int y);

    QRectF rect() const;
signals:
    void xChanged();
    void yChanged();
private:
    int m_x;
    int m_y;
};

#endif // GRAPHITEM_H
