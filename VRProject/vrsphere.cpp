#include "vrsphere.h"

VRSphere::VRSphere(double r)
    :VRSurface()
{
    radius = r;
    minS = 0;
    maxS = 2*M_PI;
    minT = - M_PI_2;
    maxT = + M_PI_2;
}

float VRSphere::x(double s, double t)
{
    return float(radius*qCos(t)*qCos(s)) ;
}

float VRSphere::y(double s, double t)
{
    return float(radius*qSin(t));
}

float VRSphere::z(double s, double t)
{
    return float(radius* qCos(t)*qSin(s));
}

double VRSphere::getRadius() const
{
    return radius;
}

void VRSphere::setRadius(double newRadius)
{
    radius = newRadius;
}

void VRSphere::update(float time, bool relative)
{
    if (relative) {
        int vitAngulaire = 360; //en degré par seconde
        this->rotate(time*vitAngulaire*0.001, QVector3D(0, 1, 0));
    }
    else {
        int vitAngulaire = 80; //en degré par seconde
        this->rotate(vitAngulaire*0.1, QVector3D(0, 1, 0));
    }
}

