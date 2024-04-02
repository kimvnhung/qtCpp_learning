#ifndef BUBBLETOOLTIP2_H
#define BUBBLETOOLTIP2_H

#include <QObject>
#include <QQuickWidget>
#include <QSharedPointer>

class BubbleToolTip2 : public QObject
{
    Q_OBJECT
public:
    explicit BubbleToolTip2(QObject *parent = nullptr);


    QQuickWidget* widget() const;

signals:
protected:
    BubbleToolTip2(const QUrl& componentUrl, QObject* parent = NULL);

private:
    class Private;
    QSharedPointer<Private> d;
};

#endif // BUBBLETOOLTIP2_H
