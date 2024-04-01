#ifndef DEMOQUICKWIDGET_H
#define DEMOQUICKWIDGET_H

#include <QObject>
#include <QQuickWidget>
#include <QSharedPointer>


class DemoQuickWidget : public QObject
{
    Q_OBJECT
public:
    DemoQuickWidget(QObject *parent = NULL);
    QQuickWidget* widget() const;
protected:
    DemoQuickWidget(const QUrl &url, QObject *parent = NULL);
private:
    struct Private;
    QSharedPointer<Private> d;
};

#endif // DEMOQUICKWIDGET_H
