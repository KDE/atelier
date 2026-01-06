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

#include "axisgnomonentity.h"

#include <QMatrix4x4>

#include <Qt3DRender/QEffect>
#include <Qt3DRender/QGraphicsApiFilter>
#include <Qt3DRender/QMaterial>
#include <Qt3DRender/QMesh>
#include <Qt3DRender/QParameter>
#include <Qt3DRender/QRenderPass>
#include <Qt3DRender/QShaderProgram>
#include <Qt3DRender/QTechnique>

using namespace Qt3DCore;
using namespace Qt3DRender;

AxisGnomonEntity::AxisGnomonEntity(QNode *parent)
    : QEntity(parent)
    , _modelMatrixParameter(new QParameter(QStringLiteral("axisGnomonModelMatrix"), QMatrix4x4()))
    , _position(0.1f, 0.1f)
    , _scale(0.1f)
{
    auto mesh = new QMesh(this);
    mesh->setSource(QUrl(QStringLiteral("qrc:/axisgnomon.gltf")));
    addComponent(mesh);

    auto material = new QMaterial(this);

    // Specify the arguments to QMatrix4x4::ortho() in OpenGL window coords.
    // Thus the position property is specified in OpenGL window coordinates
    // (origin at lower left of window).
    QMatrix4x4 projectionMatrix;
    projectionMatrix.ortho(0.0f, 1.0f, 0.0f, 1.0f, -1.0f, 1.0f);
    material->addParameter(new QParameter(QStringLiteral("axisGnomonProjectionMatrix"), projectionMatrix));
    applyModelMatrix();
    material->addParameter(_modelMatrixParameter);

    auto shaderProgram = new QShaderProgram(material);
    shaderProgram->setVertexShaderCode(QShaderProgram::loadSource(QUrl(QStringLiteral("qrc:/axisgnomon.vert"))));
    shaderProgram->setFragmentShaderCode(QShaderProgram::loadSource(QUrl(QStringLiteral("qrc:/shaders/gl3/pervertexcolor.frag"))));

    auto technique = new QTechnique(material);
    technique->graphicsApiFilter()->setApi(QGraphicsApiFilter::OpenGL);
    technique->graphicsApiFilter()->setMajorVersion(3);
    technique->graphicsApiFilter()->setMinorVersion(1);
    technique->graphicsApiFilter()->setProfile(QGraphicsApiFilter::CoreProfile);

    auto m_filterKey = new QFilterKey(material);
    m_filterKey->setName(QStringLiteral("renderingStyle"));
    m_filterKey->setValue(QStringLiteral("forward"));

    technique->addFilterKey(m_filterKey);

    auto renderPass = new QRenderPass(material);
    renderPass->setShaderProgram(shaderProgram);

    technique->addRenderPass(renderPass);

    auto effect = new QEffect(material);
    effect->addTechnique(technique);

    material->setEffect(effect);

    addComponent(material);
}

QVector2D AxisGnomonEntity::position() const
{
    return _position;
}

float AxisGnomonEntity::scale() const
{
    return _scale;
}

void AxisGnomonEntity::setPosition(const QVector2D &position)
{
    if (_position != position) {
        _position = position;
        applyModelMatrix();
        Q_EMIT positionChanged(position);
    }
}

void AxisGnomonEntity::setScale(float scale)
{
    if (!qFuzzyCompare(_scale, scale)) {
        _scale = scale;
        applyModelMatrix();
        Q_EMIT scaleChanged(scale);
    }
}

void AxisGnomonEntity::applyModelMatrix()
{
    QMatrix4x4 modelMatrix;
    modelMatrix.translate(QVector3D(_position, 0.0f));
    modelMatrix.scale(_scale);
    _modelMatrixParameter->setValue(modelMatrix);
}
