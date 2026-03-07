import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "MathHelper.js" as Helper

Rectangle {
    color: "white"

    property var pointAs: [
        {
            "id": 0,
        },
    ]
    property var pointBs: [
        {
            "id": 100,
        },
    ]

    Canvas {
        id: grid
        anchors.fill: parent
        onPaint: {
            var ctx = getContext("2d")
            ctx.strokeStyle = "#e2e8f0"
            ctx.lineWidth = 1

            for (var x = 0; x < width; x += 20) {
                ctx.beginPath()
                ctx.moveTo(x, 0)
                ctx.lineTo(x, height)
                ctx.stroke()
            }

            for (var y = 0; y < height; y += 20) {
                ctx.beginPath()
                ctx.moveTo(0, y)
                ctx.lineTo(width, y)
                ctx.stroke()
            }
        }
    }

    ColumnLayout {
        anchors.fill: parent
        RowLayout {
            Layout.fillWidth: true
            Layout.preferredHeight: 50

            Button {
                text: "Add point A"
            }

            Button {
                text: "Add point B"
            }
        }

        RowLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
            Rectangle {
                id: areaA
                Layout.preferredWidth: 1
                Layout.fillHeight: true
                Layout.fillWidth: true
                color: "#f5f7fb"
                border.color: "#d1d5db"
                radius: 8

                Repeater {
                    model: pointAs
                    delegate: MovablePointC {
                        pointId: modelData.id
                        pointColor: Helper.colorFromIndex(modelData.id)
                    }
                }
            }

            Rectangle {
                id: areaB
                Layout.preferredWidth: 1
                Layout.fillHeight: true
                Layout.fillWidth: true
                color: "#f5f7fb"
                border.color: "#d1d5db"
                radius: 8

                Repeater {
                    model: pointBs
                    delegate: MovablePointC {
                        pointId: modelData.id
                        pointColor: Helper.colorFromIndex(modelData.id)
                    }
                }

                DropArea {
                    id: dropAreaB
                    anchors.fill: parent
                    keys: ["movable-point-c"]

                    onEntered: function(drag){
                        console.log("Entered drop area B with point id: ", drag.source.pointId)
                    }

                    onExited: {
                        console.log("Exited drop area B with point id: ", drag.source.pointId)
                    }

                    onPositionChanged: function(drag){
                        console.log("Position changed in drop area B with point id: ", drag.source.pointId)
                    }

                    onDropped: function(drop){
                        console.log("Dropped point with id: ")
                    }

                }
            }
        }
    }
}
