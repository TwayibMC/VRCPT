#ifndef VRSTEREOCAMERA_H
#define VRSTEREOCAMERA_H

#define VR_CAMERA_MONO 0
#define VR_CAMERA_LEFT 1
#define VR_CAMERA_RIGHT 2

#include "vrcamera.h"

class VRStereoCamera : public VRCamera
{
public:
    VRStereoCamera();

    float getEyeDistance() const;
    void setEyeDistance(float newEyeDistance);

    float getFocalDistance() const;
    void setFocalDistance(float newFocalDistance);

    QMatrix4x4 viewMatrix();
    QMatrix4x4 projectionMatrix();

    int getCameraType() const;
    void setCameraType(int newCameraType);

protected :
    QMatrix4x4 leftViewMatrix();
    QMatrix4x4 leftProjectionMatrix();
    QMatrix4x4 rightViewMatrix();
    QMatrix4x4 rightProjectionMatrix();

private:
    float eyeDistance;
    float focalDistance;
    int cameraType;
};

#endif // VRSTEREOCAMERA_H
