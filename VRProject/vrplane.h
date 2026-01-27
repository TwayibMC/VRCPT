#ifndef VRPLANE_H
#define VRPLANE_H

#include "vrbody.h"

class VRPlane : public VRBody
{
public:
    VRPlane(float length = 10.0f, float width = 10.0f);

    // VRBody interface
public:
    void draw() override;
    void initializeBuffer() override;

    float getLength() const;
    void setLength(float newLength);

    float getWidth() const;
    void setWidth(float newWidth);

private:
    float length;
    float width;
};

#endif // VRPLANE_H
