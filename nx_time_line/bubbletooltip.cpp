#include "bubbletooltip.h"

#include <QUrl>


struct BubbleToolTip::Private
{
    BubbleToolTip* const q;

    std::unique_ptr<QQuickWidget> widget{

    };

};

BubbleToolTip::BubbleToolTip(QObject *parent):
    BubbleToolTip(QUrl("qrc:/qml/Nx/Controls/Bubble.qml"), parent)
{

}

BubbleToolTip::BubbleToolTip(
    const QUrl& componentUrl,
    QObject* parent)
    :
    QObject(parent),
    d(new Private{this})
{

}

BubbleToolTip::~BubbleToolTip()
{

}

