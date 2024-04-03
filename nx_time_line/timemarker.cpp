#include "timemarker.h"
#include "formatter.h"
#include "qqmlcontext.h"

#include <QDateTime>

static constexpr int kPointerLength = 3;

struct TimeMarker::Private
{
    TimeMarker* const q;

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

    // const QmlProperty<QString> dateText{q->widget(), "dateText"};
    // const QmlProperty<QString> timeText{q->widget(), "timeText"};
    // const QmlProperty<QPointF> pointerPos{q->widget(), "pointerPos"};
    // const QmlProperty<qreal> pointerLength{q->widget(), "pointerLength"};
    // const QmlProperty<qreal> timestampMs{q->widget(), "timestampMs"};
    // const QmlProperty<int> mode{q->widget(), "mode"};
};

TimeMarker::TimeMarker(QObject* parent):
    TimeMarker(QUrl("qrc:/qml/TimeMarker.qml"), parent)
{
    setSuppressedOnMouseClick(false);
}

// void TimeMarker::registerQmlType()
// {
//     qDebug()<<"register models";
//     qmlRegisterType<Timeline>("models",1,0,"Timeline");

// }

TimeMarker::TimeMarker(const QUrl& sourceUrl, QObject* parent):
    base_type(sourceUrl, parent,false),
    d(new Private{this})
{
    qDebug()<<__FUNCTION__;
    init();
    // widget()->rootContext()->setContextProperty(QString("instance"),d->sample);

    setObjectName("TimeSliderTooltip");
    setOrientation(Qt::Vertical);

    // if (ini().debugDisableQmlTooltips)
    //     return;

    // widget()->setAttribute(Qt::WA_TransparentForMouseEvents, false);
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

// QPointF TimeMarker::pointerPos() const
// {
//     return d->pointerPos();
// }

// TimeMarker::Mode TimeMarker::mode() const
// {
//     return (Mode) d->mode.value();
// }

// void TimeMarker::setMode(Mode value)
// {
//     if (d->mode == (int) value)
//         return;

//     d->mode = (int) value;
//     d->updatePosition();
// }

void TimeMarker::setTimeContent(const TimeContent& value)
{
    d->timeContent = value;
    // d->timestampMs = value.archivePosition.count();
    d->applyTimeContent();
}

void TimeMarker::setTimeMarkerDisplay(Display value)
{
    if (d->display == value)
        return;

    d->display = value;
    d->applyTimeContent();
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
        // timeFormat = timeContent->localFileLength >= 1h
        //                  ? (timeContent->showMilliseconds ? Format::hhh_mm_ss_zzz : Format::hhh_mm_ss)
        //                  : (timeContent->showMilliseconds ? Format::mm_ss_zzz : Format::mm_ss);
    }

    timeTextLine = toString(timeContent->displayPosition.count(), timeFormat);

    if (dateTextLine.isEmpty())
    {
        // timeText = timeTextLine;
        // dateText = {};
        return;
    }

    // timeText = timeContent->showDate ? "" : timeTextLine;
    // dateText = dateTextLine;
}

void TimeMarker::Private::updatePosition()
{
    // switch ((Mode) mode.value())
    // {
    // case Mode::normal:
    // {
    //     pointerLength = kPointerLength;
    //     q->setEnclosingRect({minX, 0, maxX - minX, QWIDGETSIZE_MAX});
    //     break;
    // }

    // case Mode::leftmost:
    // {
    //     pointerLength = 0;
    //     q->setEnclosingRect({pointerX, 0, QWIDGETSIZE_MAX, QWIDGETSIZE_MAX});
    //     break;
    // }

    // case Mode::rightmost:
    // {
    //     pointerLength = 0;
    //     q->setEnclosingRect({pointerX - QWIDGETSIZE_MAX, 0, QWIDGETSIZE_MAX, QWIDGETSIZE_MAX});
    //     break;
    // }
    // }

    // q->setTarget(QPoint(pointerX, bottomY + pointerLength));
}
