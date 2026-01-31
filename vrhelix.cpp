#include "vrhelix.h"

VRHelix::VRHelix(double radius, double pitch)
    :VRCurve()
{
    numWindings = 3;
    this->radius = radius;
    frequency = 1;
    this->pitch = pitch;
    maxT = M_PI * 2  * numWindings;

    FSFileName = ":/shaders/FS_simple.fsh";
    VSFileName = ":/shaders/VS_simpler.vsh";
}

double VRHelix::getRadius() const
{
    return radius;
}

void VRHelix::setRadius(double radius)
{
    this->radius = radius;
}

double VRHelix::getFrequency() const
{
    return frequency;
}

void VRHelix::setFrequency(double frequency)
{
    this->frequency = frequency;
}

double VRHelix::getPitch() const
{
    return pitch;
}

void VRHelix::setPitch(double pitch)
{
    this->pitch = pitch;
}

float VRHelix::x(double t)
{
    return float(radius*qCos(frequency*t));
}

float VRHelix::y(double t)
{
    return float(pitch * frequency * t / 2 * M_PI);
}

float VRHelix::z(double t)
{
    return float(radius*qSin(frequency*t));
}

double VRHelix::getNumWindings() const
{
    return numWindings;
}

void VRHelix::setNumWindings(double numWindings)
{
    this->numWindings = numWindings;
}
