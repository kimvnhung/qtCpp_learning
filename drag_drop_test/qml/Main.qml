import QtQuick
import QtQuick.Controls

Window {
    width: 1100
    height: 760
    visible: true
    title: qsTr("Qt QML Drag and Drop Demo")

    GraphCanvas {
        anchors.fill: parent
    }
}
