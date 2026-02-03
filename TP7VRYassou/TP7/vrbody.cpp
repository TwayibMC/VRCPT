#include "vrbody.h"

VRBody::VRBody()
    :QOpenGLFunctions(),
      vao(), program(),
      position(), orientation(),
      texture()
{
    vbo = QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    ibo = QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);
    vbo.create();
    ibo.create();
    vao.create();
    program.create();

    numVertices = 0;
    numTriangles = 0;
    numIndices = 0;

    opacity = 1.0f;
    wireframe = false;
    culling = true;
    scale = 1.0f;
    globalColor = QColor(255, 255, 255);

    VSFileName = ":/shaders/VS_simple_texture.vsh";
    FSFileName = ":/shaders/FS_simple_texture.fsh";

    specStrength = 10;
    origin = QVector3D(0,0,0);
}

VRBody::~VRBody()
{
    if (program.hasOpenGLShaderPrograms())
        program.removeAllShaders();
    vao.destroy();
    ibo.destroy();
    vbo.destroy();
    if (texture)
        delete texture;
}

void VRBody::initialize()
{
    initializeShader();
    initializeBuffer();		//à écrire dans la classe fille
    initializeVAO();
}

void VRBody::initializeShader()
{
    initializeOpenGLFunctions();

    if (program.hasOpenGLShaderPrograms())
        program.removeAllShaders();
    bool resultat;

    program.bind();
    //Vertex Shader
    resultat = program.addShaderFromSourceFile(QOpenGLShader::Vertex, VSFileName);
    if (!resultat)     {
        QMessageBox msg;
        msg.setWindowTitle("Vertex shader");
        msg.setText(program.log());
        msg.exec();
        exit(EXIT_FAILURE);
    }

    //Fragment Shader
    resultat = program.addShaderFromSourceFile(QOpenGLShader::Fragment, FSFileName);
    if (!resultat)     {
        QMessageBox msg;
        msg.setWindowTitle("Fragment shader");
        msg.setText(program.log());
        msg.exec();
        exit(EXIT_FAILURE);
    }

    //Link
    resultat = program.link();
    if (!resultat)     {
        QMessageBox msg;
        msg.setWindowTitle("Link du shader program");
        msg.setText(program.log());
        msg.exec();
        exit(EXIT_FAILURE);
    }

    //Libération
    program.release();
}

void VRBody::initializeVAO()
{
    //Initialisation du VAO
    vao.bind();
    vbo.bind();
    ibo.bind();

    //Définition de l'attribut position
    program.setAttributeBuffer("vr_Position", GL_FLOAT, 0, 3, sizeof(VRVertex));
    program.enableAttributeArray("vr_Position");

    //Définition de l'attribut de coordonnée texture
    program.setAttributeBuffer("vr_TexCoord", GL_FLOAT, sizeof(VRVertex::position), 2, sizeof(VRVertex));
    program.enableAttributeArray("vr_TexCoord");

    //Définition de l'attribut de vecteur normal
    program.setAttributeBuffer("vr_Normal", GL_FLOAT, sizeof(VRVertex::position)+sizeof (VRVertex::texCoord), 3, sizeof(VRVertex));
    program.enableAttributeArray("vr_Normal");

    //Libération
    vao.release();
    program.release();
}

void VRBody::update(float time, bool relative)
{}

void VRBody::rotate(float angle, QVector3D axis)
{
    orientation =  QQuaternion::fromAxisAndAngle(axis, angle) * orientation ;
}

void VRBody::translate(QVector3D position)
{
    this->position += position;
}

void VRBody::setOrientation(float yaw, float pitch, float roll)
{
    orientation = QQuaternion::fromEulerAngles(pitch, yaw, roll);
}

QMatrix4x4 VRBody::modelMatrix()
{
    QMatrix4x4 model;
    model.translate(position);
    model.rotate(orientation);
    model.scale(scale);
    return model;
}

VRCamera *VRBody::getCamera() const
{
    return camera;
}

void VRBody::setCamera(VRCamera *newCamera)
{
    camera = newCamera;
}

const QVector3D &VRBody::getPosition() const
{
    return position;
}

void VRBody::setPosition(const QVector3D &newPosition)
{
    position = newPosition;
}

const QQuaternion &VRBody::getOrientation() const
{
    return orientation;
}

void VRBody::setOrientation(const QQuaternion &newOrientation)
{
    orientation = newOrientation;
}

int VRBody::getNumVertices() const
{
    return numVertices;
}

int VRBody::getNumTriangles() const
{
    return numTriangles;
}

int VRBody::getNumIndices() const
{
    return numIndices;
}

const QString &VRBody::getVSFileName() const
{
    return VSFileName;
}

void VRBody::setVSFileName(const QString &newVSFileName)
{
    VSFileName = newVSFileName;
}

const QString &VRBody::getFSFileName() const
{
    return FSFileName;
}

void VRBody::setFSFileName(const QString &newFSFileName)
{
    FSFileName = newFSFileName;
}

float VRBody::getOpacity() const
{
    return opacity;
}

void VRBody::setOpacity(float newOpacity)
{
    opacity = newOpacity;
}

bool VRBody::getWireframe() const
{
    return wireframe;
}

void VRBody::setWireframe(bool newWireframe)
{
    wireframe = newWireframe;
}

bool VRBody::getCulling() const
{
    return culling;
}

void VRBody::setCulling(bool newCulling)
{
    culling = newCulling;
}

float VRBody::getScale() const
{
    return scale;
}

void VRBody::setScale(float newScale)
{
    scale = newScale;
}

const QColor &VRBody::getGlobalColor() const
{
    return globalColor;
}

void VRBody::setGlobalColor(const QColor &newGlobalColor)
{
    globalColor = newGlobalColor;
}

void VRBody::setTexture(QString textureFilename, bool hMirror, bool vMirror)
{
    if (texture)
        delete texture;
    texture = new QOpenGLTexture(QImage(textureFilename).mirrored(hMirror, vMirror));
}

VRLight *VRBody::getLight() const
{
    return light;
}

void VRBody::setLight(VRLight *newLight)
{
    light = newLight;
}

float VRBody::getSpecStrength() const
{
    return specStrength;
}

void VRBody::setSpecStrength(float newSpecStrength)
{
    specStrength = newSpecStrength;
}

void VRBody::setOrigin(QVector3D pos)
{
    origin = pos;
}
