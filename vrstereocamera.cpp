#include "vrstereocamera.h"
#include <QtMath>

VRStereoCamera::VRStereoCamera()
    : VRCamera(),
    eyeDistance(4.0f),
    focalDistance(60.0f),
    cameraType(RV_CAMERA_MONO)
{}

float VRStereoCamera::getEyeDistance() const
{
    return eyeDistance;
}

void VRStereoCamera::setEyeDistance(float newEyeDistance)
{
    eyeDistance = newEyeDistance;
}

float VRStereoCamera::getFocalDistance() const
{
    return focalDistance;
}

void VRStereoCamera::setFocalDistance(float newFocalDistance)
{
    focalDistance = newFocalDistance;
}

int VRStereoCamera::getCameraType() const
{
    return cameraType;
}

void VRStereoCamera::setCameraType(int newCameraType)
{
    cameraType = newCameraType;
}

QMatrix4x4 VRStereoCamera::viewMatrix()
{
    switch (cameraType) {
    case RV_CAMERA_LEFT:
        return leftViewMatrix();
    case RV_CAMERA_RIGHT:
        return rightViewMatrix();
    case RV_CAMERA_MONO:
    default:
        return VRCamera::viewMatrix();
    }
}

QMatrix4x4 VRStereoCamera::projectionMatrix()
{
    switch (cameraType) {
    case RV_CAMERA_LEFT:
        return leftProjectionMatrix();
    case RV_CAMERA_RIGHT:
        return rightProjectionMatrix();
    case RV_CAMERA_MONO:
    default:
        return VRCamera::projectionMatrix();
    }
}

QMatrix4x4 VRStereoCamera::leftViewMatrix()
{
    QVector3D viewDir = (target - position);
    QVector3D right = QVector3D::crossProduct(viewDir, up).normalized();
    QVector3D offset = right * (eyeDistance * 0.5f);

    QVector3D leftPos = position - offset;
    QVector3D leftTarget = target - offset;

    QMatrix4x4 view;
    view.lookAt(leftPos, leftTarget, up);
    return view;
}

QMatrix4x4 VRStereoCamera::rightViewMatrix()
{
    QVector3D viewDir = (target - position);
    QVector3D right = QVector3D::crossProduct(viewDir, up).normalized();
    QVector3D offset = right * (eyeDistance * 0.5f);

    QVector3D rightPos = position + offset;
    QVector3D rightTarget = target + offset;

    QMatrix4x4 view;
    view.lookAt(rightPos, rightTarget, up);
    return view;
}

QMatrix4x4 VRStereoCamera::leftProjectionMatrix()
{
    QMatrix4x4 proj;

    float top = zMin * qTan(qDegreesToRadians(fov * 0.5f));
    float bottom = -top;
    float shift = (eyeDistance * 0.5f) * zMin / focalDistance;
    float left = -top * aspect + shift;
    float right = top * aspect + shift;

    proj.frustum(left, right, bottom, top, zMin, zMax);
    return proj;
}

QMatrix4x4 VRStereoCamera::rightProjectionMatrix()
{
    QMatrix4x4 proj;

    float top = zMin * qTan(qDegreesToRadians(fov * 0.5f));
    float bottom = -top;
    float shift = (eyeDistance * 0.5f) * zMin / focalDistance;
    float left = -top * aspect - shift;
    float right = top * aspect - shift;

    proj.frustum(left, right, bottom, top, zMin, zMax);
    return proj;
}
