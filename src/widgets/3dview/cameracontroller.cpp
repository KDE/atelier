/* Atelier KDE Printer Host for 3D Printing
    Copyright (C) <2018>
    Author: Kevin Ottens - ervin@kde.org

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

#include "cameracontroller.h"

#include <Qt3DRender/QCamera>

namespace
{
inline float clampInputs(float input1, float input2)
{
    float axisValue = input1 + input2;
    return (axisValue < -1) ? -1 : (axisValue > 1) ? 1 : axisValue;
}

inline float zoomDistance(QVector3D firstPoint, QVector3D secondPoint)
{
    return (secondPoint - firstPoint).lengthSquared();
}
}

CameraController::CameraController(QNode *parent)
    : Qt3DExtras::QAbstractCameraController(parent)
{
}

void CameraController::moveCamera(const Qt3DExtras::QAbstractCameraController::InputState &state, float dt)
{
    auto cam = camera();
    if (!cam) {
        return;
    }

    // Mouse
    if (state.leftMouseButtonActive) {
        cam->pan(state.rxAxisValue * lookSpeed() * dt);
        cam->tilt(state.ryAxisValue * lookSpeed() * dt);
    }

    // Keyboard
    cam->panAboutViewCenter((state.txAxisValue * lookSpeed()) * dt, QVector3D(0.0f, 0.0f, 1.0f));
    cam->translate(QVector3D(0.0f, 0.0f, state.tyAxisValue * linearSpeed() * dt),
                   Qt3DRender::QCamera::DontTranslateViewCenter);
}
