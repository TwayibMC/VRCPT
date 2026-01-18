#ifndef VRCAMERA_H
#define VRCAMERA_H

#include <QMatrix4x4>
#include <QVector3D>

class VRCamera
{
public:
    VRCamera();
    virtual ~VRCamera();

    QMatrix4x4 viewMatrix();
    QMatrix4x4 projectionMatrix();

    float getFov() const;
    void setFov(float newFov);

    float getAspect() const;
    void setAspect(float newAspect);

    float getZMin() const;
    void setZMin(float newZMin);

    float getZMax() const;
    void setZMax(float newZMax);

    QVector3D getPosition() const;
    void setPosition(const QVector3D &newPosition);

    QVector3D getTarget() const;
    void setTarget(const QVector3D &newTarget);

    QVector3D getUp() const;
    void setUp(const QVector3D &newUp);

    bool getOrthogonal() const;
    void setOrthogonal(bool newOrthogonal);

protected:
    QVector3D position;
    QVector3D target;
    QVector3D up;
    float fov;
    float aspect;
    float zMin;
    float zMax;
    bool isOrthogonal = false;
};

#endif // VRCAMERA_H
