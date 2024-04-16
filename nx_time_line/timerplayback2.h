#ifndef TIMERPLAYBACK2_H
#define TIMERPLAYBACK2_H

#include "qmlwidget.h"

#include <QObject>

class TimerPlayback2 : public QmlWidget
{
    Q_OBJECT

    using base_type = QmlWidget;
public:
    explicit TimerPlayback2(QObject *parent = nullptr, bool isInit = true);
    ~TimerPlayback2();

signals:


private:
    class Private;
    QSharedPointer<Private> d;

    // QmlWidget interface
public:
    QQuickWidget *widget() const override;

protected:
    void registerQmlType() override;
    void setUpQmlUrl(const QUrl &componentUrl) override;
    QObject *rootObject() const override;
};

#endif // TIMERPLAYBACK2_H
