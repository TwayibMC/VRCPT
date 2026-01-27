#include "vrcamera.h"

VRCamera::VRCamera()
{
    position = QVector3D(0, 0, 0);
    target = QVector3D(0, 0, -1);
    up = QVector3D(0, 1, 0);

    aspect = 1.33f;
    zMax = 100.0f;
    zMin  = 0.1f;
    fov = 45.0f;

    orthogonal = false;
}

VRCamera::~VRCamera()
{
}

QMatrix4x4 VRCamera::viewMatrix()
{
    QMatrix4x4 view;
    view.lookAt(position, target, up);
    return view;
}

QMatrix4x4 VRCamera::projectionMatrix()
{
    QMatrix4x4 proj;
    if (!orthogonal)
        proj.perspective(fov, aspect, zMin, zMax);
    else
    {
        float bottom = -fov/2;
        float top = fov/2;
        float left = bottom * aspect;
        float right = top * aspect;
        proj.ortho(left, right, bottom, top, 0.0f, zMax);
    }
    return proj;
}

QVector3D VRCamera::getPosition() const
{
    return position;
}

void VRCamera::setPosition(const QVector3D &position)
{
    this->position = position;
}

QVector3D VRCamera::getTarget() const
{
    return target;
}

void VRCamera::setTarget(const QVector3D &target)
{
    this->target = target;
}

QVector3D VRCamera::getUp() const
{
    return up;
}

void VRCamera::setUp(const QVector3D &up)
{
    this->up = up;
}

float VRCamera::getFov() const
{
    return fov;
}

void VRCamera::setFov(float fov)
{
    this->fov = fov;
}

float VRCamera::getAspect() const
{
    return aspect;
}

void VRCamera::setAspect(float aspect)
{
    this->aspect = aspect;
}

float VRCamera::getZMin() const
{
    return zMin;
}

void VRCamera::setZMin(float zMin)
{
    this->zMin = zMin;
}

float VRCamera::getZMax() const
{
    return zMax;
}

void VRCamera::setZMax(float zMax)
{
    this->zMax = zMax;
}

bool VRCamera::isOrthogonal() const
{
    return orthogonal;
}

void VRCamera::setOrthogonal(bool newOrthogonal)
{
    orthogonal = newOrthogonal;
}

