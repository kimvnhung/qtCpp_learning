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
    d->widget->setResizeMode(QQuickWidget::SizeRootObjectToView);
    d->widget->setSource(compentUrl);

    connect(d->widget.get(),SIGNAL(textChanged(QString)), this, SIGNAL(textChanged(QString)));
}

QQuickWidget* DemoQuickWidget::widget() const
{
    return d->widget.get();
}

void DemoQuickWidget::setText(QString text)
{
    bool success;
    QMetaObject::invokeMethod(d->widget.get(), "showText",
                            Q_RETURN_ARG(bool,success),
                            Q_ARG(QString, text));
    qDebug()<<"set success : "<<success;
}
