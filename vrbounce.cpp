#include "vrbounce.h"

VRBounce::VRBounce(double hMax, double tMax)
    : VRCurve(),
    hMax(hMax),
    tMax(tMax),
    a(0.0),
    b(0.0)
{
    updateCoefficients();
    updateMaxT();

    FSFileName = ":/shaders/FS_simple.fsh";
    VSFileName = ":/shaders/VS_simpler.vsh";
}

double VRBounce::getHMax() const
{
    return hMax;
}

void VRBounce::setHMax(double hMax)
{
    this->hMax = hMax;
    updateCoefficients();
}

double VRBounce::getTMax() const
{
    return tMax;
}

void VRBounce::setTMax(double tMax)
{
    this->tMax = tMax;
    updateCoefficients();
    updateMaxT();
}

float VRBounce::x(double t)
{
    Q_UNUSED(t)
    return 0.0f;
}

float VRBounce::y(double t)
{
    return float(a * t * t + b * t);
}

float VRBounce::z(double t)
{
    Q_UNUSED(t)
    return 0.0f;
}

void VRBounce::updateCoefficients()
{
    if (tMax == 0.0) {
        a = 0.0;
        b = 0.0;
        return;
    }

    a = -hMax / (tMax * tMax);
    b = 2.0 * hMax / tMax;
}

void VRBounce::updateMaxT()
{
    minT = 0.0;
    maxT = 2.0 * tMax;
}
