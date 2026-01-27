#ifndef VRTERRAIN_H
#define VRTERRAIN_H

#include "vrsurface.h"

class VRTerrain : public VRSurface
{
public:
    VRTerrain(double width);
    void draw() override;
    void setHeightmap(QString textureFilename);

    float getHeightFactor() const;
    void setHeightFactor(float newHeightFactor);

protected :
    QOpenGLTexture *heightmap;
    float heightFactor;
    float x(double s, double t);
    float y(double s, double t);
    float z(double s, double t);
};

#endif // VRTERRAIN_H
