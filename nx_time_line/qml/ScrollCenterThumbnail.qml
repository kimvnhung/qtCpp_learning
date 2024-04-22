import QtQuick 2.15
import QtQuick.Controls.Basic

Rectangle{
    color: "transparent"
    width: 30
    height: parent.height
    anchors{
        centerIn: parent
    }
    y: (parent.height-height)/2

    Row{
        anchors.centerIn: parent
        Repeater {
            model: 3
            Rectangle{
                width: 3
                height: 16
                color: "transparent"
                Rectangle {
                    width: 1
                    height: parent.height/2
                    color: "#787878"
                    anchors.centerIn: parent
                }
            }
        }
    }

}
