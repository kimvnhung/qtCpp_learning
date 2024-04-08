import QtQuick 2.15

Column {
    property int lineType: RuleLine.UNDEFINED

    Rectangle {
        id: line
        width: 1
        height: parent.height
        color: "white"
    }

    Text {
        width: parent.width
        height: 15
        text: text
        color: "white"
        horizontalAlignment: Text.AlignHCenter
    }
}
