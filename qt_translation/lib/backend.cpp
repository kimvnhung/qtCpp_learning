#include "backend.h"

#include <QtCore/qcoreapplication.h>

Backend::Backend(QObject *parent)
    : QObject(parent)
{
}

QString Backend::cxxTrExample() const
{
    return tr("Hello from C++ tr()");
}

QString Backend::cxxIdExample() const
{
    //% "Hello from C++ qtTrId()"
    return qtTrId("app.cxx_hello_id");
}
