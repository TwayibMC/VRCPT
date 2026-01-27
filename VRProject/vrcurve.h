#ifndef VRCURVE_H
#define VRCURVE_H
#include <QtMath>
#include "vrbody.h"


class VRCurve : public VRBody
{
public:
    VRCurve();


    virtual QVector3D pos(double t) {return QVector3D(x(t), y(t), z(t));}

    void draw() override;
    void initializeVAO() override;

    double getMinT() const;
    void setMinT(double minT);

    double getMaxT() const;
    void setMaxT(double maxT);

    int getNumSeg() const;
    void setNumSeg(int numSeg);

    float getLineWidth() const;
    void setLineWidth(float lineWidth);

protected:
    void initializeBuffer() override;

    virtual float x(double t) = 0;
    virtual float y(double t) {return 0.0f; }
    virtual float z(double t) = 0;

    double minT;
    double maxT;
    int numSeg;
    float lineWidth;
};

#endif // VRCURVE_H
