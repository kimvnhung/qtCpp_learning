#ifndef TIMERPLAYBACK_H
#define TIMERPLAYBACK_H

#include "models/ruleline.h"
#include "models/ruler.h"
#include "models/timestep.h"
#include "qmlwidget.h"

#include <QObject>
#include <QThread>

class TimerPlayback : public QmlWidget
{
    Q_OBJECT
    Q_PROPERTY(QQmlListProperty<RuleLine> ruleLines READ ruleLines NOTIFY ruleLinesChanged FINAL)
    Q_PROPERTY(double ruleWidth READ ruleWidth WRITE setRuleWidth NOTIFY ruleWidthChanged FINAL)
    Q_PROPERTY(double viewWidth READ viewWidth WRITE setViewWidth NOTIFY viewWidthChanged FINAL)
    // Q_PROPERTY(double ruleX READ ruleX WRITE setRuleX NOTIFY ruleXChanged FINAL)
    Q_PROPERTY(double viewX READ viewX WRITE setViewX NOTIFY viewXChanged FINAL)
    Q_PROPERTY(qint64 curPos READ curPos WRITE setCurPos NOTIFY curPosChanged FINAL)
    // Q_PROPERTY(Ruler ruler READ ruler NOTIFY rulerChanged FINAL)
    Q_PROPERTY(double mouseX READ mouseX WRITE setMouseX NOTIFY mouseXChanged FINAL)
    Q_PROPERTY(QQmlListProperty<LineData> lineDatas READ lineDatas NOTIFY lineDatasChanged FINAL)
    Q_PROPERTY(QQmlListProperty<TimeStep> timeSteps READ timeSteps NOTIFY timeStepsChanged FINAL)

    using base_type = QmlWidget;
public:
    explicit TimerPlayback(QObject *parent = nullptr,bool isInit = true);
    ~TimerPlayback();
    void setDuration(std::chrono::milliseconds duration);

    QQmlListProperty<RuleLine> ruleLines();
    QQmlListProperty<LineData> lineDatas();
    QQmlListProperty<TimeStep> timeSteps();

    double mouseX() const;
    void setMouseX(double value);

    double ruleWidth() const;
    void setRuleWidth(double width);

    // double ruleX() const;
    // void setRuleX(double value);

    double viewWidth() const;
    void setViewWidth(double value);

    double viewX() const;
    void setViewX(double value);

    qint64 curPos() const;
    void setCurPos(qint64 value);

    // Ruler* ruler() const;

    QQuickWidget* widget() const override;


signals:
    void ruleLinesChanged();
    void ruleWidthChanged();
    void viewWidthChanged();
    void viewXChanged();
    void curPosChanged();
    // void ruleXChanged();
    // void rulerChanged();
    void lineDatasChanged();
    void mouseXChanged();
    void timeStepsChanged();

public slots:
    double typeDistance(RuleLine::RuleLineType type);
    void generateLineData();

    // QmlWidget interface
protected:
    void registerQmlType() override;
    void setUpQmlUrl(const QUrl &componentUrl) override;
    QObject *rootObject() const override;

private:
    class Private;
    QSharedPointer<Private> d;
    QThread background;
};

#endif // TIMERPLAYBACK_H
