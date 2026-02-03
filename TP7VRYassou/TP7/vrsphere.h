#ifndef VRSPHERE_H
#define VRSPHERE_H
#include "vrsurface.h"

class VRSphere : public VRSurface
{
public:
    VRSphere(double radius = 1.0);

    double getRadius() const;
    void setRadius(double newRadius);
    void update(float time, bool relative = true) override;

protected:
    float x(double s, double t) override;
    float y(double s, double t) override;
    float z(double s, double t) override;

    double radius;
};

#endif // VRSPHERE_H
