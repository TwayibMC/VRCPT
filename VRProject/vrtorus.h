#ifndef VRTORUS_H
#define VRTORUS_H
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
