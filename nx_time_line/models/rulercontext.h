#ifndef RULERCONTEXT_H
#define RULERCONTEXT_H

#include "ruleline.h"

#include <QMutex>
#include <QObject>

class RulerContext : public QObject
{
    Q_OBJECT
    Q_PROPERTY(double widthPerMili READ widthPerMili NOTIFY widthPerMiliChanged FINAL)
public:
    explicit RulerContext(QObject *parent = nullptr,
                          qint64 totalTime = 0,
                          qint64 highestUnit = 0,
                          double width = 0,
                          double x = 0);
    ~RulerContext();

    double widthPerMili() const;
    void setWidthPerMili(double value);


    qint64 totalTime() const;
    void setTotalTime(qint64 newTotalTime);

    double width() const;
    void setWidth(double newWidth);

    double x() const;
    void setX(double newX);

    qint64 normalUnit() const;
    void setNormalUnit(qint64 newNormalUnit);

    qint64 smallUnit() const;
    void setSmallUnit(qint64 newSmallUnit);

    qint64 smallestUnit() const;
    void setSmallestUnit(qint64 newSmallestUnit);

    qint64 highestUnit() const;
    void setHighestUnit(qint64 newHighestUnit);

    double visibleWidth() const;
    void setVisibleWidth(double newVisibleWidth);

    bool isVisible(qint64 value);

    double relativeWidth(qint64 value);
    qint64 startValueByUnit(qint64 unit);
    qint64 stopValueByUnit(qint64 unit);
    qint64 absoluteStart() const;
    qint64 absoluteStop() const;
    double startPosition() const;
    double stopPosition() const;

signals:
    void widthPerMiliChanged();
    void widthChanged();
    void xChanged();
    void totalTimeChanged();
    void unitsChanged();


private:
    qint64 m_totalTime;
    qint64 m_highestUnit;
    qint64 m_normalUnit;
    qint64 m_smallUnit;
    qint64 m_smallestUnit;
    double m_width;
    double m_widthPerMili;
    double m_x;
    double m_visibleWidth;
    qint64 m_visibleRange[2];
    qint64 m_absoluteVisibleRange[2];

    void updateUnits();
    bool isRoundedBy(qint64 target, qint64 unit);

    qint64 roundedBy(qint64 target, qint64 unit);
    void refreshVisibleRange();

};

#endif // RULERCONTEXT_H
