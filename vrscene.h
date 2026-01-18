#ifndef VRSCENE_H
#define VRSCENE_H

#include <QList>

#include "vrbody.h"
#include "vrcamera.h"

class VRScene : public QList<VRBody*>
{
public:
    VRScene();

    void setCamera(VRCamera *newCamera);
    void translate(QVector3D vec);
    void rotate(float angle, QVector3D axis);
    void draw();

    void update(float time, bool relative = true);

private:
    VRCamera* camera;
};
#endif // VRSCENE_H
