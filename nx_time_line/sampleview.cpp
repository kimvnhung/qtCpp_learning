#include "sampleview.h"
#include "models/sample.h"

#include <QQuickWidget>
#include <QQmlContext>

class SampleView::Private
{
public:
    Private(SampleView *owner):
        owner(owner)
    {
        widget = new QQuickWidget(NULL);
        sample = new Sample(owner);
    }

    SampleView* owner;
    QQuickWidget *widget;
    Sample *sample;
};

SampleView::SampleView(QObject *parent) :
    SampleView(QUrl("qrc:/qml/sample_view.qml"),parent)
{

}

//protected
SampleView::SampleView(const QUrl& componentUrl, QObject *parent):
    QObject(parent),
    d(new Private(this))
{
    qmlRegisterType<Sample>("models",1,0,"Sample");

    d->widget->rootContext()->setContextProperty(QString("instance"),d->sample);
    d->widget->setObjectName("SampleView");
    d->widget->setResizeMode(QQuickWidget::SizeRootObjectToView);
    d->widget->setSource(componentUrl);
}

QQuickWidget* SampleView::widget() const
{
    return d->widget;
}

void SampleView::increaseSpeed()
{
    d->sample->setSpeed(d->sample->speed()+1);
}

void SampleView::decreaseSpeed()
{
    d->sample->setSpeed(d->sample->speed()-1);
}

void SampleView::increaseFan()
{
    d->sample->setNumHands(d->sample->numHands()+1);
}

void SampleView::decreaseFan()
{
    d->sample->setNumHands(d->sample->numHands()-1);
}

