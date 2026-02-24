#pragma once

#include <QObject>
#include <QtQml/qqml.h>

class Backend : public QObject
{
    Q_OBJECT
    QML_ELEMENT

public:
    explicit Backend(QObject *parent = nullptr);

    Q_INVOKABLE QString cxxTrExample() const;
    Q_INVOKABLE QString cxxIdExample() const;
};
