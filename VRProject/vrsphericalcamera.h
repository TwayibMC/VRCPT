#ifndef VRSPHERICALCAMERA_H
#define VRSPHERICALCAMERA_H

#include <QtMath>
#include "vrcamera.h"

class VRSphericalCamera : public VRCamera
{
public:
    VRSphericalCamera();

    float getPhi() const;
    void setPhi(float newPhi);
    float getTheta() const;
    void setTheta(float newTheta);
    float getRho() const;
    void setRho(float newRho);

    void setTarget(const QVector3D &target) override;
    void setPosition(const QVector3D &position) override;

private:
    float phi;
    float theta;
    float rho;

    void updatePosition();
};

#endif // VRSPHERICALCAMERA_H
