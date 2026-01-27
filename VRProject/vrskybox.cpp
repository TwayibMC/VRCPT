#include "vrskybox.h"

VRSkyBox::VRSkyBox()
    :VRBody()
{
    VSFileName = ":/shaders/VS_skybox_texture.vsh";
    FSFileName = ":/shaders/FS_skybox_texture.fsh";
}


void VRSkyBox::draw()
{
    glCullFace(GL_BACK);
    glFrontFace(GL_CW);
    glEnable(GL_CULL_FACE);
    glDisable(GL_BLEND);

    //Réglages OpenGL pour la texture cubique
    if (texture) {
        glEnable(GL_TEXTURE_CUBE_MAP);
        glEnable(GL_TEXTURE0);
        texture->bind();
    }

    program.bind();
    vao.bind();

    //Définition de la variable uniforme
    QMatrix4x4 matrix;
    matrix = camera->projectionMatrix() * camera->viewMatrix() * this->modelMatrix();
    program.setUniformValue("u_ModelViewProjectionMatrix", matrix);
    program.setUniformValue("texture0", 0);

    for (int i =0; i<6; i++)
        glDrawArrays(GL_TRIANGLE_FAN, 4*i, 4);

    vao.release();
    program.release();

    if (texture) {
        texture->release();
        glDisable(GL_TEXTURE_CUBE_MAP);
        glDisable(GL_TEXTURE0);
    }
}

void VRSkyBox::initializeBuffer()
{
    //Definitions des 8 sommets du cube
    QVector3D A(-1, -1, 1);
    QVector3D B(1, -1, 1);
    QVector3D C(1, 1, 1);
    QVector3D D(-1, 1, 1);
    QVector3D E(-1, -1, -1);
    QVector3D F(1, -1, -1);
    QVector3D G(1, 1, -1);
    QVector3D H(-1, 1, -1);

    //Tableau des données : 24 sommets puis 24 couleurs
    QVector3D vertexData[] = {
        A, B, C, D, //face avant
        H, G, F, E, //face arriere
        A, D, H, E, //face gauche
        B, F, G, C, //face droite
        D, C, G, H, //face dessus
        A, E, F, B, //face dessous
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

void VRSkyBox::initializeVAO()
{
    //Initialisation du Vertex Array Object
    program.bind();
    vao.bind();
    vbo.bind();
    ibo.bind();

    //Définition des attributs
    program.setAttributeBuffer("vr_Position", GL_FLOAT, 0, 3);
    program.enableAttributeArray("vr_Position");

    //Libération
    vao.release();
    program.release();
}

// VERSION CORRIGÉE POUR COMPATIBILITÉ GPU (AMD FIX)
void VRSkyBox::setCubeTexture(QString leftImage, QString rightImage, QString frontImage, QString backImage, QString topImage, QString bottomImage)
{
    // Récupérer les fonctions OpenGL du contexte actuel
    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
    if (!f) return;

    if (texture) {
        delete texture;
        texture = nullptr;
    }

    texture = new QOpenGLTexture(QOpenGLTexture::TargetCubeMap);
    texture->create();
    texture->bind();

    // CORRECTIF AMD : Force l'alignement (via le pointeur de fonction 'f')
    f->glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    QString faces[6] = { rightImage, leftImage, topImage, bottomImage, frontImage, backImage };

    for (int i = 0; i < 6; i++) {
        QImage img = QImage(faces[i]).convertToFormat(QImage::Format_RGBA8888);
        if (img.isNull()) {
            qDebug() << "Erreur : impossible de charger la texture face" << i << faces[i];
            continue;
        }

        // Utilisation de glTexImage2D via le contexte
        f->glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA8,
                        img.width(), img.height(), 0,
                        GL_RGBA, GL_UNSIGNED_BYTE, img.bits());
    }

    f->glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    f->glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    f->glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    f->glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    f->glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    f->glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
    texture->release();
}
