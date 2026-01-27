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

    VSFileName = ":/shaders/VS_lit_texture.vsh";
    FSFileName = ":/shaders/FS_lit_texture.fsh";
}

void VRSurface::draw()
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    if (this->wireframe)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    else
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    if (culling)
        glEnable(GL_CULL_FACE);
    else
        glDisable(GL_CULL_FACE);

    if (texture) {
        glEnable(GL_TEXTURE_2D);
        glActiveTexture(GL_TEXTURE0);
        texture->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
        texture->setMagnificationFilter(QOpenGLTexture::Linear);
        texture->bind();
    }

    program.bind();
    vao.bind();

    // Vertex shader
    program.setUniformValue("u_ModelMatrix", this->modelMatrix());
    program.setUniformValue("u_ViewMatrix", camera->viewMatrix());
    program.setUniformValue("u_ProjectionMatrix", camera->projectionMatrix());
    program.setUniformValue("u_Opacity", opacity);
    program.setUniformValue("u_Color", globalColor);

    // Fragment shader
    program.setUniformValue("texture0", 0);
    program.setUniformValue("light_ambient_color", light->getAmbient());
    program.setUniformValue("light_diffuse_color", light->getDiffuse());
    program.setUniformValue("light_specular_color", light->getSpecular());
    program.setUniformValue("light_specular_strength", specStrength);
    program.setUniformValue("light_position", light->getPosition());
    program.setUniformValue("eye_position", camera->getPosition());

    glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, nullptr);

    vao.release();
    program.release();

    if (texture) {
        texture->release();
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
            vertexData[cptPoint].texCoord = QVector2D(j/float(numSegS), i/float(numSegT));
            vertexData[cptPoint].normal = normal(s,t);

            if ((i < numSegT) && (j < numSegS)) {
                // Indices du quad (i,j)
                uint i0 = cptPoint;
                uint i1 = cptPoint + 1;
                uint i2 = cptPoint + uint(numSegS) + 1;
                uint i3 = cptPoint + uint(numSegS) + 2;

                // ✅ Correction: ordre inversé (cohérent pour le culling)
                // Triangle 1 : i0, i1, i3
                indexData[cptIndex++] = i0;
                indexData[cptIndex++] = i1;
                indexData[cptIndex++] = i3;

                // Triangle 2 : i0, i3, i2
                indexData[cptIndex++] = i0;
                indexData[cptIndex++] = i3;
                indexData[cptIndex++] = i2;
            }
        }
    }

    // VBO
    vbo = QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    vbo.create();
    vbo.bind();
    vbo.allocate(vertexData, numVertices*int(sizeof (VRVertex)));
    vbo.setUsagePattern(QOpenGLBuffer::StaticDraw);
    vbo.release();

    // IBO
    ibo = QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);
    ibo.create();
    ibo.bind();
    ibo.allocate(indexData, numIndices*int(sizeof (uint)));
    ibo.setUsagePattern(QOpenGLBuffer::StaticDraw);
    ibo.release();

    // ✅ évite les fuites mémoire
    delete [] vertexData;
    delete [] indexData;
}
