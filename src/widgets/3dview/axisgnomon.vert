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

#version 150 core

in vec3 vertexPosition;
in vec3 vertexNormal;
in vec4 vertexColor;

out vec3 worldPosition;
out vec3 worldNormal;
out vec4 color;

uniform mat4 modelMatrix;
uniform mat3 modelNormalMatrix;
uniform mat4 inverseViewMatrix;

uniform float aspectRatio;

uniform mat4 axisGnomonModelMatrix;
uniform mat4 axisGnomonProjectionMatrix;

void main()
{
    worldNormal = normalize(modelNormalMatrix * vertexNormal);
    worldPosition = vec3(modelMatrix * vec4(vertexPosition, 1.0));
    color = vertexColor;

    mat4 projectionMatrix = axisGnomonProjectionMatrix;
    projectionMatrix[0][0] = projectionMatrix[0][0] / aspectRatio;
    projectionMatrix[0][3] = projectionMatrix[0][3] / aspectRatio;

    mat4 viewRotation = mat4(transpose(mat3(inverseViewMatrix)));
    mat4 mvp = projectionMatrix * axisGnomonModelMatrix * viewRotation;
    gl_Position = mvp * vec4(vertexPosition, 1.0);
}
