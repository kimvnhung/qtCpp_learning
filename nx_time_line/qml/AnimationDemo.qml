import QtQuick 2.15

Rectangle{
    id: root
    Repeater{
        id: repeater
        model: 7
        Rectangle{
            width: 1
            height: 60

            color: "red"

            x: index*root.width/repeater.count
            Text {
                text: index+"ms"
            }
        }
    }

    PropertyAnimation{
        target: root
        property: "width,x"
        duration: 500
        running: true
    }

    function getCurPosFromMouseX(parentMouseX){
        return 0
    }



    function isMaximumScale(){
        return false
    }
}
