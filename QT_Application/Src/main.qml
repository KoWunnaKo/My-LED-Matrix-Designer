import QtQuick 2.9
import QtQuick.Controls 2.2

ApplicationWindow {
    visible: true
    width: 900
    height: 900
    title: qsTr("Scroll")
    Rectangle{
        anchors.fill: parent
        color: "black"
    }

    Item {
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: panel.top
        anchors.leftMargin: parent.width / 5
        //anchors.rightMargin:  30
        anchors.topMargin: 20
        anchors.bottomMargin: 100
        id: main
        function onPaint(x, y)
        {
            //console.log("x: "+ x+" y: "+y)
            var cellWidth = Math.floor(maingrid.width / maingrid.columns )
            var cellheight = Math.floor(maingrid.height / maingrid.rows)
            var indexA = Math.floor(x / cellWidth)
            var indexB = Math.floor(y / cellheight)
            //console.log("indexA: "+ indexA+" indexB: "+indexB)
            var rect = repeater.itemAt((indexA)+(indexB*maingrid.columns))
//            if(rect.color === "red")
                rect.color = "#ff3d3d"
//            else
//                rect.color = "red"

        }

        Grid{
            id: maingrid

            spacing: 10
            rows: 7
            columns: 5
            Repeater{
                id: repeater
                model: maingrid.rows * maingrid.columns
                Rectangle{
                    //id: c1
                    //anchors.centerIn: parent
                    property int m_height: main.height / maingrid.columns
                    property int m_width: main.width / maingrid.rows

                    property int minSize: (m_height < m_width ) ? m_height : m_width
                    height: minSize
                    width: minSize
                    radius: minSize
                    color: "#c1c1c1"
                    property bool dontSend: false
                    onColorChanged: {

                        myNetwork.array = ""
                        for (var i = 0; i < repeater.count; i++)
                        {
                            var rect = repeater.itemAt(i)
                            if(rect.color == "#c1c1c1")
                                myNetwork.array += "0"
                            else
                                myNetwork.array += "1"

                            if( i != repeater.count - 1)
                                myNetwork.array += ","
                        }
                        myNetwork.array.replace("#ff0000", "0");
                        myNetwork.array.replace("#0000ff", "1");
                        if(!dontSend)
                        myNetwork.sendUDP("<Matrix>"+myNetwork.array)
                        console.log("array : "+ myNetwork.array)
                    }

                }
            }


        }
        MouseArea{
            anchors.fill: maingrid
            onMouseXChanged: {
                if( mouseX > x && mouseX < width && mouseY > y && mouseY < height)
                    main.onPaint(mouseX, mouseY)
            }

            onMouseYChanged: {
                if( mouseX > x && mouseX < width && mouseY > y && mouseY < height)
                    main.onPaint(mouseX, mouseY)
            }

            onPressed: {

            }
        }

    }

    Button{
        id: panel
        anchors.bottom: parent.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        height: 50
        width: 100
        text: "Clear All"
        onClicked: {
            for (var i = 0; i < repeater.count; i++)
            {
                var rect = repeater.itemAt(i)
    //            if(rect.color === "red")
                rect.dontSend = true
                rect.color = "#c1c1c1"
                rect.dontSend = false

            }
            myNetwork.sendUDP("<Matrix>clear")

        }
    }




}
