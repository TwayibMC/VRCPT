#include "vrairplane.h"

VRAirplane::VRAirplane()
    :VRModel("./model/GeeBee2.x") //
{
    velocity = 1;
    acceleration = 1;
}

float VRAirplane::getVelocity() const { return velocity; }
void VRAirplane::setVelocity(float newVelocity) { velocity = newVelocity; }
float VRAirplane::getAcceleration() const { return acceleration; }
void VRAirplane::setAcceleration(float newAcceleration) { acceleration = newAcceleration; }

void VRAirplane::update(float time, bool relative)
{
    if (meshes.size() < 3) return;

    float dt = time * 0.001f;
    float v = velocity * dt;
    float s = acceleration * pow(dt, 2) / 2.0f;

    // Déplacement vers l'avant
    this->translate((v + s) * orientation.rotatedVector(QVector3D(0, 0, -1)));

    // Hélice
    QVector3D dir_helix = meshes[2]->getOrientation().rotatedVector(QVector3D(0, 0, -1));
    meshes[2]->rotate(v * 80.0f, dir_helix);

    this->setVelocity(velocity + acceleration * dt);

    // --- STABILISATION RÉALISTE (Anti-conflit) ---
    QVector3D rightWing = orientation.rotatedVector(QVector3D(1, 0, 0));
    QVector3D nose = orientation.rotatedVector(QVector3D(0, 0, -1));

    // 1. Stabilisation des ailes (Roulis)
    // On réduit le facteur à 1.2f pour ne pas bloquer les virages du joueur
    if (qAbs(rightWing.y()) > 0.01f) {
        this->rotate(-rightWing.y() * 1.2f, QVector3D(0, 0, 1));
    }

    // 2. Stabilisation du nez (Tangage)
    // Empêche l'avion de piquer du nez ou de monter tout seul indéfiniment
    if (qAbs(nose.y()) > 0.01f) {
        this->rotate(nose.y() * 0.8f, QVector3D(1, 0, 0));
    }
}
