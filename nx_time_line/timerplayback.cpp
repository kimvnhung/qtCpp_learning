#include "timerplayback.h"
#include "models/ruleline.h"

#include <QQmlContext>
#include <QQuickItem>

using namespace std::chrono;

double HIGHEST_VISABLE_W = 50;

class TimerPlayback::Private{
public:
    Private(TimerPlayback *parent):
        q(parent),
        initWidth(0),
        initHeight(0),
        width(0),
        height(0),
        duration(std::chrono::milliseconds(0))
    {
        //demo init

    }
    TimerPlayback* q;

    std::unique_ptr<QQuickWidget> widget{new QQuickWidget()};

    double width,height;
    double initWidth,initHeight;

    QList<RuleLine*> ruleLines;
    std::chrono::milliseconds duration;


    template<typename Rep, typename Period>
    bool is_round(const std::chrono::duration<Rep, Period>& duration, std::chrono::duration<Rep, Period> unit);

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

void TimerPlayback::setDuration(std::chrono::milliseconds duration)
{
    d->duration = duration;
    d->updateRuleLines();
}

void TimerPlayback::registerQmlType()
{
    qmlRegisterUncreatableType<RuleLine>("models",1,0,"RuleLine","Cannot create RuleLine in QML");
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
    qDebug()<<__FUNCTION__<<"width : "<<w;
    if(!w)
        return false;

    if(!initWidth)
        initWidth = w;

    if(width == w)
        return false;

    width = w;
    updateRuleLines();
    return true;
}

void TimerPlayback::Private::updateRuleLines()
{
    qDebug()<<__FUNCTION__;
    if(!width)
        return;

    double widthPerMiliSecond = width/duration.count();
    int highestCount = 0;
    std::chrono::milliseconds highestUnit = std::chrono::milliseconds(static_cast<int>(50/widthPerMiliSecond));

    while ((widthPerMiliSecond*highestUnit.count())<HIGHEST_VISABLE_W) {
        while (highestUnit <= duration) {
            highestUnit += 100ms;
            if (highestUnit > 3h)
            {
                qDebug()<<__FUNCTION__<<__LINE__;
                if (highestUnit.count()%(duration_cast<milliseconds>(3h).count())==0)
                    break;
            }
            else if (highestUnit > 1h)
            {
                qDebug()<<__FUNCTION__<<__LINE__;
                if (highestUnit.count()%duration_cast<milliseconds>(1h).count()==0)
                    break;
            }
            else if (highestUnit > 30min)
            {
                qDebug()<<__FUNCTION__<<__LINE__;
                if (highestUnit.count()%duration_cast<milliseconds>(30min).count()==0)
                    break;
            }
            else if (highestUnit > 10min)
            {
                qDebug()<<__FUNCTION__<<__LINE__<<"hihgestCount :"<<highestUnit.count()<<" 10ms.count :"<<duration_cast<milliseconds>(10min).count();
                if (highestUnit.count()%duration_cast<milliseconds>(10min).count()==0)
                    break;
            }
            else if (highestUnit > 5min)
            {
                qDebug()<<__FUNCTION__<<__LINE__;
                if (highestUnit.count()%(5min).count()==0)
                    break;
            }
            else if (highestUnit > 1min)
            {
                qDebug()<<__FUNCTION__<<__LINE__;
                if (highestUnit.count()%(1min).count()==0)
                    break;
            }
            else if (highestUnit > 30s)
            {
                qDebug()<<__FUNCTION__<<__LINE__;
                if (highestUnit.count()%(30s).count()==0)
                    break;
            }
            else if (highestUnit > 10s)
            {
                qDebug()<<__FUNCTION__<<__LINE__;
                if (highestUnit.count()%(10s).count()==0)
                    break;
            }
            else if (highestUnit > 30s)
            {
                qDebug()<<__FUNCTION__<<__LINE__;
                if (highestUnit.count()%(30s).count()==0)
                    break;
            }
            else if (highestUnit > 10s)
            {
                qDebug()<<__FUNCTION__<<__LINE__;
                if (highestUnit.count()%(10s).count()==0)
                    break;
            }
            else if (highestUnit > 5s)
            {
                qDebug()<<__FUNCTION__<<__LINE__;
                if (highestUnit.count()%(5s).count()==0)
                    break;
            }
            else if (highestUnit > 1s)
            {
                qDebug()<<__FUNCTION__<<__LINE__;
                if (highestUnit.count()%(1s).count()==0)
                    break;
            }
            else if (highestUnit > 500ms)
            {
                qDebug()<<__FUNCTION__<<__LINE__;
                if (highestUnit.count()%(500ms).count()==0)
                    break;
            }else {
                qDebug()<<__FUNCTION__<<__LINE__;
                if (highestUnit.count()%(100ms).count()==0)
                    break;
            }
        }
    }
    qDebug()<<"highestUnit : "<<highestUnit<<"; distance : "<<widthPerMiliSecond*highestUnit.count();

    highestCount = duration/highestUnit;

    qDebug()<<"higestCount : "<<highestCount;
    qDebug()<<"width : "<<width;
    qDebug()<<"widthPerMiliSecond : "<<widthPerMiliSecond;
    qDebug()<<"widthPerHighest : "<<duration_cast<milliseconds>(highestUnit*1).count()*widthPerMiliSecond;

    //case still not initialized
    if(ruleLines.empty()){
        for(int i=0; i< highestCount;i++){
            ruleLines.append(new RuleLine(q,RuleLine::RuleLineType::HIGHEST,highestUnit*i));
            ruleLines[i]->setPosition(duration_cast<milliseconds>(highestUnit*i).count()*widthPerMiliSecond);
        }
    }else {
         //case alread initialized
    }
    emit q->ruleLinesChanged();
}

double TimerPlayback::typeDistance(RuleLine::RuleLineType type)
{
    double startPos = 0;
    for (int i = 0; i < d->ruleLines.size(); i++) {
        if(d->ruleLines[i]->type() == type){
            if(startPos == 0)
                startPos = d->ruleLines[i]->position();
            else
                return d->ruleLines[i]->position()-startPos;
        }
    }

    return 40;// default
}

