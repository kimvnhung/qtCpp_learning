import QtQuick 2.15
import QtQuick.Controls

Item {
    visible: true
    width: 300
    height: 400

    Rectangle {
        id: content
        width: parent.width
        height: parent.height

        Image {
            id: image
            source: "qrc:/images/earth.jpg"
            fillMode: Image.PreserveAspectFit
        }
    }

    Flickable {
        id: flickable
        clip: true
        // ...
        ScrollBar.vertical: ScrollBar {
            id: control
            size: 0.3
            position: 0.2
            active: true
            orientation: Qt.Vertical

            contentItem: Rectangle {
                implicitWidth: 6
                implicitHeight: 100
                radius: width / 2
                color: control.pressed ? "#81e889" : "#c2f4c6"
            }
        }
    }
}
