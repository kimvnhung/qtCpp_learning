import QtQuick 2.15
import Qt.labs.qmlmodels
import models 1.0

DelegateChooser{
    id: chooser

    required property bool isRelative

    DelegateChoice{

        TimeStepItem{
            required property var modelData
            model: modelData
            isRelative: chooser.isRelative
            delegate: chooser
        }
    }
}
