#ifndef VRMESH_H
#define VRMESH_H

#include "vrbody.h"

class VRMesh : public VRBody
{
public:
    VRMesh();
    ~VRMesh() override;

    void initializeBuffer() override;
    void draw() override;

    void setVertices(QVector<VRVertex> *vertices);
    void setIndices(QVector<uint> *indices);
    QVector3D getLocalCenter() const;

protected:
    QVector<VRVertex> *vertices;
    QVector<uint> *indices;
};

#endif // VRMESH_H
