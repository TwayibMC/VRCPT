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

    //Accesseurs
    QVector3D getPosition() const;
    virtual void setPosition(const QVector3D &position);
    QVector3D getTarget() const;
    void setTarget(const QVector3D &target);
    QVector3D getUp() const;
    void setUp(const QVector3D &up);
    float getFov() const;
    void setFov(float fov);
    float getAspect() const;
    void setAspect(float aspect);
    float getZMin() const;
    void setZMin(float zMin);
    float getZMax() const;
    void setZMax(float zMax);

    bool getIsOrthogonal() const;
    void setIsOrthogonal(bool newIsOrthogonal);

    bool isOrthogonal() const;
    void setOrthogonal(bool newOrthogonal);

protected:
    QVector3D position;
    QVector3D target;
    QVector3D up;
    float fov;
    float aspect;
    float zMin;
    float zMax;
    bool orthogonal;
};

#endif // VRCAMERA_H
