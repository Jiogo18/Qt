import QtQuick 2.0

Rectangle {
    id:button
    width: 100; height: 100
    x:100; y:100
    Text {
        id: lblButton
        text: qsTr(compcpp.txt())
        scale: 2
        //anchors.leftMargin: parent.left
        //anchors.topMargin: parent.top
        //anchors.centerIn: parent
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
    }

    MouseArea {
        id:area
        anchors.fill: parent
        onClicked: lblButton.text = compcpp.txt()
    }

    states: [
        State {
            id: press
            when:area.pressed === true
            PropertyChanges {
                target: button;
                color:"white"
            }
        },
        State {
            id: release
            when:area.pressed === false
            PropertyChanges {
                target: button;
                color:"red"
            }
        }

    ]
}
