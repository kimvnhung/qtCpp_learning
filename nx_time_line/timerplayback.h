#ifndef TIMERPLAYBACK_H
#define TIMERPLAYBACK_H

#include "qmlwidget.h"

#include <QObject>

class TimerPlayback : public QmlWidget
{
    Q_OBJECT
    using base_type = QmlWidget;
public:
    explicit TimerPlayback(QObject *parent = nullptr,bool isInit = true);


    QQuickWidget* widget() const override;
signals:

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
