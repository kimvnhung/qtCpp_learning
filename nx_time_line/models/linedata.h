#ifndef LINEDATA_H
#define LINEDATA_H

#include "ruleline.h"

#include <QObject>

class LineData : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int lineType READ lineType WRITE setLineType NOTIFY lineTypeChanged FINAL)
    Q_PROPERTY(qint64 value READ value WRITE setValue NOTIFY valueChanged FINAL)
    Q_PROPERTY(double position READ position NOTIFY positionChanged FINAL)
    Q_PROPERTY(double relativePosition READ relativePosition WRITE setRelativePosition NOTIFY relativePositionChanged FINAL)
public:
    explicit LineData(QObject *parent = nullptr,
                      RuleLine::RuleLineType lineType = RuleLine::RuleLineType::UNDEFINED,
                      qint64 value = 0,
                      bool visible = false,
                      int level = 0);

    int lineType() const;
    void setLineType(int value);

    qint64 value() const;
    void setValue(qint64 value);

    void setVisible(bool visible);

    double position() const;
    void setPosition(double value);

    int level() const;

    double relativePosition() const;
    void setRelativePosition(double value);

signals:
    void lineTypeChanged();
    void valueChanged();
    void positionChanged();
    void relativePositionChanged();
private:
    RuleLine::RuleLineType m_lineType;
    qint64 m_value;
    double m_position;
    double m_relativePosition;
    bool m_visible;
    int m_level;
};

#endif // LINEDATA_H
