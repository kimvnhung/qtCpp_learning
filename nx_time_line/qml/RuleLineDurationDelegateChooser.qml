import QtQuick 2.15
import Qt.labs.qmlmodels

DelegateChooser {
    id: chooser
    role: 'hasSub'
    property int curHighestUnit: 5000



    DelegateChoice{
        roleValue: false
        RuleLineDurationItem{
            required property var modelData
            value: isInteger(modelData.value) ? modelData.value:1
            curHighestUnit: chooser.curHighestUnit
            delegate: chooser
        }
    }

    function isInteger(value) {
        return typeof value === 'number' && !isNaN(value) && parseInt(value) === value;
    }
}
