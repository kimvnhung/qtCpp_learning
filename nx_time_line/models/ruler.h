#ifndef RULER_H
#define RULER_H

#include "linedata.h"
#include <QObject>
#include <QQmlListProperty>

class Ruler : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QQmlListProperty<LineData> lineDatas READ lineDatas NOTIFY lineDatasChanged FINAL)
    Q_PROPERTY(double x READ x WRITE setX NOTIFY xChanged FINAL)
    Q_PROPERTY(double width READ width WRITE setWidth NOTIFY widthChanged FINAL)
    Q_PROPERTY(double visibleWidth READ visibleWidth WRITE setVisibleWidth NOTIFY visibleWidthChanged FINAL)
public:
    explicit Ruler(QObject *parent = nullptr, qint64 totalTime = 0);
    ~Ruler();

    QQmlListProperty<LineData> lineDatas();

    double x() const;
    void setX(double value);

    double width() const;
    void setWidth(double width);

    double visibleWidth() const;
    void setVisibleWidth(double visibleWidth);

signals:
    void lineDatasChanged();
    void xChanged();
    void widthChanged();
    void visibleWidthChanged();

private:
    QList<LineData*> m_lineDatas;
    double m_x,m_width,m_visibleWidth;
    qint64 m_totalTime;

    void updateLineDatas();
};

#endif // RULER_H
