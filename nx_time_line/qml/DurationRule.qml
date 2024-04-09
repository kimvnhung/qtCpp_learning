import QtQuick 2.15

Rectangle {
    property int duration: 3


    Row{
        anchors{
            top: parent.top
            left: parent.left
        }

        RuleLineEdge {
            id: leftEdge
        }
        DurationRuleContent{

        }
        RuleLineEdge{

        }
    }
}
