#include "timerplayback.h"
#include "models/ruleline.h"
#include "models/ruler.h"

#include <QQmlContext>
#include <QQuickItem>

using namespace std::chrono;

double HIGHEST_VISABLE_W = 50;

class TimerPlayback::Private
{
public:
    Private(TimerPlayback *parent) :
        q(parent),
        initWidth(0),
        initHeight(0),
        width(0),
        height(0),
        viewWidth(0),
        viewX(0),
        curPos(0),
        duration(std::chrono::milliseconds(0)),
        totalTime(10000),
        widthPerMili(0)
    {
        // demo init
    }
    TimerPlayback *q;

    std::unique_ptr<QQuickWidget> widget{new QQuickWidget()};

    double width, height;
    double initWidth, initHeight;
    double viewWidth;
    double viewX;
    qint64 curPos;

    QList<RuleLine *> ruleLines;
    QList<LineData *> lineDatas;
    std::chrono::milliseconds duration;
    qint64 totalTime;
    double widthPerMili;

    template <typename Rep, typename Period>
    bool is_round(const std::chrono::duration<Rep, Period> &duration, std::chrono::duration<Rep, Period> unit);

    void updateRuleLines();
    void updateLineDatas();
    bool setWidth(double width);
    bool setViewWidth(double value);
    bool setViewX(double value);
    bool setCurPos(qint64 value);
};

TimerPlayback::~TimerPlayback()
{
    qDebug()<<__FUNCTION__;
    d.clear();
}

