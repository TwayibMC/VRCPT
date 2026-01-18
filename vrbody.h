#ifndef VRBODY_H
#define VRBODY_H

#include <QOpenGLFunctions>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLShaderProgram>
#include <QVector3D>
#include <QQuaternion>
#include <QMessageBox>
#include <QOpenGLTexture>

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

    void translate(QVector3D vec);
    void rotate(float angle, QVector3D axis);
    void setOrientation(float yaw, float pitch, float roll);

    void setTexture(QString textureFilename, bool hMirror = true, bool vMirror = true);

    //Tuto5
    virtual void update(float time, bool relative = true){}

    // accesseurs et mutateurs
    QVector3D getPosition() const;
    void setPosition(const QVector3D &newPosition);
    QQuaternion getOrientation() const;
    void setOrientation(const QQuaternion &newOrientation);
    int getNumVertices() const;
    int getNumTriangles() const;
    int getNumIndices() const;
    VRCamera *getCamera() const;
    void setCamera(VRCamera *newCamera);
    QString getVSFileName() const;
    void setVSFileName(const QString &newVSFileName);
    QString getFSFileName() const;
    void setFSFileName(const QString &newFSFileName);
    float getOpacity() const;
    void setOpacity(float newOpacity);
    bool getWireframe() const;
    void setWireframe(bool newWireframe);
    bool getCulling() const;
    void setCulling(bool newCulling);
    float getScale() const;
    void setScale(float newScale);
    QColor getGlobalColor() const;
    void setGlobalColor(const QColor &newGlobalColor);
    VRLight *getLight() const;
    void setLight(VRLight *newLight);
    float getSpecStrength() const;
    void setSpecStrength(float newSpecStrength);

protected:
    QString VSFileName;               //! : nom du fichier qui contient le vertex shader
    QString FSFileName;               //! : nom du fichier qui contient le fragment shader
    QOpenGLBuffer vbo;                //! : vertex buffer qui stocke les sommets du maillage
    QOpenGLBuffer ibo;                //! : index buffer (optionnel) qui stocke les index à utiliser pour le rendu
    QOpenGLVertexArrayObject vao;     //! : vertex array object (obligatoire)
    QOpenGLShaderProgram program;     //! : shader program (obligatoire)

    QVector3D position;               //! : position de l'objet dans la scène
    QQuaternion orientation;          //! : attitude de l'objet dans la scène
    float opacity = 1.0f;
    bool wireframe = false;
    bool culling = false;
    float scale = 1.0f;
    QColor globalColor;

    int numVertices;                  //! : nombre de sommets de l'objet (lecture seule)
    int numTriangles;                 //! : nombre de triangles de l'objet (lecture seule)
    int numIndices;                   //! : nombre d'index (0 si on n'utilise pas l'ibo)

    VRCamera *camera;                 //! : pointeur sur la caméra utilisée pour le rendu.
    QOpenGLTexture* texture;
    VRLight *light;
    float specStrength;

    struct VRVertex {
        QVector3D position;         //! position du sommet
        QVector2D texCoord;         //! coordonnées texture
        QVector3D normal;           //! vecteur normal

        VRVertex(QVector3D pos = QVector3D(), QVector2D tex = QVector2D(), QVector3D normalVector = QVector3D())
        {
            position = pos;
            texCoord = tex;
            normal = normalVector;
        }
    };

};

#endif // VRBODY_H
