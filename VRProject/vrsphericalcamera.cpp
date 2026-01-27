#include "vrsphericalcamera.h"

VRSphericalCamera::VRSphericalCamera()
{
    rho = 20;
    theta = M_PI_2;
    phi = M_PI_4/2;
    updatePosition();
}

void VRSphericalCamera::updatePosition()
{
    QVector3D cible = this->getTarget();
    float xcamera = cible.x() + rho*qCos(phi)*qCos(theta);
    float ycamera = cible.y() + rho*qSin(phi);
    float zcamera = cible.z() + rho*qCos(phi)*qSin(theta);
    this->setPosition(QVector3D(xcamera, ycamera, zcamera));
}

float VRSphericalCamera::getPhi() const
{
    return phi;
}

void VRSphericalCamera::setPhi(float newPhi)
{
    phi = qMin(M_PI_2, qMax(newPhi, -M_PI_2));
    updatePosition();
}

float VRSphericalCamera::getTheta() const
{
    return theta;
}

void VRSphericalCamera::setTheta(float newTheta)
{
    theta = newTheta;
    updatePosition();
}

float VRSphericalCamera::getRho() const
{
    return rho;
}

void VRSphericalCamera::setRho(float newRho)
{
    rho = qMax(newRho, 0.1);
    updatePosition();
}

void VRSphericalCamera::setTarget(const QVector3D &target)
{
    this->target = target;
    updatePosition();
}

void VRSphericalCamera::setPosition(const QVector3D &position)
{
    this->position = position;
    float x = position.x() - target.x();
    float y = position.y() - target.y();
    float z = position.z() - target.z();
    rho = sqrt(pow(x,2)+pow(y,2)+pow(z,2));
    theta = qAtan2(z,x);
    phi = qAsin(y/rho);
}

