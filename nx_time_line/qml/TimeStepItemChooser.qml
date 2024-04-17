import QtQuick 2.15
import Qt.labs.qmlmodels
import models 1.0

DelegateChooser{
    id: chooser

    DelegateChoice{

        TimeStepItem{
            required property var modelData
            model: modelData
            delegate: chooser
        }
    }
}
