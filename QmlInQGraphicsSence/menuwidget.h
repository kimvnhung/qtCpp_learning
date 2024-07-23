#ifndef MENUWIDGET_H
#define MENUWIDGET_H

#include "qmlwidget.h"

class MenuWidget : public QMLWidget
{
    Q_OBJECT
    Q_PROPERTY(QList<QString> menuItems READ menuItems NOTIFY menuItemsChanged)
public:
    explicit MenuWidget(QObject *parent = nullptr);
    ~MenuWidget();

    QList<QString> menuItems() const;

signals:
    void menuItemsChanged();
    void menuItemClicked(int index);
public slots:
    void onMenuItemClicked(int index);
    // QMLWidget interface
public:
    QQuickWidget* widget() const override;
protected:
    void registerQmlType() override;
    void setUpQmlUrl(const QUrl& componentUrl) override;
    QObject* rootObject() const override;

private:
    class Private;
    QSharedPointer<Private> d;
};

#endif // MENUWIDGET_H
