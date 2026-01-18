#include "vrcamera.h"

VRCamera::VRCamera()
{
    position = QVector3D(0, 0, 0);
    target = QVector3D(0, 0, -1);
    up = QVector3D(0, 1, 0);
    fov  = 45;
    aspect = 1.33f;
    zMax = 100;
    zMin  = 0.1f;
}

VRCamera::~VRCamera()
{}


QMatrix4x4 VRCamera::viewMatrix()
{
    QMatrix4x4 view;
    view.lookAt(position, target, up);
    return view;
}

QMatrix4x4 VRCamera::projectionMatrix()
{
    QMatrix4x4 proj;

    if (isOrthogonal) {
        float bottom = -fov/2;
        float top = fov/2;
        float left = bottom * aspect;
        float right = top * aspect;
        proj.ortho(left, right, bottom, top, zMin, zMax);
    }
    else {
        proj.perspective(fov, aspect, zMin, zMax);
    }
    return proj;
}


float VRCamera::getFov() const
{
    return fov;
}

void VRCamera::setFov(float newFov)
{
    fov = newFov;
}

float VRCamera::getAspect() const
{
    return aspect;
}

void VRCamera::setAspect(float newAspect)
{
    aspect = newAspect;
}

float VRCamera::getZMin() const
{
    return zMin;
}

void VRCamera::setZMin(float newZMin)
{
    zMin = newZMin;
}

float VRCamera::getZMax() const
{
    return zMax;
}

void VRCamera::setZMax(float newZMax)
{
    zMax = newZMax;
}

QVector3D VRCamera::getPosition() const
{
    return position;
}

void VRCamera::setPosition(const QVector3D &newPosition)
{
    position = newPosition;
}

QVector3D VRCamera::getTarget() const
{
    return target;
}

void VRCamera::setTarget(const QVector3D &newTarget)
{
    target = newTarget;
}

QVector3D VRCamera::getUp() const
{
    return up;
}

void VRCamera::setUp(const QVector3D &newUp)
{
    up = newUp;
}

bool VRCamera::getOrthogonal() const
{
    return isOrthogonal;
}

void VRCamera::setOrthogonal(bool newOrthogonal)
{
    isOrthogonal = newOrthogonal;
}
