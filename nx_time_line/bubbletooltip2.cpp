#include "bubbletooltip2.h"

class BubbleToolTip2::Private{
public:
    Private()
    {

    }

    std::unique_ptr<QQuickWidget> widget{new QQuickWidget()};

};


BubbleToolTip2::BubbleToolTip2(QObject *parent):
    BubbleToolTip2(QUrl("qrc:/qml/bubble_tool_tip.qml"),parent)
{

}

QQuickWidget* BubbleToolTip2::widget() const
{
    return d->widget.get();
}

BubbleToolTip2::BubbleToolTip2(const QUrl& componentUrl, QObject* parent):
    QObject(parent),
    d(new Private())
{
    d->widget->setObjectName("BubbleToolTip2");
    d->widget->setResizeMode(QQuickWidget::SizeRootObjectToView);
    d->widget->setSource(componentUrl);
}

