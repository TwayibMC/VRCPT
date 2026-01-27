#include "vrplane.h"

VRPlane::VRPlane(float newLength, float newWidth)
    :VRBody()
{
    width = newWidth;
    length = newLength;
    globalColor = QColor(0, 255, 0);

    FSFileName = ":/shaders/FS_lit_texture.fsh";
    VSFileName = ":/shaders/VS_lit_texture.vsh";
}

void VRPlane::draw()
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    if (culling)
        glEnable(GL_CULL_FACE);
    else
        glDisable(GL_CULL_FACE);

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

    program.setUniformValue("u_ModelMatrix", this->modelMatrix());
    program.setUniformValue("u_ViewMatrix", camera->viewMatrix());
    program.setUniformValue("u_ProjectionMatrix", camera->projectionMatrix());
    program.setUniformValue("u_Opacity", opacity);
    program.setUniformValue("u_Color", globalColor);
    program.setUniformValue("texture0", 0);

    QColor ambientColor = light ? light->getAmbient() : QColor(100, 100, 100);
    QColor diffuseColor = light ? light->getDiffuse() : QColor(255, 255, 255);
    QColor specularColor = light ? light->getSpecular() : QColor(255, 255, 255);
    QVector3D lightPosition = light ? light->getPosition() : QVector3D(10, 0, 10);

    program.setUniformValue("light_ambient_color", ambientColor);
    program.setUniformValue("light_diffuse_color", diffuseColor);
    program.setUniformValue("light_specular_color", specularColor);
    program.setUniformValue("light_specular_strength", 20.0f);
    program.setUniformValue("light_position", lightPosition);
    program.setUniformValue("eye_position", camera->getPosition());

    if (wireframe) {
        glDrawArrays(GL_LINE_LOOP, 0, 4);
    }
    else {
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    }

    if (texture) {
        texture->release();
        glDisable(GL_TEXTURE0);
        glDisable(GL_TEXTURE_2D);
    }

    vao.release();
    program.release();
}

void VRPlane::initializeBuffer()
{
    float w = width/2;
    float l = length/2;

    QVector3D A(-l/2, 0, w/2);
    QVector3D B(l/2, 0, w/2);
    QVector3D C(l/2, 0, -w/2);
    QVector3D D(-l/2, 0, -w/2);

    QVector2D SW(0, 0);
    QVector2D SE(1, 0);
    QVector2D NE(1, 1);
    QVector2D NW(0, 1);

    QVector3D up(0, 1, 0);

    VRVertex vertexData[] = {
        VRVertex(A, SW, up),
        VRVertex(B, SE, up),
        VRVertex(C, NE, up),
        VRVertex(D, NW, up)
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

    numVertices = 4;
    numTriangles = 2;
    numIndices = 0;

}




