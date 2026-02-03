#ifndef VRAIRPLANE_H
#define VRAIRPLANE_H

#include "vrmodel.h"

class VRAirplane : public VRModel
{
public:
    VRAirplane();

    float getVelocity() const;
    void setVelocity(float newVelocity);

    float getAcceleration() const;
    void setAcceleration(float newAcceleration);

    void update(float time, bool relative = true);

private:
    float velocity;
    float acceleration;
};

#endif // VRAIRPLANE_H
