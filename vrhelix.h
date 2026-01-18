#ifndef VRHELIX_H
#define VRHELIX_H

/*!
  \file vrhelix.h
  \brief Déclaration de la classe RVHelix.
  \author  Leo Donati
  \date    14/01/2025
  \version 3.0

  Cours de Réalité Virtuelle
  Polytech'Nice Sophia

  Utilise la classe RVCurve avec l'équation paramétrée de l'hélice
*/

#include "vrcurve.h"

class VRHelix : public VRCurve
{
public:
    VRHelix(double radius = 1.0, double pitch = 0.2);

    double getRadius() const;
    void setRadius(double radius);

    double getFrequency() const;
    void setFrequency(double frequency);

    double getPitch() const;
    void setPitch(double pitch);

    double getNumWindings() const;
    void setNumWindings(double numWindings);

protected:
    float x(double t) override;
    float y(double t) override;
    float z(double t) override;

    double radius;         //!< rayon de l'hélice
    double frequency;      //!< fréquence de rotation (nb tour/s)
    double pitch;          //!< pas de l'hélice (de combien elle avance à chaque tour)
    double numWindings;    //!< nombre d'enroulements
};

#endif // VRHELIX_H
