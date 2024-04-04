// Copyright 2018-present Network Optix, Inc. Licensed under MPL 2.0: www.mozilla.org/MPL/2.0/

import QtQuick 2.0
import QtQuick.Controls

// import Nx.Controls 1.0

// import "private"

// Since we want have consistent behavior we have to avoid flicking and manage scrolling by
// wheel directly with mouse area.
Item
{
    Text {
        id: txt
        text: qsTr("This is rule")
    }
}
