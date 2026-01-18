#include "vrlight.h"

VRLight::VRLight() {
    ambient = QColor(100, 100, 100);
    diffuse = QColor(255, 255, 255);
    specular = QColor(255, 255, 255);
    position = QVector3D(10, 0, 10);
}

const QVector3D &VRLight::getPosition() const
{
    return position;
}

void VRLight::setPosition(const QVector3D &newPosition)
{
    position = newPosition;
}

const QColor &VRLight::getAmbient() const
{
    return ambient;
}

void VRLight::setAmbient(const QColor &newAmbient)
{
    ambient = newAmbient;
}

const QColor &VRLight::getDiffuse() const
{
    return diffuse;
}

void VRLight::setDiffuse(const QColor &newDiffuse)
{
    diffuse = newDiffuse;
}

const QColor &VRLight::getSpecular() const
{
    return specular;
}

void VRLight::setSpecular(const QColor &newSpecular)
{
    specular = newSpecular;
}
