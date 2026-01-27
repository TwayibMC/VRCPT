#ifndef VRPLANET_H
#define VRPLANET_H

#include "vrsphere.h"

class VRPlanet : public VRSphere
{
public:
    VRPlanet(double r = 1.0);

    void setNightTexture(QString filename, bool hMirror = true, bool vMirror = true);
    void draw() override;

private:
    QOpenGLTexture *nightTexture;
    float time = 0.0f;
};

#endif // VRPLANET_H
