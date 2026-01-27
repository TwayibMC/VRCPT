#ifndef VRTEXCUBE_H
#define VRTEXCUBE_H

#include "vrbody.h"

class VRTexCube : public VRBody
{
public:
    VRTexCube();

    // VRBody interface
public:
    void draw() override;
    QMatrix4x4 modelMatrix() override;
    void initializeBuffer() override;
    void initializeVAO() override;
};

#endif // VRTEXCUBE_H
