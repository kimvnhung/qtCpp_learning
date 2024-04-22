import QtQuick 2.15

Rectangle{
    id: root

    width: 2
    height: parent.height
    color: "transparent"
    x: 100

    property color cursorColor: "white"


    Rectangle{
        id: selectedArea
        anchors.fill: parent

        opacity: 0.5
        color: "#79b8d5"
    }

    Rectangle {
        id: leftCursor
        width: 2
        height: parent.height

        color: "transparent"

        Rectangle {
            width: parent.width
            height: 1
            color: root.cursorColor

            anchors{
                top: parent.top
                left: parent.left
            }
        }

        Rectangle{
            width: 1
            height: parent.height
            color: root.cursorColor

            anchors{
                top: parent.top
                left:parent.left
            }
        }

        Rectangle{
            width: parent.width
            height: 1
            color: root.cursorColor

            anchors{
                bottom: parent.bottom
                left: parent.left
            }
        }
    }

    Rectangle {
        id: rightCursor
        width: 2
        height: parent.height

        color: "transparent"

        anchors{
            top: parent.top
            right: parent.right
        }

        Rectangle {
            width: parent.width
            height: 1
            color: root.cursorColor

            anchors{
                top: parent.top
                right: parent.right
            }
        }

        Rectangle{
            width: 1
            height: parent.height
            color: root.cursorColor

            anchors{
                top: parent.top
                right: parent.right
            }
        }

        Rectangle{
            width: parent.width
            height: 1
            color: root.cursorColor

            anchors{
                bottom: parent.bottom
                right: parent.right
            }
        }
    }
}
