#include "menuwidget.h"

#include <QQmlContext>
#include <QQuickItem>

class MenuWidget::Private
{
public:
    Private()
    {
        menuItems.append("Item 1");
        menuItems.append("Item 2");
    }

    std::unique_ptr<QQuickWidget> widget{new QQuickWidget()};

    QList<QString> menuItems;
};

MenuWidget::MenuWidget(QObject *parent)
    :
    QMLWidget{QUrl("qrc:/qml/menu.qml"), parent},
    d{new Private()}
{
    init();
    widget()->setMinimumSize(100,100);
}

MenuWidget::~MenuWidget()
{
    d.clear();
}

QList<QString> MenuWidget::menuItems() const
{
    return d->menuItems;
}

void MenuWidget::onMenuItemClicked(int index)
{
    qDebug() << "Menu item clicked: " << d->menuItems.at(index);
    emit menuItemClicked(index);
}

QQuickWidget* MenuWidget::widget() const
{
    return d->widget.get();
}


void MenuWidget::registerQmlType()
{
    widget()->rootContext()->setContextProperty(QString("instance"), this);
}

void MenuWidget::setUpQmlUrl(const QUrl& componentUrl)
{
    widget()->setObjectName("MenuWidget");
    widget()->setResizeMode(QQuickWidget::SizeRootObjectToView);
    d->widget->setSource(componentUrl);
}

QObject* MenuWidget::rootObject() const
{
    return d->widget.get()->rootObject();
}
