#include "bubbletooltip2.h"

#include <QQuickItem>

class BubbleToolTip2::Private{
public:
    Private()
    {
        model = new BubbleToolTipModel();
    }

    std::unique_ptr<QQuickWidget> widget{new QQuickWidget()};
    BubbleToolTipModel *model;
    State state = State::hidden;
};


BubbleToolTip2::BubbleToolTip2(QObject *parent):
    BubbleToolTip2(QUrl("qrc:/qml/Bubble.qml"),parent)
{

}

QQuickWidget* BubbleToolTip2::widget() const
{
    return d->widget.get();
}

BubbleToolTip2::BubbleToolTip2(const QUrl& componentUrl, QObject* parent,bool isInit):
    base_type(componentUrl,parent),
    d(new Private())
{
    qDebug()<<__FUNCTION__;
    if(isInit){
        init();
    }
}

void BubbleToolTip2::registerQmlType()
{
    qDebug()<<__FUNCTION__;
}

void BubbleToolTip2::setUpQmlUrl(const QUrl& componentUrl)
{
    qDebug()<<__FUNCTION__;
    d->widget->setObjectName("BubbleToolTip2");
    d->widget->setResizeMode(QQuickWidget::SizeRootObjectToView);
    d->widget->setSource(componentUrl);
}

QObject* BubbleToolTip2::rootObject() const
{
    return d->widget.get()->rootObject();
}



void BubbleToolTip2::show()
{
    if (d->widget)
    {
        qDebug()<<__FUNCTION__<<__LINE__;
        invokeQmlMethod<void>("show");
        d->widget->raise();
    }

    // d->setState(State::shown);
}

void BubbleToolTip2::hide(bool immediately)
{
    // if (d->widget)
    //     invokeQmlMethod<void>(d->widget->rootObject(), "hide", immediately);

    // d->setState(State::hidden);
}

void BubbleToolTip2::suppress(bool immediately)
{
    // if (d->widget)
    //     invokeQmlMethod<void>(d->widget->rootObject(), "hide", immediately);
    // if (d->state == State::shown)
    //     d->setState(State::suppressed);
}

QString BubbleToolTip2::text() const
{
    // return d->text();
    return "";
}

void BubbleToolTip2::setText(const QString& value)
{
    // d->text = value;
}

Qt::Orientation BubbleToolTip2::orientation() const
{
    // return d->orientation;
    return Qt::Horizontal;
}

void BubbleToolTip2::setOrientation(Qt::Orientation value)
{
    // if (d->orientation == value)
    //     return;

    // d->orientation = value;
    // d->updatePosition();
}

bool BubbleToolTip2::suppressedOnMouseClick() const
{
    // return d->suppressedOnMouseClick;
    return false;
}

void BubbleToolTip2::setSuppressedOnMouseClick(bool value)
{
    // d->suppressedOnMouseClick = value;
}

void BubbleToolTip2::setTarget(const QRect& targetRect)
{
    // if (d->targetRect == targetRect)
    //     return;

    // d->targetRect = targetRect;
    // d->updatePosition();
}

void BubbleToolTip2::setTarget(const QPoint& targetPoint)
{
    // const QRectF targetRect(targetPoint, QSizeF(0, 0));
    // if (d->targetRect == targetRect)
    //     return;

    // d->targetRect = targetRect;
    // d->updatePosition();
}

void BubbleToolTip2::setEnclosingRect(const QRect& value)
{
    // if (d->enclosingRect == value)
    //     return;

    // d->enclosingRect = value;
    // d->updatePosition();
}

BubbleToolTip2::State BubbleToolTip2::state() const
{
    // return d->state;
    return BubbleToolTip2::State::shown;
}

// void BubbleToolTip2::Private::updatePosition()
// {
//     if (!widget)
//         return;

//     if (targetRect.width() < 0 || targetRect.height() < 0)
//     {
//         widget->move(-widget->width(), -widget->height());
//         return;
//     }

//     const auto minIntersection = std::min(64.0,
//                                           (orientation == Qt::Vertical ? targetRect.width() : targetRect.height()) / 2.0);

//     const auto params = invokeQmlMethod<QJSValue>(widget->rootObject(), "calculateParameters",
//                                                   (int) orientation,
//                                                   QRectF(targetRect).adjusted(0.5, 0.5, 0.5, 0.5),
//                                                   enclosingRect,
//                                                   minIntersection);

//     if (params.isUndefined() || !NX_ASSERT(params.isObject()))
//     {
//         widget->move(-widget->width(), -widget->height());
//         return;
//     }

//     pointerEdge = params.property("pointerEdge").toInt();
//     normalizedPointerPos = params.property("normalizedPointerPos").toNumber();

//     const auto x = params.property("x").toNumber();
//     const auto y = params.property("y").toNumber();

//     const auto pos = widget->parentWidget()
//                          ? widget->parentWidget()->mapFromGlobal(QPoint(x, y))
//                          : QPoint(x, y);

//     widget->move(pos);
// }

// void BubbleToolTip2::Private::setState(State value)
// {
//     if (state == value)
//         return;

//     state = value;
//     emit q->stateChanged(state);
// }
