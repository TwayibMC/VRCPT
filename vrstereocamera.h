#ifndef VRSTEREOCAMERA_H
#define VRSTEREOCAMERA_H

/*!
  \file vrstereocamera.h
  \brief Déclaration de la classe VRStereoCamera.
  \author  Leo Donati
  \date    14/01/2025
  \version 1.0

  Cours de Réalité Virtuelle
  Polytech'Nice Sophia

  Caméra stéréoscopique dérivée de VRCamera.
*/

#include "vrcamera.h"

#define RV_CAMERA_MONO 0
#define RV_CAMERA_LEFT 1
#define RV_CAMERA_RIGHT 2

class VRStereoCamera : public VRCamera
{
public:
    VRStereoCamera();

    float getEyeDistance() const;
    void setEyeDistance(float newEyeDistance);

    float getFocalDistance() const;
    void setFocalDistance(float newFocalDistance);

    int getCameraType() const;
    void setCameraType(int newCameraType);

    QMatrix4x4 viewMatrix() override;
    QMatrix4x4 projectionMatrix() override;

protected:
    QMatrix4x4 leftViewMatrix();
    QMatrix4x4 rightViewMatrix();
    QMatrix4x4 leftProjectionMatrix();
    QMatrix4x4 rightProjectionMatrix();

private:
    float eyeDistance;
    float focalDistance;
    int cameraType;
};

#endif // VRSTEREOCAMERA_H
