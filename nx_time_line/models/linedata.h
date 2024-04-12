#ifndef LINEDATA_H
#define LINEDATA_H

#include "ruleline.h"

#include <QObject>

class LineData : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int lineType READ lineType WRITE setLineType NOTIFY lineTypeChanged FINAL)
    Q_PROPERTY(qint64 value READ value WRITE setValue NOTIFY valueChanged FINAL)
public:
    explicit LineData(QObject *parent = nullptr,
                      RuleLine::RuleLineType lineType = RuleLine::RuleLineType::UNDEFINED,
                      qint64 value = 0,
                      bool visible = false);

    int lineType() const;
    void setLineType(int value);

    qint64 value() const;
    void setValue(qint64 value);

    void setVisible(bool visible);
signals:
    void lineTypeChanged();
    void valueChanged();
private:
    RuleLine::RuleLineType m_lineType;
    qint64 m_value;
    bool m_visible;
};

#endif // LINEDATA_H
