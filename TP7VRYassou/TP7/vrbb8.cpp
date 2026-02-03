#include "vrbb8.h"

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
}


void VRBB8::draw()
{
    foreach (VRMesh* mesh, meshes) {
        mesh->draw();
    }
}

void VRBB8::move(float d, QVector3D orientAct, QVector3D direction, QVector3D perp)
{
    translate(d * direction);
    QVector3D v1 = orientAct;
    QVector3D v2 = direction;
    float theta = qRadiansToDegrees(qAcos(QVector3D::dotProduct(v1,v2)));
    QVector3D rot = QVector3D(0,1,0);
    if (QVector3D::dotProduct(v1,v2) == 0){
        rot = QVector3D::crossProduct(v1,v2);
    }
    for (int i=0; i<=6; i++){
        meshes[i]->rotate(theta, rot);
    }
}
