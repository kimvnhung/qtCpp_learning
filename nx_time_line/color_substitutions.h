// Copyright 2018-present Network Optix, Inc. Licensed under MPL 2.0: www.mozilla.org/MPL/2.0/

#pragma once

#include <QtCore/QMap>
#include <QtGui/QColor>

using ColorSubstitutions = QMap<QColor, QColor>;
inline bool operator<(const QColor& color1, const QColor& color2)
{
    return color1.rgb() < color2.rgb();
}
