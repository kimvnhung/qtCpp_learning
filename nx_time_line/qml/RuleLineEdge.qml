import QtQuick 2.15
import models 1.0

Column {
    id: root
    property int lineType: RuleLine.UNDEFINED
    property int value: 1//ms

    Rectangle {
        id: line
        width: 1
        height: getHeightFromType(lineType)
        color: "white"
        opacity: 1-parent.lineType*0.25
    }

    Text {
        width: 1
        height: 15
        text: convertMillisecondsToString(value)
        color: root.lineType != RuleLine.SMALLEST? "white":"transparent"
        horizontalAlignment: Text.AlignHCenter
        opacity: 1-parent.lineType*0.25
    }

    onValueChanged: {
        if(value === 11000){
            console.log("value:"+value+"; convertMiliToString: "+convertMillisecondsToString(value))
        }
    }

    function getHeightFromType(type) : real {
        switch(type){
        case RuleLine.HIGHEST:
            return 15
        case RuleLine.NORMAL:
            return 11
        case RuleLine.SMALL:
            return 7
        case RuleLine.SMALLEST:
            return 3
        default:
            return 0
        }
    }

    function convertMillisecondsToString(duration) {
        // Convert milliseconds to seconds, minutes, hours, and remaining milliseconds
        var totalHours = Math.floor(duration / (1000 * 60 * 60));
        var totalMinutes = Math.floor((duration % (1000 * 60 * 60)) / (1000 * 60));
        var totalSeconds = Math.floor((duration % (1000 * 60)) / 1000);
        var totalMilliseconds = duration % 1000;

        if(totalMilliseconds > 0){
            return totalMilliseconds + "ms";
        }else if (totalSeconds > 0) {
            return totalSeconds + "s";
        } else if (totalMinutes > 0) {
            return totalMinutes + "m";
        } else if (totalHours > 0) {
            return totalHours + "h";
        }
    }
}
