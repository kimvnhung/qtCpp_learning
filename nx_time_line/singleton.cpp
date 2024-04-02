// Copyright 2018-present Network Optix, Inc. Licensed under MPL 2.0: www.mozilla.org/MPL/2.0/

#include "singleton.h"

#include <QDebug>

void SingletonBase::printInstantiationError(const std::type_info& typeInfo)
{
    qDebug()<<typeInfo.name()<< "Singleton is created more than once.";
}
