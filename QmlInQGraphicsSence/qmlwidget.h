#ifndef QMLWIDGET_H
#define QMLWIDGET_H

#include <QObject>

class QMLWidget : public QObject
{
    Q_OBJECT
public:
    explicit QMLWidget(QObject *parent = nullptr);

signals:
};

#endif // QMLWIDGET_H
