#include "vrbb8.h"
#include <QtMath>

// Correction : Chemins relatifs pour le modèle et TOUTES les textures
VRBB8::VRBB8() : VRModel("model/BB8 New/bb8.obj")
{
    meshes[0]->setTexture("model/BB8 New/HEAD diff MAP.jpg"); // tete
    meshes[1]->setTexture("model/BB8 New/white dirt map.jpg"); // bas grande antenne
    meshes[2]->setTexture("model/BB8 New/white dirt map.jpg"); // haut grande antenne
    meshes[3]->setTexture("model/BB8 New/BODY self ill MAP.jpg");
    meshes[4]->setTexture("model/BB8 New/BODY self ill MAP.jpg"); // petite antenne
    meshes[5]->setTexture("model/BB8 New/HEAD diff MAP.jpg"); // anneau sous la tete
    meshes[6]->setTexture("model/BB8 New/Body diff MAP.jpg"); // corps

    meshes[3]->setOrigin(meshes[3]->getLocalCenter());
    meshes[6]->setOrigin(meshes[6]->getLocalCenter());
}


void VRBB8::draw()
{
    foreach (VRMesh* mesh, meshes) {
        mesh->draw();
    }
}

void VRBB8::move(float d, QVector3D orientAct, QVector3D direction, QVector3D perp)
{
    QVector3D flatDirection = QVector3D(direction.x(), 0.0f, direction.z());
    if (flatDirection.lengthSquared() < 0.0001f) {
        return;
    }
    flatDirection.normalize();
    translate(d * flatDirection);

    QVector3D v1 = QVector3D(orientAct.x(), 0.0f, orientAct.z()).normalized();
    QVector3D v2 = flatDirection;
    float dot = QVector3D::dotProduct(v1, v2);
    dot = qBound(-1.0f, dot, 1.0f);
    float theta = qRadiansToDegrees(qAcos(dot));
    QVector3D cross = QVector3D::crossProduct(v1, v2);
    if (QVector3D::dotProduct(cross, QVector3D(0, 1, 0)) < 0.0f) {
        theta = -theta;
    }

    QVector3D rollAxis = QVector3D::crossProduct(QVector3D(0, 1, 0), v2);
    rollAxis.normalize();
    float rollAngle = d * 10.0f;

    meshes[3]->rotate(rollAngle, rollAxis);
    meshes[6]->rotate(rollAngle, rollAxis);

    meshes[0]->rotate(theta, QVector3D(0, 1, 0));
    meshes[1]->rotate(theta, QVector3D(0, 1, 0));
    meshes[2]->rotate(theta, QVector3D(0, 1, 0));
    meshes[4]->rotate(theta, QVector3D(0, 1, 0));
    meshes[5]->rotate(theta, QVector3D(0, 1, 0));
}
