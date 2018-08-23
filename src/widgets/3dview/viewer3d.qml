import QtQuick 2.0
import QtQuick.Scene3D 2.0
import LineMesh 1.0

Item {
    id: item
    width: 1000
    height: 1000
    signal droppedUrls (var urls)

    DropArea {
        id: dropArea
        anchors.fill: parent
        onDropped: if(drop.hasUrls) {
            droppedUrls(drop.urls)
        }
    }

    Rectangle {
        id: scene
        anchors.fill: parent
        Scene3D {
            id: scene3d
            anchors.fill: parent
            focus: true
            aspects: ["input", "logic"]
            cameraAspectRatioMode: Scene3D.AutomaticAspectRatio
            AnimatedEntity {
                id: entity
            }

        }
    }

    Text {
        objectName: "fileName"
        id: fileName
        text: ""
        onTextChanged: {
            entity.runLineMesh(text)
        }
    }
}
