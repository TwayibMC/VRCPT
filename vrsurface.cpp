/*!
  \file VRsurface.cpp
  \brief Définition de la classe VRSurface.
  \author  Leo Donati
  \date    17/12/2024
  \version 2.1

  Cours de Réalité Virtuelle
  Polytech'Nice Sophia
*/

#include "vrsurface.h"

VRSurface::VRSurface()
    :VRBody()
{
    minS = 0;
    maxS = 1;
    minT = 0;
    maxT = 1;
    numSegS = 25;
    numSegT = 25;

    FSFileName = ":/shaders/FS_lit_texture.fsh";
    VSFileName = ":/shaders/VS_lit_texture.vsh";
}

void VRSurface::draw()
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

    program.setUniformValue("light_ambient_color", QColor(100, 100, 100));
    program.setUniformValue("light_diffuse_color", QColor(255, 255, 255));
    program.setUniformValue("light_specular_color", QColor(255, 255, 255));
    program.setUniformValue("light_specular_strength", 20.0f);
    program.setUniformValue("light_position", QVector3D(10, 0, 10));
    program.setUniformValue("eye_position", camera->getPosition());

    if (texture)
        program.setUniformValue("texture0", 0);

    if (this->wireframe)
        glDrawElements(GL_LINES, numIndices, GL_UNSIGNED_INT, nullptr);
    else
        glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, nullptr);

    vao.release();
    program.release();

    if (texture) {
        texture->release();
        glDisable(GL_TEXTURE0);
        glDisable(GL_TEXTURE_2D);
    }
}

void VRSurface::initializeBuffer()
{
    numVertices = (numSegS + 1) * (numSegT + 1);
    numTriangles = 2 * numSegS * numSegT;
    numIndices = 3 * numTriangles;

    VRVertex * vertexData = new VRVertex [numVertices];
    uint* indexData = new uint[numIndices];
    double t = minT;
    double s = minS;
    double deltaT = (maxT - minT)/numSegT;
    double deltaS = (maxS - minS)/numSegS;
    uint cptPoint = 0;
    uint cptIndex = 0;
    for (int i = 0; i <= numSegT; i++, t += deltaT) {
        s = minS;
        for (int j = 0; j <= numSegS; j++, s += deltaS, cptPoint++) {
            vertexData[cptPoint].position = pos(s,t);
            vertexData[cptPoint].texCoord = QVector2D(j/float(numSegS),i/float(numSegT));
            vertexData[cptPoint].normal = normal(s,t);
            if ((i < numSegT) && (j < numSegS)) {
                indexData[cptIndex++] = cptPoint;
                indexData[cptIndex++] = cptPoint + uint(numSegS) + 1;
                indexData[cptIndex++] = cptPoint + uint(numSegS) + 2;
                indexData[cptIndex++] = cptPoint;
                indexData[cptIndex++] = cptPoint + uint(numSegS) + 2;
                indexData[cptIndex++] = cptPoint + 1;
            }
        }
    }

    //Initialisation et remplissage du Vertex Buffer Object
    vbo = QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    vbo.create();
    vbo.bind();
    vbo.allocate(vertexData, numVertices*int(sizeof (VRVertex)));
    vbo.setUsagePattern(QOpenGLBuffer::StaticDraw);
    vbo.release();

    //Initialisation et remplissage de l'Index Buffer Object
    ibo = QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);
    ibo.create();
    ibo.bind();
    ibo.allocate(indexData, numIndices*int(sizeof (uint)));
    ibo.setUsagePattern(QOpenGLBuffer::StaticDraw);
    ibo.release();
}


