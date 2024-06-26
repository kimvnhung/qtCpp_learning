#include "timerplayback.h"
#include "models/ruleline.h"
#include "models/ruler.h"

#include <QQmlContext>
#include <QQuickItem>
#include <QTime>
#include <QThread>

using namespace std::chrono;

double HIGHEST_VISABLE_W = 200;
qint64 MS_LEVELS[] = {
    5000,  // 5s
    10000, // 10s
    30000,
    60000,
    300000,
    600000,
    1800000,
    3600000,
    10800000};

int MS_LEVELS_SIZE = sizeof(MS_LEVELS) / sizeof(qint64);

int DELEGATE_STATES[][3] = {
    {2, 5, 10}, // 1s
    {2, 3, 2},  // 1any
    {5, 2, 5},  // 5s
    {5, 2, 3},  // 5min
    {2, 5, 2},  // 10any
    {3, 2, 5},  // 30any
    {3, 2, 3}   // 3h
};

class TimerPlayback::Private
{
public:
    Private(TimerPlayback *parent) : q(parent),
                                     initWidth(0),
                                     initHeight(0),
                                     width(0),
                                     height(0),
                                     viewWidth(0),
                                     viewX(0),
                                     curPos(0),
                                     mouseX(0),
                                     duration(std::chrono::milliseconds(0)),
                                     totalTime(25200000),
                                     widthPerMili(0),
                                     delegateState(DELEGATE_STATES[0]),
                                     isGenerated(false),
        ctx(new RulerContext(parent,12300000))
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
    double mouseX;

    QList<RuleLine *> ruleLines;
    QVector<LineData *> lineDatas;
    std::chrono::milliseconds duration;
    qint64 totalTime;
    double widthPerMili;
    qint64 highestUnit, normalUnit, smallUnit, smallestUnit;
    int *delegateState;
    bool isGenerated;

    template <typename Rep, typename Period>
    bool is_round(const std::chrono::duration<Rep, Period> &duration, std::chrono::duration<Rep, Period> unit);

    void updateRuleLines();
    void generateLineDatas();
    void updateLineDatas();
    bool setWidth(double width);
    bool setViewWidth(double value);
    bool setViewX(double value);
    bool setCurPos(qint64 value);
    bool setMouseX(double value);
    void refreshDelegateState();
    void updatePosition();
    QVector<qint64> existedValueAt(qint64 start, qint64 stop, int level);
    int curMissingLevel();
    int indexShouldBeAdded(qint64 newValue, int startIdx = 0, int endIdx = 0);


    /*
     * using context
     *
    */

    RulerContext* ctx;
    QVector<TimeStep*> timeSteps;
    void updateContext();
};

void TimerPlayback::Private::updateContext()
{
    qDebug()<<__FUNCTION__;
    QElapsedTimer timer;
    qint64 start = timer.elapsed();
    if(width == 0 || viewWidth == 0)
        return;

    ctx->setWidth(width);
    ctx->setX(viewX);
    ctx->setVisibleWidth(viewWidth);


    if(timeSteps.empty())
    {
        int highestCount = ctx->totalTime()/ctx->highestUnit();
        qDebug()<<"highestCount : "<<highestCount;
        for(int i=0;i<highestCount+1;i++)
        {
            qint64 start2 = timer.elapsed();
            TimeStep *step = new TimeStep(ctx,i*ctx->highestUnit(),ctx->highestUnit(),RuleLine::RuleLineType::HIGHEST);
            timeSteps.append(step);
            qDebug()<<"time to create one step: "<<(timer.elapsed()-start2);

        }
        emit q->timeStepsChanged();
    }
    qDebug()<<"time to init step: "<<(timer.elapsed()-start);
}

QQmlListProperty<TimeStep> TimerPlayback::timeSteps()
{
    return QQmlListProperty<TimeStep>(this,&d->timeSteps);
}

TimerPlayback::~TimerPlayback()
{
    qDebug() << __FUNCTION__;
    d.clear();
}

TimerPlayback::TimerPlayback(QObject *parent, bool isInit) : base_type(QUrl("qrc:/qml/TimerPlayback.qml"), parent),
                                                             d(new Private(this))
{
    if (isInit)
    {
        init();
    }

    // connect(&background,&QThread::started,this,&TimerPlayback::generateLineData);
}

