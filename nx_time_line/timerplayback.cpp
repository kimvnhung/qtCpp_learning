#include "timerplayback.h"

#include <QQuickItem>

class TimerPlayback::Private{
public:
    Private(TimerPlayback *parent)
    {

    }

    std::unique_ptr<QQuickWidget> widget{new QQuickWidget()};
};

TimerPlayback::TimerPlayback(QObject *parent, bool isInit):
    base_type(QUrl("qrc:/qml/TimerPlayback.qml"),parent),
    d(new Private(this))
{
    if(isInit){
        init();
    }
}

void TimerPlayback::registerQmlType()
{

}

void TimerPlayback::setUpQmlUrl(const QUrl& componentUrl)
{
    widget()->setObjectName("TimerPlayback");
    widget()->setResizeMode(QQuickWidget::SizeRootObjectToView);
    widget()->setSource(componentUrl);
}

QQuickWidget* TimerPlayback::widget() const
{
    return d->widget.get();
}

QObject* TimerPlayback::rootObject() const
{
    return widget()->rootObject();
}

