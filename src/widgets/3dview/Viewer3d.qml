/* Atelier KDE Printer Host for 3D Printing
    Copyright (C) <2017-2018>
    Author: Patrick José Pereira - patrickjp@kde.org
            Kevin Ottens - ervin@kde.org

    This program is free software; you can redistribute it and/or
    modify it under the terms of the GNU General Public License as
    published by the Free Software Foundation; either version 3 of
    the License or any later version accepted by the membership of
    KDE e.V. (or its successor approved by the membership of KDE
    e.V.), which shall act as a proxy defined in Section 14 of
    version 3 of the license.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

import QtQuick
import QtQuick.Scene3D

Item {
    id: item
    required property size bedSize
    width: 1000
    height: 1000
    signal droppedUrls (var urls)

    DropArea {
        id: dropArea
        anchors.fill: parent
        onDropped: function(drop) {
            if(drop.hasUrls) {
                item.droppedUrls(drop.urls)
            }
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
            SceneEntity {
                id: entity
                bedSize: item.bedSize
            }

        }
    }

    Text {
        objectName: "fileName"
        id: fileName
        text: ""
        onTextChanged: {
            entity.currentFile = text
        }
    }
}
