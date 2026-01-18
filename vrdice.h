#ifndef VRDICE_H
#define VRDICE_H

#include "vrbody.h"

class VRDice : public VRBody
{
public:
    VRDice();

    // RVBody interface
public:
    void draw() override;
    QMatrix4x4 modelMatrix() override;
    void initializeBuffer() override;
    void initializeVAO() override;
    void update(float t, bool b) override;
};

#endif // VRDICE_H
