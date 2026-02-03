#ifndef VRBB8_H
#define VRBB8_H

#include "vrmodel.h"

class VRBB8 : public VRModel
{
public:
    VRBB8();
    void draw() override;
    void move(float d, QVector3D orientAct, QVector3D direction, QVector3D perp);
};

#endif // VRBB8_H
