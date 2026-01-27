#ifndef VRHELIX_H
#define VRHELIX_H
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

    double radius;
    double frequency;
    double pitch;
    double numWindings;
};

#endif // VRHELIX_H
