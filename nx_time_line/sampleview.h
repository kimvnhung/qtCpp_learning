#ifndef SAMPLEVIEW_H
#define SAMPLEVIEW_H

#include <QObject>
#include <QQuickWidget>
#include <QSharedPointer>

class SampleView : public QObject
{
    Q_OBJECT
public:
    explicit SampleView(QObject *parent = nullptr);

    QQuickWidget *widget() const;

    void increaseSpeed();
    void decreaseSpeed();
    void increaseFan();
    void decreaseFan();

protected:
    SampleView(const QUrl& componentUrl, QObject *parent);
private:
    class Private;
    QSharedPointer<Private> d;
};

#endif // SAMPLEVIEW_H
