#include "timestep.h"



class TimeStep::Private
{
public:
    Private(TimeStep *owner,RulerContext *ctx, qint64 offset, qint64 unit, RuleLine::RuleLineType lineType):
        owner(owner),
        offset(offset),
        unit(unit),
        lineType(lineType),
        context(ctx)
    {
        updateSubItems();
    }

    TimeStep *owner;

    qint64 offset;
    qint64 unit;
    RuleLine::RuleLineType lineType;
    QList<TimeStep*> subItems;
    RulerContext *context;

    int *delegateState;

    void updateSubItems();
    int getSubCount();
    bool isVisible();
};

/*
 * Private Function
*/
bool TimeStep::Private::isVisible()
{
    return context->isVisible(offset+unit);
}
void TimeStep::Private::updateSubItems()
{
    if(subItems.empty())
    {
        int subCount = getSubCount();
        if(subCount == 0)
            return;

        qint64 subUnit = unit/subCount;
        for(int i=0;i<subCount;i++){
            RuleLine::RuleLineType subLineType = RuleLine::RuleLineType::UNDEFINED;
            if(lineType < RuleLine::RuleLineType::SMALLEST){
                subLineType = (RuleLine::RuleLineType)((int)lineType+1);
            }

            TimeStep *step = new TimeStep(context,offset+i*subUnit,subUnit,subLineType);
            connect(context,&RulerContext::unitsChanged,step, &TimeStep::onContextChanged);
            subItems.append(step);
        }

        emit owner->subItemsChanged();
    }
}

int TimeStep::Private::getSubCount()
{
    if(unit == 100)
        return 0;

    int secs = unit / 1000;
    int mins = unit / 60000;
    int hours = unit / (3600000);

    if(unit == 500){
        return 5;
    }else if (secs == 1)
    {
        return 2;
    }
    else if (secs == 5 || mins == 5)
    {
        return 5;
    }
    else if (hours == 3)
    {
        return 3;
    }
    else if (secs == 10 || mins == 10 || hours == 10)
    {
        return 2;
    }
    else if (secs == 30 || mins == 30 || hours == 30)
    {
        return 3;
    }
    else if (mins == 1 || hours == 1)
    {
        return 2;
    }

    return 0;
}

/*
 *
*/

TimeStep::TimeStep(RulerContext* context, qint64 offset, qint64 unit, RuleLine::RuleLineType lineType):
    QObject{context},
    d(new Private(this,context,offset,unit,lineType))
{
    connect(context,&RulerContext::xChanged,this,&TimeStep::contextChanged);
    connect(context,&RulerContext::widthChanged,this,&TimeStep::contextChanged);
    connect(this,&TimeStep::contextChanged,this,&TimeStep::onContextChanged);
}


void TimeStep::onContextChanged()
{
    if(!d->isVisible())
        return;

    if(unit() == context()->highestUnit())
        setLineType((int)RuleLine::RuleLineType::HIGHEST);
    else if(unit() == context()->normalUnit())
        setLineType((int)RuleLine::RuleLineType::NORMAL);
    else if(unit() == context()->smallUnit())
        setLineType((int)RuleLine::RuleLineType::SMALL);
    else if(unit() == context()->smallestUnit())
        setLineType((int)RuleLine::RuleLineType::SMALLEST);
    else
        setLineType((int)RuleLine::RuleLineType::UNDEFINED);

    d->updateSubItems();
}

void TimeStep::setLineType(int value)
{
    if((int)d->lineType == value)
        return;

    d->lineType = (RuleLine::RuleLineType)value;
    emit lineTypeChanged();
    emit isDisableTextChanged();
}

RulerContext* TimeStep::context() const
{
    return d->context;
}

QQmlListProperty<TimeStep> TimeStep::subItems()
{
    return QQmlListProperty<TimeStep>(this, &d->subItems);
}

int TimeStep::lineType() const
{    return (int)d->lineType;
}

qint64 TimeStep::value() const
{
    return d->offset+d->unit;
}

qint64 TimeStep::unit() const
{
    return d->unit;
}

double TimeStep::width() const
{
    return context()->widthPerMili()*unit();
}

double TimeStep::relativeWidth() const
{
    if(d->offset < d->context->startValueByUnit(unit()) || value() > d->context->stopValueByUnit(unit()))
        return 0;

    if(d->offset == d->context->startValueByUnit(unit()))
        return width() - (abs(d->context->x())-d->context->startValueByUnit(unit())*d->context->widthPerMili());
    if(value() == d->context->stopValueByUnit(unit()))
        return width()-(d->context->stopValueByUnit(unit())*d->context->widthPerMili()-abs(d->context->x())-d->context->visibleWidth());

    return width();
}

bool TimeStep::isDisableText() const
{
    return (value()%d->context->normalUnit() == 0 && d->lineType != RuleLine::RuleLineType::NORMAL) || (d->context->stopValueByUnit(unit()) == value());
}
