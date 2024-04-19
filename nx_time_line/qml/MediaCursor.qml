import QtQuick 2.15

Rectangle{
    id: mediaCursor

    width: 2
    height: parent.height
    color: "transparent"
    x: 100


    Rectangle{
        id: selectedArea
        anchors.fill: parent

        opacity: 0.3
        color: "blue"
    }

    Rectangle {
        id: leftCursor
        width: 2
        height: parent.height

        color: "transparent"

        Rectangle {
            width: parent.width
            height: 1
            color: "white"

            anchors{
                top: parent.top
                left: parent.left
            }
        }

        Rectangle{
            width: 1
            height: parent.height
            color: "white"

            anchors{
                top: parent.top
                left:parent.left
            }
        }

        Rectangle{
            width: parent.width
            height: 1
            color: "white"

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
            color: "white"

            anchors{
                top: parent.top
                right: parent.right
            }
        }

        Rectangle{
            width: 1
            height: parent.height
            color: "white"

            anchors{
                top: parent.top
                right: parent.right
            }
        }

        Rectangle{
            width: parent.width
            height: 1
            color: "white"

            anchors{
                bottom: parent.bottom
                right: parent.right
            }
        }
    }
}
