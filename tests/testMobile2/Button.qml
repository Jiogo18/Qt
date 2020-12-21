import QtQuick 2.0

Rectangle {
    property alias text: lblButton.text//propriétée que le constructeur connais pas

    objectName: "button"
    id:rectanle
    width: 100; height: 100
    x:0; y:0
    Text {
        id: lblButton
        text: ""
        scale: 2
        color:"white"
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
    }


    MouseArea {
        id:area
        anchors.fill: parent
        onClicked: compcpp.actionButton(rectanle.objectName)
        //onPressed: console.log("button press")
        //onReleased: console.log("button release")
    }
    states: [
        State {
            id: press
            name:"light_gray"
            when:area.pressed === true
            PropertyChanges {
                target: rectanle;
                color:"#A5A5A5"
            }
        },
        State {
            id: release
            name:"gray"
            when:area.pressed === false
            PropertyChanges {
                target: rectanle;
                color:"#5A5A5A"
            }
        }

    ]
}
