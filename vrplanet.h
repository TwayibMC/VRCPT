#ifndef VRPLANET_H
#define VRPLANET_H

#include "vrsphere.h"

class VRPlanet : public VRSphere
{
public:
    VRPlanet();
    void setNightTexture(QString textureFilename, bool hMirror = true, bool vMirror = true);

private:
    QOpenGLTexture* nightTexture;

    // VRBody interface
public:
    void draw() override;
};

#endif // VRPLANET_H
