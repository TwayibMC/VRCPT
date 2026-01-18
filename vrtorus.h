#ifndef VRTORUS_H
#define VRTORUS_H

/*!
  \file VRtorus.h
  \brief Déclaration de la classe VRTorus
  \author  Leo Donati
  \date    17/12/2024
  \version 2.1

  Cours de Réalité Virtuelle
  Polytech'Nice Sophia

  Utilise la classe VRSurface avec une représentation paramètrique du tore
*/

#include "vrsurface.h"

class VRTorus : public VRSurface
{
public:
    VRTorus(double smallRadius = 1.0, double bigRadius = 4.0);

    double getBigRadius() const;
    void setBigRadius(double R);

    double getSmallRadius() const;
    void setSmallRadius(double R);

protected:
    float x(double s, double t) override;
    float y(double s, double t) override;
    float z(double s, double t) override;

    double bigRadius;
    double smallRadius;
};

#endif // VRTORUS_H
