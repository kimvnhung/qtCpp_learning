#ifndef BUBBLETOOLTIP_H
#define BUBBLETOOLTIP_H

#include <QObject>
#include <QQuickWidget>
#include <QSharedPointer>

class BubbleToolTip : public QObject
{
    Q_OBJECT
public:
    explicit BubbleToolTip(QObject *parent = nullptr);
    ~BubbleToolTip();

    void show();
    void hide(bool immediately = false);
    void suppress(bool immediately = false);

    QString text() const;
    void setText(const QString& value);

    Qt::Orientation orientation() const; //< Default Qt::Horizontal
    void setOrientation(Qt::Orientation value);

    bool suppressedOnMouseClick() const; //< Default true.
    void setSuppressedOnMouseClick(bool value);

    // Target and enclosing rect are specified in global coordinates.
    void setTarget(const QRect& targetRect);
    void setTarget(const QPoint& targetPoint);
    void setEnclosingRect(const QRect& value);

    enum class State
    {
        shown,
        hidden,
        suppressed
    };

    State state() const;
    QQuickWidget* widget() const;

signals:
    void stateChanged(State state);
protected:
    BubbleToolTip(const QUrl& componentUrl,QObject *parent = nullptr);

private:
    struct Private;
    QSharedPointer<Private> d;
};

#endif // BUBBLETOOLTIP_H
