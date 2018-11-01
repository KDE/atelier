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

import QtQuick 2.7
import Qt3D.Core 2.0
import Qt3D.Render 2.0
import Qt3D.Input 2.0
import Qt3D.Extras 2.0
import Atelier 1.0

Entity {
    id: sceneRoot
    property string currentFile

    BedProperties {
        id: bedProperties
    }

    Camera {
        id: camera
        fieldOfView: 45
        position: Qt.vector3d( 10.0, -10.0, 35.0 )
        upVector: Qt.vector3d( 0.0, 0.85, 0.75 )
        viewCenter: Qt.vector3d( 10.0, 10.0, 0.0 )
    }

    CameraController { camera: camera }

    components: [
        RenderSettings {
            activeFrameGraph: ForwardRenderer {
                camera: camera
                frustumCulling: false
            }
        },
        InputSettings { }
    ]

    Entity {
        id: gridEntity
        components: [
            PhongMaterial { ambient: "darkBlue" },
            GridMesh {
                meshResolution: Qt.size(Math.floor(bedProperties.width / 10),
                                        Math.floor(bedProperties.depth / 10))
            },
            Transform {
                scale3D: Qt.vector3d(bedProperties.width / 10,
                                     bedProperties.depth / 10,
                                     1)
            }
        ]
    }

    Entity {
        id: lineEntity
        components: [
            PhongMaterial { ambient: "darkGreen" },
            LineMesh {
                readonly property string currentFile: sceneRoot.currentFile
                onCurrentFileChanged: readAndRun(currentFile)
            }
        ]
    }

    AxisGnomonEntity {
        scale: 0.05
        position: Qt.vector2d(0.075, 0.075)
    }
}
