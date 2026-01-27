#include "vrtexcube.h"

VRTexCube::VRTexCube()
    :VRBody()
{
    VSFileName = ":/shaders/VS_lit_texture_cube.vsh";
    FSFileName = ":/shaders/FS_lit_texture.fsh";
}


void VRTexCube::draw()
{
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    if (wireframe)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    else
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    if (culling)
        glEnable(GL_CULL_FACE);
    else
        glDisable(GL_CULL_FACE);

    //Réglages OpenGL pour les textures
    if (texture) {
        glEnable(GL_TEXTURE_2D);
        glEnable(GL_TEXTURE0);
        texture->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
        texture->setMagnificationFilter(QOpenGLTexture::Linear);
        //Liaison de la texture
        texture->bind();
    }

    program.bind();
    vao.bind();

    //Variables uniformes pour le vertex shader
    program.setUniformValue("u_ModelMatrix", this->modelMatrix());
    program.setUniformValue("u_ViewMatrix", camera->viewMatrix());
    program.setUniformValue("u_ProjectionMatrix", camera->projectionMatrix());
    program.setUniformValue("u_Opacity", opacity);
    program.setUniformValue("u_Color", globalColor);

    //Variables uniformes pour le fragment shader
    program.setUniformValue("texture0", 0);
    program.setUniformValue("light_ambient_color", light->getAmbient());
    program.setUniformValue("light_diffuse_color", light->getDiffuse());
    program.setUniformValue("light_specular_color", light->getSpecular());
    program.setUniformValue("light_specular_strength", specStrength);
    program.setUniformValue("light_position", light->getPosition());
    program.setUniformValue("eye_position", camera->getPosition());

    for (int i =0; i<6; i++)
        glDrawArrays(GL_TRIANGLE_FAN, 4*i, 4);

    vao.release();
    program.release();

    if (texture) {
        texture->release();
        glDisable(GL_TEXTURE0);
        glDisable(GL_TEXTURE_2D);
        glActiveTexture(GL_TEXTURE0);
    }
}

QMatrix4x4 VRTexCube::modelMatrix()
{
    QMatrix4x4 model;
    model.translate(-0.5f, -0.5f, -0.5f);
    return VRBody::modelMatrix() * model;
}

void VRTexCube::initializeBuffer()
{
    //Definitions des 8 sommets du cube
    QVector3D A(0, 0, 1);
    QVector3D B(1, 0, 1);
    QVector3D C(1, 1, 1);
    QVector3D D(0, 1, 1);
    QVector3D E(0, 0, 0);
    QVector3D F(1, 0, 0);
    QVector3D G(1, 1, 0);
    QVector3D H(0, 1, 0);

    //Définitions des 6 couleurs primaires
    QVector3D rouge(1, 0, 0);
    QVector3D vert(0, 1, 0);
    QVector3D bleu(0, 0, 1);
    QVector3D cyan(0, 1, 1);
    QVector3D magenta(1, 0, 1);
    QVector3D jaune(1, 1, 0);

    //Définitions de 3 coordonnées texture des sommets de chaque face
    QVector3D SW(1, 0, 0);
    QVector3D SE(0, 0, 0);
    QVector3D NE(0, 1, 0);
    QVector3D NW(1, 1, 0);

    //Définition des 6 vecteiurs normaux
    QVector3D posX(1, 0, 0);
    QVector3D negX(-1, 0, 0);
    QVector3D posY(0, 1, 0);
    QVector3D negY(0, -1, 0);
    QVector3D posZ(0, 0, 1);
    QVector3D negZ(0, 0, -1);

    //Tableau des données : 24 sommets puis 24 couleurs puis 24 coord texture
    QVector3D vertexData[] = {
        A, B, C, D, //face avant
        H, G, F, E, //face arriere
        A, D, H, E, //face gauche
        B, F, G, C, //face droite
        D, C, G, H, //face dessus
        A, E, F, B, //face dessous
        rouge, rouge, rouge, rouge,
        cyan, cyan, cyan, cyan,
        vert, vert, vert, vert,
        magenta, magenta, magenta, magenta,
        bleu, bleu, bleu, bleu,
        jaune, jaune, jaune, jaune,
        SW, SE, NE, NW,
        SW, SE, NE, NW,
        SW, SE, NE, NW,
        SW, SE, NE, NW,
        SW, SE, NE, NW,
        SW, SE, NE, NW,
        posZ, posZ, posZ, posZ,
        negZ, negZ, negZ, negZ,
        negX, negX, negX, negX,
        posX, posX, posX, posX,
        posY, posY, posY, posY,
        negY, negY, negY, negY
    };

    //Initialisation du Vertex Buffer Object
    vbo = QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    //Création du VBO
    vbo.create();
    //Lien du VBO avec le contexte de rendu OpenGL
    vbo.bind();
    //Allocation des données dans le VBO
    vbo.allocate(vertexData, sizeof (vertexData));
    vbo.setUsagePattern(QOpenGLBuffer::StaticDraw);
    //Libération du VBO
    vbo.release();

    numVertices = 24;
    numTriangles = 12;
    numIndices = 0;
}

void VRTexCube::initializeVAO()
{
    //Initialisation du Vertex Array Object
    program.bind();
    vao.bind();
    vbo.bind();
    ibo.bind();

    //Définition des attributs
    program.setAttributeBuffer("vr_Position", GL_FLOAT, 0, 3);
    program.enableAttributeArray("vr_Position");

    program.setAttributeBuffer("vr_Color", GL_FLOAT, sizeof(QVector3D)*numVertices, 3);
    program.enableAttributeArray("vr_Color");

    program.setAttributeBuffer("vr_TexCoord", GL_FLOAT, 2*sizeof(QVector3D)*numVertices, 3);
    program.enableAttributeArray("vr_TexCoord");

    program.setAttributeBuffer("vr_Normal", GL_FLOAT, 3*sizeof(QVector3D)*numVertices, 3);
    program.enableAttributeArray("vr_Normal");
    //Libération
    vao.release();
    program.release();
}
