#include "sampleview.h"

#include <QQuickWidget>

struct SampleView::Private{
    std::unique_ptr<QQuickWidget> widget{new QQuickWidget(NULL)};
};

SampleView::SampleView(QObject *parent) :
    SampleView(QUrl("qrc:/qml/sample_view.qml"),parent)
{

}

//protected
SampleView::SampleView(const QUrl& componentUrl, QObject *parent):
    QObject(parent),
    d(new Private())
{
    d->widget->setObjectName("SampleView");
    d->widget->setResizeMode(QQuickWidget::SizeRootObjectToView);
    d->widget->setSource(componentUrl);
}

QQuickWidget* SampleView::widget() const
{
    return d->widget.get();
}
