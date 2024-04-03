#ifndef QMLWIDGET_H
#define QMLWIDGET_H

#include <QObject>
#include <QUrl>
#include <QVariant>

 class QmlWidget : public QObject
{
    Q_OBJECT

protected:
    QmlWidget(const QUrl& componentUrl, QObject *parent = NULL);
    virtual void registerQmlType() = 0;
    virtual void setUpQmlUrl(const QUrl& componentUrl) = 0;
    virtual QObject* rootObject() const = 0;

    void init();

    template<typename ResultType, typename... ArgTypes>
    ResultType invokeQmlMethod(const char* methodName, ArgTypes... args)
    {
        if constexpr (std::is_void_v<ResultType>)
        {
            Q_ASSERT(QMetaObject::invokeMethod(rootObject(), methodName, Qt::DirectConnection,
                                               Q_ARG(QVariant, QVariant::fromValue(args))...));
        }
        else
        {
            QVariant result;
            Q_ASSERT(QMetaObject::invokeMethod(rootObject(), methodName, Qt::DirectConnection,
                                               Q_RETURN_ARG(QVariant, result),
                                               Q_ARG(QVariant, QVariant::fromValue(args))...));

            return result.value<ResultType>();
        }
    }
private:
    QUrl m_componentUrl;
};

#endif // QMLWIDGET_H
