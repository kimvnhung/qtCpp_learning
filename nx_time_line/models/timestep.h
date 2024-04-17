#ifndef TIMESTEP_H
#define TIMESTEP_H

#include <QObject>
#include <QQmlListProperty>
#include "ruleline.h"
#include "rulercontext.h"

class TimeStep : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int lineType READ lineType NOTIFY lineTypeChanged FINAL)
    Q_PROPERTY(qint64 value READ value NOTIFY valueChanged FINAL)
    Q_PROPERTY(QQmlListProperty<TimeStep> subItems READ subItems NOTIFY subItemsChanged FINAL)
public:
    explicit TimeStep(RulerContext* ctx);

    RulerContext* context();

    QQmlListProperty<TimeStep> subItems();

    int lineType() const;
    qint64 value() const;

signals:
    void subItemsChanged();
    void valueChanged();
    void lineTypeChanged();

private:
    qint64 m_offset;
    qint64 m_unit;
    RuleLine::RuleLineType m_lineType;
    QList<TimeStep*> m_subItems;
    RulerContext *m_context = NULL;
};

#endif // TIMESTEP_H
