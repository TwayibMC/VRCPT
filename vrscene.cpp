#include "vrscene.h"

VRScene::VRScene(): camera() {}


void VRScene::setCamera(VRCamera *newCamera)
{
    camera = newCamera;
    foreach (VRBody* body, *this) {
        body->setCamera(camera);
    }
}

void VRScene::translate(QVector3D vec)
{
    foreach (VRBody* body, *this) {
        body->translate(vec);
    }
}

void VRScene::rotate(float angle, QVector3D axis)
{
    foreach (VRBody* body, *this) {
        body->rotate(angle, axis);
    }
}

void VRScene::draw()
{
    foreach (VRBody* body, *this) {
        body->draw();
    }
}

void VRScene::update(float t, bool relative)
{
    foreach (VRBody* body, *this) {
        body->update(t, relative);
    }
}
