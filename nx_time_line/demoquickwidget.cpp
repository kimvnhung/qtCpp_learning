#include "demoquickwidget.h"

#include <QQuickWidget>

struct DemoQuickWidget::Private {
    DemoQuickWidget* const q;
    std::unique_ptr<QQuickWidget> widget{
        new QQuickWidget(NULL)
    };

};

DemoQuickWidget::DemoQuickWidget(QObject *parent):
    DemoQuickWidget(QUrl("qrc:qml/demo.qml"),parent)
{

}

//protected
DemoQuickWidget::DemoQuickWidget(const QUrl& compentUrl,QObject* parent):
    QObject(parent),
    d(new Private{this})
{
    d->widget->setObjectName("DemoQuickWidget");
    d->widget->setMinimumSize(400,300);
    d->widget->setResizeMode(QQuickWidget::SizeRootObjectToView);
    d->widget->setSource(compentUrl);
}

QQuickWidget* DemoQuickWidget::widget() const
{
    return d->widget.get();
}
