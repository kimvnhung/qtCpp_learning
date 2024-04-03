#include "timemarker.h"
#include "formatter.h"
#include "qqmlcontext.h"

#include <QDateTime>

using namespace std::chrono;
static constexpr int kPointerLength = 3;

class TimeMarker::Private
{
public:
    Private(TimeMarker* parent):
        q(parent)
    {
        model = new Timeline(parent);
    }
    TimeMarker* const q;
    Timeline* model;

    FormatterPtr dateFormatter = Formatter::custom(
        QLocale(), Formatter::system()->is24HoursTimeFormat());

    std::optional<TimeContent> timeContent;


    int pointerX = 0;
    int bottomY = 0;
    int minX = 0;
    int maxX = 0;

    Display display = Display::compact;

    void applyTimeContent();
    void updatePosition();

};

TimeMarker::TimeMarker(QObject* parent):
    TimeMarker(QUrl("qrc:/qml/TimeMarker.qml"), parent)
{
    setSuppressedOnMouseClick(false);
}

TimeMarker::TimeMarker(const QUrl& sourceUrl, QObject* parent):
    base_type(sourceUrl, parent,false),
    d(new Private{this})
{
    qDebug()<<__FUNCTION__;
    init();
    // widget()->rootContext()->setContextProperty(QString("instance"),d->sample);

    setObjectName("TimeSliderTooltip");
    setOrientation(Qt::Vertical);

}

void TimeMarker::registerQmlType()
{
    qDebug()<<__FUNCTION__;
    qmlRegisterType<Timeline>("models",1,0,"Timeline");
}

TimeMarker::~TimeMarker()
{
    // Required here for forward declared scoped pointer destruction.
}

bool TimeMarker::isShown() const
{
    return state() == State::shown;
}

void TimeMarker::setShown(bool value)
{
    if (value)
        show();
    else
        hide();
}

QPointF TimeMarker::pointerPos() const
{
    return reinterpret_cast<Timeline*>(model())->pointerPos();
}

TimeMarker::Mode TimeMarker::mode() const
{
    return (Mode)reinterpret_cast<Timeline*>(model())->mode();
}

void TimeMarker::setMode(Mode value)
{
    reinterpret_cast<Timeline*>(model())->setMode((int)value);
    d->updatePosition();
}

void TimeMarker::setTimeContent(const TimeContent& value)
{
    d->timeContent = value;
    reinterpret_cast<Timeline*>(model())->setTimestampMs(value.archivePosition.count());
    d->applyTimeContent();
}

void TimeMarker::setTimeMarkerDisplay(Display value)
{
    if (d->display == value)
        return;

    d->display = value;
    d->applyTimeContent();
}

BubbleToolTipModel* TimeMarker::model() const
{
    return d->model;
}

void TimeMarker::setPosition(int pointerX, int bottomY, int minX, int maxX)
{
    d->pointerX = pointerX;
    d->bottomY = bottomY;
    d->minX = minX;
    d->maxX = maxX;
    d->updatePosition();
}

void TimeMarker::Private::applyTimeContent()
{
    if (!timeContent)
        return;

    QString dateTextLine;
    QString timeTextLine;

    Format timeFormat;
    if (timeContent->isTimestamp)
    {
        const QDateTime dateTime =
            QDateTime::fromMSecsSinceEpoch(timeContent->displayPosition.count());
        dateTextLine = toString(dateTime.date(), Format::dd_MM_yyyy, dateFormatter);
        timeFormat = timeContent->showMilliseconds ? Format::hh_mm_ss_zzz : Format::hh_mm_ss;
    }
    else
    {
        dateTextLine.clear();
        timeFormat = timeContent->localFileLength >= 1h
                         ? (timeContent->showMilliseconds ? Format::hhh_mm_ss_zzz : Format::hhh_mm_ss)
                         : (timeContent->showMilliseconds ? Format::mm_ss_zzz : Format::mm_ss);
    }

    timeTextLine = toString(timeContent->displayPosition.count(), timeFormat);

    if (dateTextLine.isEmpty())
    {
        model->setTimeText(timeTextLine);
        model->setDateText({});
        return;
    }

    model->setTimeText(timeContent->showDate ? "" : timeTextLine);
    model->setDateText(dateTextLine);

}

void TimeMarker::Private::updatePosition()
{
    switch ((Mode)model->mode())
    {
    case Mode::normal:
    {
        model->setPointerLength(kPointerLength);
        q->setEnclosingRect({minX, 0, maxX - minX, QWIDGETSIZE_MAX});
        break;
    }

    case Mode::leftmost:
    {
        model->setPointerLength(0);
        q->setEnclosingRect({pointerX, 0, QWIDGETSIZE_MAX, QWIDGETSIZE_MAX});
        break;
    }

    case Mode::rightmost:
    {
        model->setPointerLength(0);
        q->setEnclosingRect({pointerX - QWIDGETSIZE_MAX, 0, QWIDGETSIZE_MAX, QWIDGETSIZE_MAX});
        break;
    }
    }

    q->setTarget(QPoint(pointerX, bottomY + model->pointerLength()));
}

Timeline::Timeline(QObject *parent):
    BubbleToolTipModel(parent)
{

}

QString Timeline::dateText() const
{
    return m_dateText;
}

void Timeline::setDateText(const QString &newDateText)
{
    if (m_dateText == newDateText)
        return;
    m_dateText = newDateText;
    emit dateTextChanged();
}

QString Timeline::timeText() const
{
    return m_timeText;
}

void Timeline::setTimeText(const QString &newTimeText)
{
    if (m_timeText == newTimeText)
        return;
    m_timeText = newTimeText;
    emit timeTextChanged();
}

QPointF Timeline::pointerPos() const
{
    return m_pointerPos;
}

void Timeline::setPointerPos(QPointF newPointerPos)
{
    if (m_pointerPos == newPointerPos)
        return;
    m_pointerPos = newPointerPos;
    emit pointerPosChanged();
}

qreal Timeline::pointerLength() const
{
    return m_pointerLength;
}

void Timeline::setPointerLength(qreal newPointerLength)
{
    if (qFuzzyCompare(m_pointerLength, newPointerLength))
        return;
    m_pointerLength = newPointerLength;
    emit pointerLengthChanged();
}

qreal Timeline::timestampMs() const
{
    return m_timestampMs;
}

void Timeline::setTimestampMs(qreal newTimestampMs)
{
    if (qFuzzyCompare(m_timestampMs, newTimestampMs))
        return;
    m_timestampMs = newTimestampMs;
    emit timestampMsChanged();
}

int Timeline::mode() const
{
    return m_mode;
}

void Timeline::setMode(int mode)
{
    if(m_mode == mode)
        return;
    m_mode = mode;
    emit modeChanged();
}
