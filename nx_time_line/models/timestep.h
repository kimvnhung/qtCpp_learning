#ifndef TIMESTEP_H
#define TIMESTEP_H

#include <QObject>
#include <QQmlListProperty>
#include <QQmlProperty>
#include "ruleline.h"
#include "rulercontext.h"

class TimeStep : public QObject
{
    Q_OBJECT
    Q_PROPERTY(RulerContext* context READ context NOTIFY contextChanged FINAL)
    Q_PROPERTY(int lineType READ lineType NOTIFY lineTypeChanged FINAL)
    Q_PROPERTY(qint64 value READ value NOTIFY valueChanged FINAL)
    Q_PROPERTY(qint64 unit READ unit NOTIFY unitChanged FINAL)
    Q_PROPERTY(double relativeWidth READ relativeWidth NOTIFY contextChanged FINAL)
    Q_PROPERTY(bool isDisableText READ isDisableText NOTIFY isDisableTextChanged FINAL)
    Q_PROPERTY(QQmlListProperty<TimeStep> subItems READ subItems NOTIFY subItemsChanged FINAL)
public:
    explicit TimeStep(RulerContext* ctx,
                      qint64 offset = 0,
                      qint64 unit = 0,
                      RuleLine::RuleLineType lineType = RuleLine::RuleLineType::UNDEFINED);

    RulerContext* context() const;

    QQmlListProperty<TimeStep> subItems();

    int lineType() const;
    void setLineType(int value);

    qint64 value() const;
    qint64 unit() const;
    double width() const;
    double relativeWidth() const;
    bool isDisableText() const;


signals:
    void subItemsChanged();
    void valueChanged();
    void lineTypeChanged();
    void contextChanged();
    void unitChanged();
    void widthChanged();
    void isDisableTextChanged();

public slots:
    void onHighestUnitChanged();

private:
    class Private;
    QSharedPointer<Private> d;
};

#endif // TIMESTEP_H
