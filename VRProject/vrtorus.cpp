#include "vrtorus.h"

VRTorus::VRTorus(double R1, double R2)
    :VRSurface()
{
    smallRadius = R1;
    bigRadius = R2;
    minS = - M_PI;
    maxS =  M_PI;
    minT = - M_PI;
    maxT =  M_PI;

    numSegS = 50;
    numSegT = 50;
}


float VRTorus::x(double s, double t)
{
    return float((bigRadius + smallRadius * qCos(t)) * qCos(s));
}

float VRTorus::y(double s, double t)
{
    return float(smallRadius * qSin(t));
}

float VRTorus::z(double s, double t)
{
    return float((bigRadius + smallRadius * qCos(t)) * qSin(s));
}

double VRTorus::getBigRadius() const
{
    return bigRadius;
}

void VRTorus::setBigRadius(double R)
{
    bigRadius = R;
}

double VRTorus::getSmallRadius() const
{
    return smallRadius;
}

void VRTorus::setSmallRadius(double r)
{
    smallRadius = r;
}
