#include "vrsubjectivecamera.h"
#include <QQuaternion>

VRSubjectiveCamera::VRSubjectiveCamera() : VRStereoCamera()
{
    yaw = 0.0f;
    pitch = 0.0f;
}

float VRSubjectiveCamera::getYaw() const
{
    return yaw;
}

void VRSubjectiveCamera::setYaw(float newYaw)
{
    yaw = newYaw;
    this->updateTarget();
}

float VRSubjectiveCamera::getPitch() const
{
    return pitch;
}

void VRSubjectiveCamera::setPitch(float newPitch)
{
    pitch = newPitch;
    this->updateTarget();
}

void VRSubjectiveCamera::move(float d)
{
   float y = position.y();
   position += d*(target - position);
   position.setY(y);
   updateTarget();
}

void VRSubjectiveCamera::lateral(float d)
{
    float y = position.y();
    position += d * QQuaternion::fromEulerAngles(0,-90,0).rotatedVector(target-position);
    position.setY(y);
    updateTarget();
}

void VRSubjectiveCamera::crouch(float yMin)
{
    if (yMin < position.y()) {
        position.setY(position.y() - 1);
    }
    updateTarget();
}

void VRSubjectiveCamera::getUp(float yMax)
{
    if (yMax > position.y()) {
        position.setY(position.y() + 1);
    }
    updateTarget();
}

bool VRSubjectiveCamera::jump(float yMin, float yMax, bool up)
{
    if(up){
        if (yMax > position.y()) {
            position.setY(position.y() + 1);
            //move(0.5);
        }
    }
    else{
        if (yMin < position.y()) {
            position.setY(position.y() - 1);
            //move(0.5);
        }
    }
    updateTarget();
    // Si on arrive à la fin de l'acsension
    if (position.y()==yMax and up==true){
        return false;
    }
    // Si on est dans la descente
    if (up == false){
        return false;
    }
    // Si on est dans l'acension
    return true;
}


void VRSubjectiveCamera::turn(float angle)
{
    setYaw(yaw+angle);
}

void VRSubjectiveCamera::tilt(float angle)
{
    setPitch(pitch+angle);
}

void VRSubjectiveCamera::setPosition(const QVector3D &position)
{
    this->position = position;
    this->updateTarget();
}

void VRSubjectiveCamera::updateTarget()
{
    float x = qCos(pitch) * qCos(yaw);
    float y = qSin(pitch);
    float z = - qCos(pitch) * qSin(yaw);
    this->setTarget(position + QVector3D(x,y,z));
}
