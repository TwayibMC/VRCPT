/*!
  \file VRsphere.cpp
  \brief Définition de la classe VRSphere.
  \author  Leo Donati
  \date    17/12/2024
  \version 2.1

  Cours de Réalité Virtuelle
  Polytech'Nice Sophia
*/

#include "vrsphere.h"

VRSphere::VRSphere(double r)
    :VRSurface()
{
    radius = r;
    minS = 0;
    maxS = 2 * M_PI;
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

