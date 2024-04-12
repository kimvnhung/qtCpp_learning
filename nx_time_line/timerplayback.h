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
    Q_PROPERTY(double ruleX READ ruleX WRITE setRuleX NOTIFY ruleXChanged FINAL)
    Q_PROPERTY(double viewX READ viewX WRITE setViewX NOTIFY viewXChanged FINAL)
    Q_PROPERTY(qint64 curPos READ curPos WRITE setCurPos NOTIFY curPosChanged FINAL)
    using base_type = QmlWidget;
public:
    explicit TimerPlayback(QObject *parent = nullptr,bool isInit = true);
    ~TimerPlayback();
    void setDuration(std::chrono::milliseconds duration);

    QQmlListProperty<RuleLine> ruleLines();
    double ruleWidth() const;
    void setRuleWidth(double width);

    double ruleX() const;
    void setRuleX(double value);

    double viewWidth() const;
    void setViewWidth(double value);

    double viewX() const;
    void setViewX(double value);

    qint64 curPos() const;
    void setCurPos(qint64 value);

    QQuickWidget* widget() const override;
signals:
    void ruleLinesChanged();
    void ruleWidthChanged();
    void viewWidthChanged();
    void viewXChanged();
    void curPosChanged();
    void ruleXChanged();

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
