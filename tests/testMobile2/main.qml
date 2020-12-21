import QtQuick 2.7
import QtQuick.Window 2.2
//import Car 1.0


Window
{
    visible: true
    width: 300
    height: 420
    title: qsTr("Appli de test pour mobile")

    /*Button { text:"<"; objectName:"button_left"; x:50; y:300; width:50; height:50 }
    Button { text:"/\\"; objectName:"button_forward"; x:100; y:250; width:50; height:50 }
    Button { text:">"; objectName:"button_right"; x:150; y:300; width:50; height:50 }
    Button { text:"\\/"; objectName:"button_back"; x:100; y:300; width:50; height:50 }*/
    //Voiture {source: ":/image/preferences-desktop-screensaver-2.png"}
    ListView {
        //Button2 {}
        id:listeItems
        /*Voit {
            x:100;
            y:100;
            width:200; height:100
        }
        ButtonM { text:"bouton !"; x:100 }*/
    }
    //Plateau {}
}
