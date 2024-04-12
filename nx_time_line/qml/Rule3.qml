import QtQuick 2.15
import QtQuick.Controls.Basic

Slider {
    id: control
    width: parent.width
    height: 50

    background: Canvas {
        x: control.leftPadding + (control.horizontal ? handle.width * 0.5 : (control.availableWidth - width) / 2)
        y: control.topPadding + (control.horizontal ? (control.availableHeight - height) / 2 : 0)
        implicitWidth: control.horizontal ? 200 : control.width
        implicitHeight: control.horizontal ? control.height : 200
        width: control.horizontal ? control.availableWidth - handle.width : implicitWidth
        height: control.horizontal ? implicitHeight : control.availableHeight - handle.height

        onPaint: {
            var ctx = getContext("2d")
            ctx.clearRect(0, 0, width, height)

            ctx.strokeStyle = Qt.rgba(1, 0, 0, 0.4)
            ctx.lineWidth = 3

            ctx.beginPath()
            ctx.moveTo(0, 0)
            ctx.lineTo(width, 0)
            ctx.lineTo(width, height)
            ctx.lineTo(0, height)
            ctx.lineTo(0, 0)
            for(var x=0;x < width;x += 40) //assuming 40 is unit of measurement
            {
                ctx.moveTo(x, height)
                if(x % 120 == 0)
                    ctx.lineTo(x, height * 0.3)
                else
                    ctx.lineTo(x, height * 0.6)
            }
            ctx.stroke();
        }
    }

    handle: Canvas {
        id: handle
        x: control.leftPadding + (control.horizontal ? control.visualPosition * (control.availableWidth - width) : 0)
        y: control.topPadding + (control.horizontal ? 0 : control.visualPosition * (control.availableHeight - height))
        implicitWidth: control.height
        implicitHeight: 28

        onPaint: {
            var ctx = getContext("2d")
            ctx.clearRect(0, 0, width, height)

            ctx.strokeStyle = Qt.rgba(1, 0, 0, 0.4)
            ctx.fillStyle = "white"
            ctx.lineWidth = 1

            ctx.beginPath()
            ctx.moveTo(0, 0)
            ctx.lineTo(width, 0)
            ctx.lineTo(width, height - width * 0.5)
            ctx.lineTo(width * 0.5, height)
            ctx.lineTo(0, height - width * 0.5)
            ctx.lineTo(0, 0)

            ctx.fill()
            ctx.stroke()
        }
    }

    function getCurPosFromMouseX(parentMouseX){
        return 0
    }



    function isMaximumScale(){
        return false
    }
}
