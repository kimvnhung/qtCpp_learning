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
    Q_PROPERTY(double viewWidth READ viewWidth WRITE setViewWidth NOTIFY viewWidthChanged FINAL)
    Q_PROPERTY(double viewX READ viewX WRITE setViewX NOTIFY viewXChanged FINAL)
    using base_type = QmlWidget;
public:
    explicit TimerPlayback(QObject *parent = nullptr,bool isInit = true);
    void setDuration(std::chrono::milliseconds duration);

    QQmlListProperty<RuleLine> ruleLines();
    double ruleWidth() const;
    void setRuleWidth(double width);

    double viewWidth() const;
    void setViewWidth(double value);

    double viewX() const;
    void setViewX(double value);

    QQuickWidget* widget() const override;
signals:
    void ruleLinesChanged();
    void ruleWidthChanged();
    void viewWidthChanged();
    void viewXChanged();

public slots:
    double typeDistance(RuleLine::RuleLineType type);

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
