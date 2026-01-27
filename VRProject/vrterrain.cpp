#include "vrterrain.h"

VRTerrain::VRTerrain(double width): VRSurface()
{
    minS = -width/2;
    maxS = +width/2;
    minT = -width/2;
    maxT = +width/2;
    numSegS = 50;
    numSegT = 50;

    VSFileName = ":/shaders/VS_heightmap.vsh";
}

void VRTerrain::draw()
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    if (this->wireframe)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    else
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    // ✅ Correction: pas de culling sur le terrain (évite les zones “vides”)
    glDisable(GL_CULL_FACE);

    if (texture) {
        glEnable(GL_TEXTURE_2D);
        glActiveTexture(GL_TEXTURE0);
        texture->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
        texture->setMagnificationFilter(QOpenGLTexture::Linear);
        texture->bind();
    }

    if (heightmap) {
        glEnable(GL_TEXTURE_2D);
        glActiveTexture(GL_TEXTURE1);
        heightmap->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
        heightmap->setMagnificationFilter(QOpenGLTexture::Linear);
        heightmap->bind(1); // ✅ 1 (pas GL_TEXTURE1)
    }

    program.bind();
    vao.bind();

    // Vertex shader
    program.setUniformValue("u_ModelMatrix", this->modelMatrix());
    program.setUniformValue("u_ViewMatrix", camera->viewMatrix());
    program.setUniformValue("u_ProjectionMatrix", camera->projectionMatrix());
    program.setUniformValue("u_Opacity", opacity);
    program.setUniformValue("u_Color", globalColor);
    program.setUniformValue("u_heightmap", 1);
    program.setUniformValue("u_facteur", heightFactor);

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

    if (heightmap) {
        heightmap->release();
        glDisable(GL_TEXTURE_2D);
    }
}

void VRTerrain::setHeightmap(QString textureFilename)
{
    heightmap = new QOpenGLTexture(QImage(textureFilename).mirrored());
}

float VRTerrain::getHeightFactor() const
{
    return heightFactor;
}

void VRTerrain::setHeightFactor(float newHeightFactor)
{
    heightFactor = newHeightFactor;
}

float VRTerrain::x(double s, double t)
{
    return s;
}

float VRTerrain::y(double s, double t)
{
    return 0;
}

float VRTerrain::z(double s, double t)
{
    return t;
}