void TimerPlayback::generateLineData()
{
    d->generateLineDatas();
}

void TimerPlayback::setDuration(std::chrono::milliseconds duration)
{
    d->duration = duration;
    d->updateRuleLines();
}

void TimerPlayback::registerQmlType()
{
    qmlRegisterType<RulerContext>("models",1,0,"RulerContext");
    qmlRegisterType<TimeStep>("models",1,0,"TimeStep");
    qmlRegisterType<Ruler>("models", 1, 0, "Ruler");
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

bool TimerPlayback::isMaximumScale() const
{
    return d->ctx->isMaximumScale();
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
    if (!d->isGenerated)
        background.start();
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
    return QQmlListProperty<LineData>(this, &d->lineDatas);
}

double TimerPlayback::viewWidth() const
{
    return d->viewWidth;
}

void TimerPlayback::setViewWidth(double value)
{
    if (!d->isGenerated)
        background.start();
    if (d->setViewWidth(value))
        emit viewWidthChanged();
}

double TimerPlayback::viewX() const
{
    return d->viewX;
}

void TimerPlayback::setViewX(double value)
{
    // if (!d->isGenerated)
    //     background.start();
    if (d->setViewX(value))
        emit viewXChanged();
}

void TimerPlayback::setCurPos(qint64 value)
{
    if (d->setCurPos(value))
        emit curPosChanged();
}

qint64 TimerPlayback::curPos() const
{
    return d->curPos;
}

double TimerPlayback::mouseX() const
{
    return d->mouseX;
}

void TimerPlayback::setMouseX(double value)
{
    if (d->setMouseX(value))
        emit mouseXChanged();
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
    // updateLineDatas();
    updateContext();
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
    // updateLineDatas();
    updateContext();
    return true;
}

bool TimerPlayback::Private::setViewX(double value)
{
    qDebug() << __FUNCTION__ << "x : " << value;
    if (viewX == value)
        return false;

    viewX = value;
    // updateRuleLines();
    // updateLineDatas();
    updateContext();
    return true;
}

bool TimerPlayback::Private::setCurPos(qint64 value)
{
    if (curPos == value)
        return false;

    curPos = value;
    return true;
}

bool TimerPlayback::Private::setMouseX(double value)
{
    if (mouseX == value)
        return false;

    mouseX = value;
    q->setCurPos((abs(viewX) + mouseX) / widthPerMili);
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

bool isRoundedBy(qint64 target, qint64 unit)
{
    return target % unit == 0;
}

qint64 roundedBy(qint64 target, qint64 unit)
{
    return target - (target % unit);
}

void TimerPlayback::Private::refreshDelegateState()
{
    int secs = highestUnit / 1000;
    int mins = highestUnit / 60000;
    int hours = highestUnit / (3600000);

    if (secs == 1)
    {
        delegateState = DELEGATE_STATES[0];
    }
    else if (secs == 5)
    {
        delegateState = DELEGATE_STATES[2];
    }
    else if (mins == 5)
    {
        delegateState = DELEGATE_STATES[3];
    }
    else if (hours == 3)
    {
        delegateState = DELEGATE_STATES[6];
    }
    else if (secs == 10 || mins == 10 || hours == 10)
    {
        delegateState = DELEGATE_STATES[4];
    }
    else if (secs == 30 || mins == 30 || hours == 30)
    {
        delegateState = DELEGATE_STATES[5];
    }
    else if (mins == 1 || hours == 1)
    {
        delegateState = DELEGATE_STATES[1];
    }
}

QVector<qint64> TimerPlayback::Private::existedValueAt(qint64 start, qint64 stop, int level)
{
    QVector<qint64> res;
    for (int i = 0; i < lineDatas.length(); i++)
    {
        if (start < lineDatas[i]->value() && lineDatas[i]->value() < stop)
        {
            if (lineDatas[i]->level() == level)
                res.append(lineDatas[i]->value());
        }
    }

    return res;
}

void TimerPlayback::Private::updateLineDatas()
{
    qDebug() << "totalTime: " << totalTime << "ruleWidth: " << width << "; viewWidth: " << viewWidth << "; viewX: " << viewX;
    QElapsedTimer timer;
    qint64 start = timer.nsecsElapsed();
    if (!width)
        return;

    if (!viewWidth)
        return;

    if (width < viewWidth)
        return;

    widthPerMili = width / totalTime;

    // calculate highestUnit
    qint64 lastestHighestUnit = highestUnit;

    for (int i = 0; i < MS_LEVELS_SIZE - 1; i++)
    {
        qDebug() << "widthPerMili*MS_LEVE" << widthPerMili * MS_LEVELS[i];
        if (widthPerMili * MS_LEVELS[i] < HIGHEST_VISABLE_W && HIGHEST_VISABLE_W < widthPerMili * MS_LEVELS[i + 1])
        {
            highestUnit = MS_LEVELS[i + 1];
            break;
        }

        if (i == MS_LEVELS_SIZE - 2)
            highestUnit = MS_LEVELS[0];
    }

    qDebug() << "time to get highestUnit :" << timer.nsecsElapsed() - start;
    start = timer.nsecsElapsed();
    // TODO:improve performance...
    refreshDelegateState();
    int delegate0 = *(delegateState);
    int delegate1 = *(delegateState + 1);
    int delegate2 = *(delegateState + 2);

    smallestUnit = highestUnit / (delegate0 * delegate1 * delegate2);
    smallUnit = highestUnit / (delegate0 * delegate1);
    normalUnit = highestUnit / (delegate0);

    qDebug() << "HighestUnitWidth : " << (highestUnit * widthPerMili);
    if (smallestUnit == 100 && highestUnit * widthPerMili > 800)
        return;

    // calculate visible range
    qint64 startedValue = roundedBy(abs(viewX) / widthPerMili, highestUnit);
    qint64 stopedValue = roundedBy((abs(viewX) + viewWidth) / widthPerMili, highestUnit) + highestUnit;

    qDebug() << "startValue " << QTime(0, 0).addMSecs(startedValue).toString("hh:mm:ss:zzz") << "; stopedValue " << QTime(0, 0).addMSecs(stopedValue).toString("hh:mm:ss:zzz");
    qDebug() << "time to get units :" << timer.nsecsElapsed() - start;
    start = timer.nsecsElapsed();

    if (lineDatas.empty())
    {
        qDebug() << "init data";

        qint64 value = 0;
        while (value <= totalTime)
        {
            for (int n = 0; n < delegate0; n++)
            {
                for (int sm = 0; sm < delegate1; sm++)
                {
                    for (int sml = 0; sml < delegate2; sml++)
                    {
                        if (sml)
                            lineDatas.append(new LineData(q, RuleLine::RuleLineType::SMALLEST, value, startedValue <= value && value <= stopedValue, (int)RuleLine::RuleLineType::SMALLEST));

                        value += smallestUnit;
                    }
                    if (sm != delegate1 - 1)
                        lineDatas.append(new LineData(q, RuleLine::RuleLineType::SMALL, value, startedValue <= value && value <= stopedValue, (int)RuleLine::RuleLineType::SMALL));
                }
                if (n != delegate0 - 1)
                    lineDatas.append(new LineData(q, RuleLine::RuleLineType::NORMAL, value, startedValue <= value && value <= stopedValue, (int)RuleLine::RuleLineType::NORMAL));
                else
                    lineDatas.append(new LineData(q, RuleLine::RuleLineType::HIGHEST, value, startedValue <= value && value <= stopedValue, (int)RuleLine::RuleLineType::HIGHEST));
            }
        }
        updatePosition();
        emit q->lineDatasChanged();
    }
    qDebug() << "time to init first lineData :" << timer.nsecsElapsed() - start;
    start = timer.nsecsElapsed();
    // zoom in
    // downgrade linetype
    updatePosition();

    for (int i = 0; i < lineDatas.length(); i++)
    {
        if (startedValue <= lineDatas[i]->value() && lineDatas[i]->value() <= stopedValue)
        {
            lineDatas[i]->setVisible(true);
            if (isRoundedBy(lineDatas[i]->value(), highestUnit))
            {
                lineDatas[i]->setLineType((int)RuleLine::RuleLineType::HIGHEST);
            }
            else if (isRoundedBy(lineDatas[i]->value(), normalUnit))
            {
                lineDatas[i]->setLineType((int)RuleLine::RuleLineType::NORMAL);
            }
            else if (isRoundedBy(lineDatas[i]->value(), smallUnit))
            {
                lineDatas[i]->setLineType((int)RuleLine::RuleLineType::SMALL);
            }
            else if (isRoundedBy(lineDatas[i]->value(), smallestUnit))
            {
                lineDatas[i]->setLineType((int)RuleLine::RuleLineType::SMALLEST);
            }
            else
            {
                lineDatas[i]->setLineType((int)RuleLine::RuleLineType::UNDEFINED);
            }
        }
    }

    qDebug() << "time to update lineType :" << timer.nsecsElapsed() - start;
    start = timer.nsecsElapsed();

    int lengthBefore = lineDatas.length();
    qint64 value = startedValue;
    while (value <= totalTime)
    {
        // if (value < startedValue)
        // {
        //     value += highestUnit;
        //     continue;
        // }

        int index = 0;
        for (int n = 0; n < delegate0; n++)
        {

            for (int sm = 0; sm < delegate1; sm++)
            {

                for (int sml = 0; sml < delegate2; sml++)
                {
                    // qDebug()<<"n:"<<n<<"; sm: "<<sm<<"; sml:"<<sml;
                    if (sml)
                    {
                        index = indexShouldBeAdded(value);
                        if (index != -1)
                        {
                            if (index == lineDatas.length())
                                lineDatas.append(new LineData(q, RuleLine::RuleLineType::SMALLEST, value, startedValue <= value && value <= stopedValue));
                            else
                                lineDatas.insert(index, new LineData(q, RuleLine::RuleLineType::SMALLEST, value, startedValue <= value && value <= stopedValue));
                        }
                    }

                    value += smallestUnit;
                }
            }
        }

        if (value > stopedValue)
            break;
    }

    if (lineDatas.length() != lengthBefore)
        emit q->lineDatasChanged();

    qDebug() << "time to add new smallest :" << timer.nsecsElapsed() - start;
    start = timer.nsecsElapsed();

    updatePosition();
}

int TimerPlayback::Private::indexShouldBeAdded(qint64 value, int startIdx, int endIdx)
{
    // qDebug()<<"value "<<value<<"; start "<<startIdx<<"; end "<<endIdx;
    if(endIdx == 0)
        endIdx = lineDatas.length()-1;

    if(lineDatas[startIdx]->value() == value || lineDatas[endIdx]->value() == value)
        return -1;

    if(startIdx == endIdx)
    {
        if(lineDatas[startIdx]->value() < value)
            return startIdx+1;
        else
            return startIdx-1;
    }

    if(startIdx == endIdx -1)
    {
        if(lineDatas[startIdx]->value() < value && value < lineDatas[endIdx]->value())
            return endIdx;

        return endIdx+1;
    }

    int midIdx = (int)((startIdx+endIdx)/2);
    if(lineDatas[midIdx]->value() == value)
        return -1;

    if(lineDatas[midIdx]->value() > value)
        return indexShouldBeAdded(value,startIdx,midIdx);

    return indexShouldBeAdded(value,midIdx,endIdx);
}

void TimerPlayback::Private::generateLineDatas()
{
    QElapsedTimer timer;
    qint64 start = timer.elapsed();
    qDebug() << __FUNCTION__ << "totalTime: " << totalTime << "ruleWidth: " << width << "; viewWidth: " << viewWidth << "; viewX: " << viewX;
    if (!width)
        return;

    if (!viewWidth)
        return;

    if (width < viewWidth)
        return;

    if (lineDatas.empty())
    {
        qDebug() << __FUNCTION__ << "init data";

        qint64 value = 0;
        while (value <= totalTime)
        {
            value += 100;
            lineDatas.append(new LineData(q, RuleLine::RuleLineType::UNDEFINED, value, false));
        }
        emit q->lineDatasChanged();
        isGenerated = true;
        qDebug() << __FUNCTION__ << "finished generate ";
    }
    qDebug() << __FUNCTION__ << "time executed: " << (timer.elapsed() - start);
}

void TimerPlayback::Private::updatePosition()
{
    for (int i = 0; i < lineDatas.length(); i++)
    {
        lineDatas[i]->setPosition(lineDatas[i]->value() * widthPerMili);
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
