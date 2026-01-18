#include "vrbody.h"

VRBody::VRBody()
     :QOpenGLFunctions(),
      vao(), program(),
        position(), orientation(), texture()
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

    globalColor = QColor(255, 255, 255);
    specStrength = 20;

    FSFileName = ":/shaders/FS_simple_texture.fsh";
    VSFileName = ":/shaders/VS_simple_texture.vsh";
}

VRBody::~VRBody()
{
    if (program.hasOpenGLShaderPrograms())
        program.removeAllShaders();
    if (texture)
        delete texture;
    ibo.destroy();
    vbo.destroy();
    vao.destroy();
}

QMatrix4x4 VRBody::modelMatrix()
{
    QMatrix4x4 model;
    model.translate(position);
    model.rotate(orientation);
    model.scale(this->scale);
    return model;
}

void VRBody::initialize()
{
    initializeShader();
    initializeBuffer();		//à écrire dans la classe fille obligatoirement
    initializeVAO();    	//à écrire dans la classe fille si on veut
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
    //Initialisation du Vertex Array Object
    program.bind();
    vao.bind();
    vbo.bind();
    ibo.bind();

    //Définition de l'attribut position
    program.setAttributeBuffer("rv_Position", GL_FLOAT, 0, 3, sizeof(VRVertex));
    program.enableAttributeArray("rv_Position");

    //Définition de l'attribut de coordonnée texture
    program.setAttributeBuffer("rv_TexCoord", GL_FLOAT, sizeof(VRVertex::position), 2, sizeof(VRVertex));
    program.enableAttributeArray("rv_TexCoord");

    program.setAttributeBuffer("rv_Normal", GL_FLOAT, sizeof(VRVertex::position) + sizeof(VRVertex::texCoord),  3, sizeof(VRVertex));
    program.enableAttributeArray("rv_Normal");

    //Libération
    vao.release();
    program.release();
}

// Déplace le centre de gravité de l'objet du vecteur vec
void VRBody::translate(QVector3D vec)
{
      this->position += vec;
}

void VRBody::rotate(float angle, QVector3D axis)
{
    this->orientation =  QQuaternion::fromAxisAndAngle(axis, angle) * orientation ;
}

void VRBody::setOrientation(float yaw, float pitch, float roll)
{
    this->orientation = QQuaternion::fromEulerAngles(pitch, yaw, roll);
}

QString VRBody::getVSFileName() const
{
    return VSFileName;
}

void VRBody::setVSFileName(const QString &newVSFileName)
{
    VSFileName = newVSFileName;
}

QVector3D VRBody::getPosition() const
{
    return position;
}

void VRBody::setPosition(const QVector3D &newPosition)
{
    position = newPosition;
}

QQuaternion VRBody::getOrientation() const
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

VRCamera *VRBody::getCamera() const
{
    return camera;
}

void VRBody::setCamera(VRCamera *newCamera)
{
    camera = newCamera;
}

QString VRBody::getFSFileName() const
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

QColor VRBody::getGlobalColor() const
{
    return globalColor;
}

void VRBody::setGlobalColor(const QColor &newGlobalColor)
{
    globalColor = newGlobalColor;
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

void VRBody::setTexture(QString textureFilename, bool hMirror, bool vMirror)
{
    if (texture)
        delete texture;
         texture = new QOpenGLTexture(QImage(textureFilename).mirrored(hMirror, vMirror));
}
