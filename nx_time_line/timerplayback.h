#ifndef TIMERPLAYBACK_H
#define TIMERPLAYBACK_H

#include "models/ruleline.h"
#include "qmlwidget.h"

#include <QObject>

class TimerPlayback : public QmlWidget
{
    Q_OBJECT
    Q_PROPERTY(QQmlListProperty<RuleLine> ruleLines READ ruleLines NOTIFY ruleLinesChanged FINAL)
    Q_PROPERTY(double ruleWidth READ ruleWidth WRITE setRuleWidth NOTIFY ruleWidthChanged FINAL)
    using base_type = QmlWidget;
public:
    explicit TimerPlayback(QObject *parent = nullptr,bool isInit = true);

    QQmlListProperty<RuleLine> ruleLines();
    double ruleWidth() const;
    void setRuleWidth(double width);

    QQuickWidget* widget() const override;
signals:
    void ruleLinesChanged();
    void ruleWidthChanged();

    // QmlWidget interface
protected:
    void registerQmlType() override;
    void setUpQmlUrl(const QUrl &componentUrl) override;
    QObject *rootObject() const override;

private:
    class Private;
    QSharedPointer<Private> d;
};

#endif // TIMERPLAYBACK_H
