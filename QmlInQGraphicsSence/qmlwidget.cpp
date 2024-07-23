#include "qmlwidget.h"

#include <QVariant>

QMLWidget::QMLWidget(const QUrl& componentUrl, QObject *parent)
    :
    QObject{parent},
    m_componentUrl(componentUrl)
{

}

void QMLWidget::init()
{
    registerQmlType();
    setUpQmlUrl(m_componentUrl);
}
