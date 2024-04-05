#include "timerplayback.h"
#include "models/ruleline.h"

#include <QQmlContext>
#include <QQuickItem>

using namespace std::chrono;
class TimerPlayback::Private{
public:
    Private(TimerPlayback *parent):
        initWidth(0),
        initHeight(0),
        width(0),
        height(0),
        duration(std::chrono::milliseconds(0))
    {
        //demo init
        ruleLines.append(new RuleLine(parent,RuleLine::HIGHEST,"1h",1h));
        ruleLines.append(new RuleLine(parent,RuleLine::NORMAL,"1h",1h));
        ruleLines.append(new RuleLine(parent,RuleLine::SMALL,"1h",1h));
        ruleLines.append(new RuleLine(parent,RuleLine::SMALLEST,"1h",1h));
        ruleLines.append(new RuleLine(parent,RuleLine::HIGHEST,"1h",1h));
        ruleLines.append(new RuleLine(parent,RuleLine::NORMAL,"1h",1h));
        ruleLines.append(new RuleLine(parent,RuleLine::SMALL,"1h",1h));
        ruleLines.append(new RuleLine(parent,RuleLine::SMALLEST,"1h",1h));
    }

    std::unique_ptr<QQuickWidget> widget{new QQuickWidget()};

    double width,height;
    double initWidth,initHeight;



    QList<RuleLine*> ruleLines;
    std::chrono::milliseconds duration;

    void updateRuleLines();
    bool setWidth(double width);
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
    qmlRegisterType<TimerPlayback>("models",1,0,"TimerPlaybackModel");
    widget()->rootContext()->setContextProperty(QString("instance"),this);
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

QQmlListProperty<RuleLine> TimerPlayback::ruleLines()
{
    return QQmlListProperty<RuleLine>(this,&d->ruleLines);
}

double TimerPlayback::ruleWidth() const
{
    return d->width;
}

void TimerPlayback::setRuleWidth(double width)
{
    if(d->setWidth(width))
        emit ruleWidthChanged();
}

bool TimerPlayback::Private::setWidth(double w)
{
    qDebug()<<"width "<<w;
    if(!initWidth)
        initWidth = w;

    if(width == w)
        return false;

    updateRuleLines();

    width = w;
    return true;
}

void TimerPlayback::Private::updateRuleLines()
{

}

