#ifndef VRBODY_H
#define VRBODY_H

#include <QOpenGLFunctions>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QMatrix4x4>
#include <QVector3D>      // AJOUTÉ : Pour le type QVector3D
#include <QQuaternion>    // AJOUTÉ : Pour corriger l'erreur "incomplete type"
#include <QMessageBox>
#include <QMessageBox>
#include <QColor>

#include "vrcamera.h"
#include "vrlight.h"


class VRBody : public QOpenGLFunctions
{
public:
    VRBody();
    virtual ~VRBody();

    virtual void draw() = 0;
    virtual QMatrix4x4 modelMatrix();

    void initialize();
    void initializeShader();
    virtual void initializeBuffer() = 0;
    virtual void initializeVAO();

    virtual void update(float time, bool relative = true);

    // pour bouger l'objet
    virtual void rotate(float angle, QVector3D axis);
    virtual void translate(QVector3D position);
    virtual void setOrientation(float yaw, float pitch, float roll);

    // accesseurs et mutateurs .....
    VRCamera *getCamera() const;
    virtual void setCamera(VRCamera *newCamera);

    const QVector3D &getPosition() const;
    virtual void setPosition(const QVector3D &newPosition);

    const QQuaternion &getOrientation() const;
    virtual void setOrientation(const QQuaternion &newOrientation);

    int getNumVertices() const;

    int getNumTriangles() const;

    int getNumIndices() const;

    const QString &getVSFileName() const;
    void setVSFileName(const QString &newVSFileName);

    const QString &getFSFileName() const;
    void setFSFileName(const QString &newFSFileName);

    float getOpacity() const;
    void setOpacity(float newOpacity);

    bool getWireframe() const;
    void setWireframe(bool newWireframe);

    bool getCulling() const;
    void setCulling(bool newCulling);

    float getScale() const;
    virtual void setScale(float newScale);

    const QColor &getGlobalColor() const;
    void setGlobalColor(const QColor &newGlobalColor);

    void setTexture(QString textureFilename, bool hMirror = true, bool vMirror = true);

    VRLight *getLight() const;
    virtual void setLight(VRLight *newLight);
    float getSpecStrength() const;
    void setSpecStrength(float newSpecStrength);

protected:
    QString VSFileName;
    QString FSFileName;
    QOpenGLBuffer vbo;
    QOpenGLBuffer ibo;
    QOpenGLVertexArrayObject vao;
    QOpenGLShaderProgram program;

    QVector3D position;
    QQuaternion orientation;

    int numVertices;
    int numTriangles;
    int numIndices;

    float opacity;
    bool wireframe;
    bool culling;
    float scale;
    QColor globalColor;

    VRCamera *camera;
    QOpenGLTexture* texture;

    VRLight* light;
    float specStrength;
    struct VRVertex {
        QVector3D position;
        QVector2D texCoord;
        QVector3D normal;

        VRVertex(QVector3D pos = QVector3D(), QVector2D tex = QVector2D(), QVector3D normalVector = QVector3D())
        {
            position = pos;
            texCoord = tex;
            normal = normalVector;
        }
    };
};

#endif // VRBODY_H
