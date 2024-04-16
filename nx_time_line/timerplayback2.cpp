#include "timerplayback2.h"

#include <QQuickItem>

class TimerPlayback2::Private {
public:
    Private(TimerPlayback2 *owner):
        q(owner)
    {

    }

    TimerPlayback2 *q;
    std::unique_ptr<QQuickWidget> widget{new QQuickWidget()};
};

TimerPlayback2::TimerPlayback2(QObject *parent, bool isInit):
    base_type(QUrl("qrc:/qml/TimerPlayback2.qml"),parent),
    d(new Private(this))
{

}

TimerPlayback2::~TimerPlayback2()
{

}

QQuickWidget* TimerPlayback2::widget() const
{
    return d->widget.get();
}

QObject* TimerPlayback2::rootObject() const
{
    return widget()->rootObject();
}

void TimerPlayback2::registerQmlType()
{

}

void TimerPlayback2::setUpQmlUrl(const QUrl &componentUrl)
{

}
