import QtQuick 2.15

Item {
    property real scaleFactor: 0.5
    Rectangle{
        width: parent.width
        height: 23
        color: "transparent"
        border{
            color: "black"
            width: 1
        }

        Rectangle{
            height: parent.height
            width: parent.width*scaleFactor
            color: "grey"
        }
    }
}
