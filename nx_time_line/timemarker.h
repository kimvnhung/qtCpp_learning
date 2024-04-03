#ifndef TIMEMARKER_H
#define TIMEMARKER_H

#include "bubbletooltip2.h"


#include <QObject>
class Timeline : public BubbleToolTipModel
{
    Q_OBJECT
    Q_PROPERTY(QString dateText READ dateText WRITE setDateText NOTIFY dateTextChanged FINAL)
    Q_PROPERTY(QString timeText READ timeText WRITE setTimeText NOTIFY timeTextChanged FINAL)
    Q_PROPERTY(QPointF pointerPos READ pointerPos WRITE setPointerPos NOTIFY pointerPosChanged FINAL)
    Q_PROPERTY(qreal pointerLength READ pointerLength WRITE setPointerLength NOTIFY pointerLengthChanged FINAL)
    Q_PROPERTY(qreal timestampMs READ timestampMs WRITE setTimestampMs NOTIFY timestampMsChanged FINAL)
    Q_PROPERTY(int mode READ mode WRITE setMode NOTIFY modeChanged FINAL)
public:
    Timeline(QObject *parent = NULL);

    enum class TimeMarkerMode
    {
        normal,
        leftmost,
        rightmost
    };
    Q_ENUM(TimeMarkerMode)

    enum class TimeMarkerDisplay
    {
        full,
        compact,
        automatic
    };
    Q_ENUM(TimeMarkerDisplay)

    QString dateText() const;
    void setDateText(const QString &newDateText);
    QString timeText() const;
    void setTimeText(const QString &newTimeText);
    QPointF pointerPos() const;
    void setPointerPos(QPointF newPointerPos);
    qreal pointerLength() const;
    void setPointerLength(qreal newPointerLength);
    qreal timestampMs() const;
    void setTimestampMs(qreal newTimestampMs);
    int mode() const;
    void setMode(int mode);

signals:
    void dateTextChanged();
    void timeTextChanged();
    void pointerPosChanged();
    void pointerLengthChanged();
    void timestampMsChanged();
    void modeChanged();

private:
    QString m_dateText;
    QString m_timeText;
    QPointF m_pointerPos;
    qreal m_pointerLength;
    qreal m_timestampMs;
    int m_mode;

};

class TimeMarker : public BubbleToolTip2
{
    Q_OBJECT
    using base_type = BubbleToolTip2;
    using Mode = Timeline::TimeMarkerMode;
    using Display = Timeline::TimeMarkerDisplay;
    using State = Timeline::State;
public:
    struct TimeContent
    {
        std::chrono::milliseconds archivePosition{};
        std::chrono::milliseconds displayPosition{};
        bool isTimestamp = true;
        std::chrono::milliseconds localFileLength{}; //< Should be 0 if archive is not local file.
        bool showDate = true;
        bool showMilliseconds = false;
    };

    explicit TimeMarker(QObject* parent = nullptr);
    virtual ~TimeMarker() override;

    bool isShown() const;
    void setShown(bool value);

    QPointF pointerPos() const;

    Mode mode() const;
    void setMode(Mode value);

    /** Set date and time shown in text lines. */
    void setTimeContent(const TimeContent& value);

    /**
     * @param value If `TimeMarkerDisplay::full`, then both date and time are always shown.
     *     If `TimeMarkerDisplay::compact`, then either date or time is shown, depending on scale.
     *     If `TimeMarkerDisplay::automatic`, then either full or compact mode is chosen,
     *         depending on available width.
     */
    void setTimeMarkerDisplay(Display value);

    /**
     * Point tooltip tail according to x, but align bottom edge of the tooltip to bottomY.
     * @param pointerX X coordinate for tooltip pointer tip, in global coordinate system.
     * @param bottomY Y coordinate to align tooltip bottom edge, in global coordinate system.
     * @param minX, maxX Horizontal boundaries for the body of the tooltip.
     *     Ignored if mode is not Mode::normal or if minX == maxX.
     *     Specified in global coordinate system.
     */
    void setPosition(int pointerX, int bottomY, int minX = 0, int maxX = 0);

    BubbleToolTipModel* model() const;
protected:
    explicit TimeMarker(const QUrl& sourceUrl, QObject* parent = nullptr);

private:
    class Private;
    QSharedPointer<Private> d;

    // QmlWidget interface
protected:
    void registerQmlType() override;
};



#endif // TIMEMARKER_H
