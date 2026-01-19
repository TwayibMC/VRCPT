#ifndef VRBOUNCE_H
#define VRBOUNCE_H

/*!
  \file vrbounce.h
  \brief Déclaration de la classe VRBounce.
  \author  Leo Donati
  \date    14/01/2025
  \version 1.0

  Cours de Réalité Virtuelle
  Polytech'Nice Sophia

  Courbe parabolique inversée pour simuler un rebond.
*/

#include "vrcurve.h"

class VRBounce : public VRCurve
{
public:
    VRBounce(double hMax = 1.0, double tMax = 1.0);

    double getHMax() const;
    void setHMax(double hMax);

    double getTMax() const;
    void setTMax(double tMax);

protected:
    float x(double t) override;
    float y(double t) override;
    float z(double t) override;

private:
    void updateCoefficients();
    void updateMaxT();

    double hMax;
    double tMax;
    double a;
    double b;
};

#endif // VRBOUNCE_H
