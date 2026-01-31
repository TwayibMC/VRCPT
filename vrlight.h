#ifndef VRLIGHT_H
#define VRLIGHT_H

#include <QVector3D>
#include <QColor>

class VRLight
{
public:
    VRLight();

    const QVector3D &getPosition() const;
    void setPosition(const QVector3D &newPosition);
    const QColor &getAmbient() const;
    void setAmbient(const QColor &newAmbient);
    const QColor &getDiffuse() const;
    void setDiffuse(const QColor &newDiffuse);
    const QColor &getSpecular() const;
    void setSpecular(const QColor &newSpecular);

private:
    QVector3D position;
    QColor ambient;
    QColor diffuse;
    QColor specular;
};

#endif // VRLIGHT_H