TimerPlayback::TimerPlayback(QObject *parent, bool isInit) : base_type(QUrl("qrc:/qml/TimerPlayback.qml"), parent),
                                                             d(new Private(this))
{
    if (isInit)
    {
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
    qmlRegisterType<Ruler>("models",1,0,"Ruler");
    qmlRegisterUncreatableType<RuleLine>("models", 1, 0, "RuleLine", "Cannot create RuleLine in QML");
    widget()->rootContext()->setContextProperty(QString("instance"), this);
}

void TimerPlayback::setUpQmlUrl(const QUrl &componentUrl)
{
    widget()->setObjectName("TimerPlayback");
    widget()->setResizeMode(QQuickWidget::SizeRootObjectToView);
    widget()->setSource(componentUrl);
}

QQuickWidget *TimerPlayback::widget() const
{
    return d->widget.get();
}

QObject *TimerPlayback::rootObject() const
{
    return widget()->rootObject();
}

QQmlListProperty<RuleLine> TimerPlayback::ruleLines()
{
    return QQmlListProperty<RuleLine>(this, &d->ruleLines);
}

double TimerPlayback::ruleWidth() const
{
    return d->width;
}

void TimerPlayback::setRuleWidth(double width)
{
    if (d->setWidth(width))
        emit ruleWidthChanged();
}

// double TimerPlayback::ruleX() const
// {
//     return d->ruler->x();
// }

// void TimerPlayback::setRuleX(double value)
// {
//     d->ruler->setX(value);
//     emit ruleXChanged();
// }

QQmlListProperty<LineData> TimerPlayback::lineDatas()
{
    return QQmlListProperty<LineData>(this,&d->lineDatas);
}


double TimerPlayback::viewWidth() const
{
    return d->viewWidth;
}

void TimerPlayback::setViewWidth(double value)
{
    if (d->setViewWidth(value))
        emit viewWidthChanged();
}

double TimerPlayback::viewX() const
{
    return d->viewX;
}

void TimerPlayback::setViewX(double value)
{
    if (d->setViewX(value))
        emit viewXChanged();
}

void TimerPlayback::setCurPos(qint64 value)
{
    if(d->setCurPos(value))
        emit curPosChanged();
}

qint64 TimerPlayback::curPos() const
{
    return d->curPos;
}

bool TimerPlayback::Private::setWidth(double w)
{
    if (!w)
        return false;

    if (!initWidth)
        initWidth = w;

    if (width == w)
        return false;

    width = w;
    // updateRuleLines();
    updateLineDatas();
    return true;
}

bool TimerPlayback::Private::setViewWidth(double value)
{
    if (value == 0)
        return false;

    if (viewWidth == value)
        return false;

    viewWidth = value;
    // updateRuleLines();
    updateLineDatas();
    return true;
}

bool TimerPlayback::Private::setViewX(double value)
{
    qDebug() << __FUNCTION__ << "x : " << value;
    if (value == 0)
        return false;

    if (viewX == value)
        return false;

    viewX = value;
    // updateRuleLines();
    updateLineDatas();
    return true;
}

bool TimerPlayback::Private::setCurPos(qint64 value)
{
    if(curPos == value)
        return false;

    curPos = value;
    return true;
}

// Ruler* TimerPlayback::ruler() const
// {
//     return d->ruler;
// }

std::chrono::milliseconds roundByUnit(std::chrono::milliseconds duration, std::chrono::milliseconds unit)
{
    int divided = duration.count() / unit.count();
    return std::chrono::milliseconds(divided * unit.count());
}

void TimerPlayback::Private::updateRuleLines()
{
    // for test other
    if (1)
        return;

    if (!width)
        return;

    double widthPerMiliSecond = width / duration.count();
    int highestCount = 0;

    std::chrono::milliseconds highestUnit = std::chrono::milliseconds(100);
    std::chrono::milliseconds normalUnit, smallUnit, smallestUnit;
    // if((100ms).count()*widthPerMiliSecond > HIGHEST_VISABLE_W){
    //     //Minimum Smallest
    // }else if((5*100ms).count()*widthPerMiliSecond > HIGHEST_VISABLE_W){
    //     //Minimum Small
    //     highestUnit = 500ms;
    // }else if((1000ms).count()*widthPerMiliSecond > HIGHEST_VISABLE_W){
    //     //Minimum Normal
    //     highestUnit = 1000ms;
    // }else
    if (duration_cast<milliseconds>(5s).count() * widthPerMiliSecond > HIGHEST_VISABLE_W)
    {
        // Minimum Highest
        highestUnit = 5s;
        normalUnit = 1s;
        smallUnit = 500ms;
        smallestUnit = 100ms;
    }
    else if (duration_cast<milliseconds>(10s).count() * widthPerMiliSecond > HIGHEST_VISABLE_W)
    {
        highestUnit = 10s;
        normalUnit = 5s;
        smallUnit = 1s;
        smallestUnit = 500ms;
    }
    else if (duration_cast<milliseconds>(30s).count() * widthPerMiliSecond > HIGHEST_VISABLE_W)
    {
        highestUnit = 30s;
        normalUnit = 10s;
        smallUnit = 5s;
        smallestUnit = 1s;
    }
    else if (duration_cast<milliseconds>(1min).count() * widthPerMiliSecond > HIGHEST_VISABLE_W)
    {
        highestUnit = 1min;
        normalUnit = 30s;
        smallUnit = 10s;
        smallestUnit = 5s;
    }
    else if (duration_cast<milliseconds>(5min).count() * widthPerMiliSecond > HIGHEST_VISABLE_W)
    {
        highestUnit = 5min;
        normalUnit = 1min;
        smallUnit = 30s;
        smallestUnit = 10s;
    }
    else if (duration_cast<milliseconds>(10min).count() * widthPerMiliSecond > HIGHEST_VISABLE_W)
    {
        highestUnit = 10min;
        normalUnit = 5min;
        smallUnit = 1min;
        smallestUnit = 30s;
    }
    else if (duration_cast<milliseconds>(30min).count() * widthPerMiliSecond > HIGHEST_VISABLE_W)
    {
        highestUnit = 30min;
        normalUnit = 10min;
        smallUnit = 5min;
        smallestUnit = 1min;
    }
    else if (duration_cast<milliseconds>(1h).count() * widthPerMiliSecond > HIGHEST_VISABLE_W)
    {
        highestUnit = 1h;
        normalUnit = 30min;
        smallUnit = 10min;
        smallestUnit = 5min;
    }
    else if (duration_cast<milliseconds>(3h).count() * widthPerMiliSecond > HIGHEST_VISABLE_W)
    {
        highestUnit = 3h;
        normalUnit = 1h;
        smallUnit = 30min;
        smallestUnit = 10min;
    }

    int totalCount = duration / std::chrono::milliseconds(100);
    highestCount = duration / highestUnit;

    // case still not initialized
    if (ruleLines.length() < totalCount)
    {
        ruleLines.clear();

        std::chrono::milliseconds startValue = roundByUnit(std::chrono::milliseconds(static_cast<int>(viewX / widthPerMiliSecond)), smallestUnit);
        std::chrono::milliseconds stopValue = roundByUnit(std::chrono::milliseconds(static_cast<int>((viewX + viewWidth) / widthPerMiliSecond)), smallestUnit);
        // smallest point
        std::chrono::milliseconds counter = startValue;
        while (counter <= stopValue)
        {
            qDebug() << __FUNCTION__ << __LINE__ << counter;
            RuleLine *newLine = new RuleLine(q, RuleLine::RuleLineType::SMALLEST, counter);
            newLine->setPosition(counter.count() * widthPerMiliSecond);
            ruleLines.append(newLine);
            counter += smallestUnit;
        }

        startValue = roundByUnit(startValue, highestUnit);
        stopValue = roundByUnit(stopValue, highestUnit);
        counter = startValue;
        while (counter <= stopValue)
        {
            qDebug() << __FUNCTION__ << __LINE__;
            RuleLine *newLine = new RuleLine(q, RuleLine::RuleLineType::HIGHEST, counter);
            newLine->setPosition(counter.count() * widthPerMiliSecond);
            ruleLines.append(newLine);
            counter += highestUnit;
        }
    }
    else
    {
        // case alread initialized
    }
    emit q->ruleLinesChanged();
}

void TimerPlayback::Private::updateLineDatas()
{
    qDebug()<<"totalTime: "<<totalTime<<"ruleWidth: "<<width<<"; viewWidth: "<<viewWidth<<"; viewX: "<<viewX;
    if(!width)
        return;

    if(!viewWidth)
        return;

    widthPerMili = width/totalTime;

    lineDatas.append(new LineData(q,RuleLine::RuleLineType::HIGHEST,1000,true));
    emit q->lineDatasChanged();

    for(int i=0; i < lineDatas.length(); i++){
        lineDatas[i]->setPosition(widthPerMili*lineDatas[i]->value());
    }

}

double TimerPlayback::typeDistance(RuleLine::RuleLineType type)
{
    double startPos = 0;
    for (int i = 0; i < d->ruleLines.size(); i++)
    {
        if (d->ruleLines[i]->type() == type)
        {
            if (startPos == 0)
                startPos = d->ruleLines[i]->position();
            else
                return d->ruleLines[i]->position() - startPos;
        }
    }

    return 40; // default
}
