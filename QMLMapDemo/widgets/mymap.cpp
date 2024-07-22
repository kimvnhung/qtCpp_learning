#include "mymap.h"

#include <QQuickItem>
#include <QQmlContext>

class MyMap::Private{
public:
    Private(MyMap *owner):
        owner(owner)
    {

    }

    MyMap *owner;
    std::unique_ptr<QQuickWidget> widget{new QQuickWidget()};
};

MyMap::MyMap(QObject *parent)
    :
    base_type(QUrl("qrc:/osm_map.qml"), parent),
    d{new Private(this)}
{
    init();
}

MyMap::~MyMap()
{
    d.clear();
}

QQuickWidget *MyMap::widget() const
{
    return d->widget.get();
}

void MyMap::registerQmlType()
{
    qmlRegisterType<MyMap>("MyMap", 1, 0, "MyMap");
}

void MyMap::setUpQmlUrl(const QUrl &componentUrl)
{
    widget()->setObjectName("TimerPlayback");
    widget()->setResizeMode(QQuickWidget::SizeRootObjectToView);
    widget()->setSource(componentUrl);
}

QObject *MyMap::rootObject() const
{
    return d->widget.get()->rootObject();
}


