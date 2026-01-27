#ifndef VRSKYBOX_H
#define VRSKYBOX_H

#include "vrbody.h"

class VRSkyBox : public VRBody
{
public:
    VRSkyBox();

    // VRBody interface
public:
    void draw() override;
    void initializeBuffer() override;
    void initializeVAO() override;
    void setCubeTexture(QString leftImage, QString rightImage,
                        QString frontImage, QString backImage,
                        QString topImage, QString bottomImage);
};

#endif // VRSKYBOX_H
