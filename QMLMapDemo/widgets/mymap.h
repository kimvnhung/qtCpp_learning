#ifndef MYMAP_H
#define MYMAP_H

#include "qmlwidget.h"

#include <QObject>

class MyMap : public QMLWidget
{
    Q_OBJECT
    using base_type = QMLWidget;
public:
    explicit MyMap(QObject *parent = nullptr);

    ~MyMap();
    QQuickWidget* widget() const override;
protected:
    void registerQmlType() override;
    void setUpQmlUrl(const QUrl &componentUrl) override;
    QObject *rootObject() const override;

private:
    class Private;
    QSharedPointer<Private> d;
};

#endif // MYMAP_H
