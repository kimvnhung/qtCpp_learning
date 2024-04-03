#include "qmlwidget.h"

#include <QVariant>

QmlWidget::QmlWidget(const QUrl& componentUrl, QObject *parent)
    :
    QObject{parent},
    m_componentUrl(componentUrl)
{

}

void QmlWidget::init()
{
    registerQmlType();
    setUpQmlUrl(m_componentUrl);
}


