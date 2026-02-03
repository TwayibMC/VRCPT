#ifndef VRMODEL_H
#define VRMODEL_H

#include <QString>
#include <QDir>
#include <QVector>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/matrix4x4.h>

#include "vrmesh.h"

class VRModel : public VRBody
{
public:
    VRModel(QString nomFichier);

    void draw() override;
    void initializeBuffer() override;
    void initializeVAO() override {}

    void rotate(float angle, QVector3D axis) override;
    void translate(QVector3D position) override;
    void setCamera(VRCamera *camera) override;
    void setOrientation(float yaw, float pitch, float roll) override;
    void setScale(float newScale) override;
    void setLight(VRLight *newLight) override;
    void setPosition(const QVector3D &newPosition) override;
    void setOrigin(QVector3D pos) override;
    QMatrix4x4 modelMatrix() override;

protected:
    QVector<VRMesh*> meshes;
    QDir directory;
    int nbMeshes;
    int numMaterial = 0;

private:
    void loadModel(QString nomFichier);
    void processNode(aiNode* node, const aiScene* scene);
    VRMesh* processMesh(aiMesh* mesh, const aiScene* scene);
};

#endif // VRMODEL_H
