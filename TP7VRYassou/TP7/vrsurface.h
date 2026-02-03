#ifndef VRSURFACE_H
#define VRSURFACE_H
#include <QtMath>
#include <QtDebug>
#include "vrbody.h"

class VRSurface : public VRBody
{
public:
    VRSurface();
    void draw() override;
    void initializeBuffer() override;

    virtual QVector3D pos(double s, double t) {
        return QVector3D(x(s,t), y(s,t), z(s,t));
    }

    QVector3D normal(double s, double t)
    {
        QVector3D n = -QVector3D::crossProduct(gradS(s,t), gradT(s,t));
        n.normalize();
        return n;
    }

protected:
    virtual float x(double s, double t) = 0;
    virtual float y(double s, double t) = 0;
    virtual float z(double s, double t) = 0;

    QVector3D gradS(double s, double t) { return pos(s+ds,t)-pos(s,t); }
    QVector3D gradT(double s, double t) { return pos(s,t+dt)-pos(s,t); }

    double minT, maxT;
    int numSegT;
    double minS, maxS;
    int numSegS;
    float time = 0;
    double ds = 0.1;
    double dt = 0.1;
};
#endif // VRSURFACE_H
