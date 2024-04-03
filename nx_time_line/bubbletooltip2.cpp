#include "bubbletooltip2.h"

#include <QQuickItem>
#include <QQuickWidget>

class BubbleToolTip2::Private{
public:
    Private(BubbleToolTip2 *parent):
        q(parent)
    {
        model = new BubbleToolTipModel(parent);
    }

    BubbleToolTip2* q;
    std::unique_ptr<QQuickWidget> widget{new QQuickWidget()};
    BubbleToolTipModel *model;

    QRectF targetRect;
    QRectF enclosingRect;
    Qt::Orientation orientation = Qt::Horizontal;
    bool suppressedOnMouseClick = true;

    void updatePosition();
};

BubbleToolTipModel::BubbleToolTipModel(QObject *parent) :
    QObject(parent),
    m_state(State::hidden)
{

}

BubbleToolTipModel::~BubbleToolTipModel()
{

}

BubbleToolTipModel::State BubbleToolTipModel::state()
{
    return m_state;
}

void BubbleToolTipModel::setState(BubbleToolTipModel::State state)
{
    m_state = state;
    emit stateChanged();
}

QString BubbleToolTipModel::text() const
{
    return m_text;
}

void BubbleToolTipModel::setText(const QString &newText)
{
    if (m_text == newText)
        return;
    m_text = newText;
    emit textChanged();
}

qreal BubbleToolTipModel::normalizedPointerPos() const
{
    return m_normalizedPointerPos;
}

void BubbleToolTipModel::setNormalizedPointerPos(qreal newNormalizedPointerPos)
{
    if (qFuzzyCompare(m_normalizedPointerPos, newNormalizedPointerPos))
        return;
    m_normalizedPointerPos = newNormalizedPointerPos;
    emit normalizedPointerPosChanged();
}

int BubbleToolTipModel::pointerEdge() const
{
    return m_pointerEdge;
}

void BubbleToolTipModel::setPointerEdge(int newPointerEdge)
{
    if (m_pointerEdge == newPointerEdge)
        return;
    m_pointerEdge = newPointerEdge;
    emit pointerEdgeChanged();
}


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
    d(new Private(this))
{
    qDebug()<<__FUNCTION__;
    if(isInit){
        init();
    }

    // connect(this, &QObject::objectNameChanged, d->widget.get(), &QObject::setObjectName);

}

void BubbleToolTip2::registerQmlType()
{
    qDebug()<<__FUNCTION__;

}

void BubbleToolTip2::setUpQmlUrl(const QUrl& componentUrl)
{
    qDebug()<<__FUNCTION__;
    widget()->setObjectName("BubbleToolTip2");
    widget()->setResizeMode(QQuickWidget::SizeRootObjectToView);
    widget()->setSource(componentUrl);
}

QObject* BubbleToolTip2::rootObject() const
{
    return widget()->rootObject();
}

void BubbleToolTip2::show()
{
    if (d->widget)
    {
        invokeQmlMethod<void>("show");
        d->widget->raise();
    }

    model()->setState(State::shown);
}

BubbleToolTipModel* BubbleToolTip2::model() const
{
    return d->model;
}

void BubbleToolTip2::hide(bool immediately)
{
    if (d->widget)
        invokeQmlMethod<void>("hide", immediately);

    model()->setState(State::hidden);
}

void BubbleToolTip2::suppress(bool immediately)
{
    if (widget())
        invokeQmlMethod<void>("hide", immediately);
    if (state() == State::shown)
        model()->setState(State::suppressed);
}

QString BubbleToolTip2::text() const
{
    return model()->text();
}

void BubbleToolTip2::setText(const QString& value)
{
    model()->setText(value);
}

Qt::Orientation BubbleToolTip2::orientation() const
{
    return d->orientation;
}

void BubbleToolTip2::setOrientation(Qt::Orientation value)
{
    if (d->orientation == value)
        return;

    d->orientation = value;
    d->updatePosition();
}

bool BubbleToolTip2::suppressedOnMouseClick() const
{
    return d->suppressedOnMouseClick;
}

void BubbleToolTip2::setSuppressedOnMouseClick(bool value)
{
    d->suppressedOnMouseClick = value;
}

void BubbleToolTip2::setTarget(const QRect& targetRect)
{
    if (d->targetRect == targetRect)
        return;

    d->targetRect = targetRect;
    d->updatePosition();
}

void BubbleToolTip2::setTarget(const QPoint& targetPoint)
{
    const QRectF targetRect(targetPoint, QSizeF(0, 0));
    if (d->targetRect == targetRect)
        return;

    d->targetRect = targetRect;
    d->updatePosition();
}

void BubbleToolTip2::setEnclosingRect(const QRect& value)
{
    if (d->enclosingRect == value)
        return;

    d->enclosingRect = value;
    d->updatePosition();
}

BubbleToolTip2::State BubbleToolTip2::state() const
{
    return model()->state();
}

void BubbleToolTip2::Private::updatePosition()
{
    if (!widget)
        return;

    if (targetRect.width() < 0 || targetRect.height() < 0)
    {
        widget->move(-widget->width(), -widget->height());
        return;
    }

    const auto minIntersection = std::min(64.0,
                                          (orientation == Qt::Vertical ? targetRect.width() : targetRect.height()) / 2.0);

    const auto params = q->invokeQmlMethod<QJSValue>("calculateParameters",
                                                  (int) orientation,
                                                  QRectF(targetRect).adjusted(0.5, 0.5, 0.5, 0.5),
                                                  enclosingRect,
                                                  minIntersection);

    if (params.isUndefined() || !params.isObject())
    {
        widget->move(-widget->width(), -widget->height());
        return;
    }

    model->setPointerEdge(params.property("pointerEdge").toInt());
    model->setNormalizedPointerPos(params.property("normalizedPointerPos").toNumber());

    const auto x = params.property("x").toNumber();
    const auto y = params.property("y").toNumber();

    const auto pos = widget->parentWidget()
                         ? widget->parentWidget()->mapFromGlobal(QPoint(x, y))
                         : QPoint(x, y);

    widget->move(pos);
}

// void BubbleToolTip2::Private::setState(State value)
// {
//     if (state == value)
//         return;

//     state = value;
//     emit q->stateChanged(state);
// }
