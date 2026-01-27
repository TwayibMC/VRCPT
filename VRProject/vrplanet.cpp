#include "vrplanet.h"

VRPlanet::VRPlanet(double r)
    :VRSphere(r), nightTexture()
{
    FSFileName = ":/shaders/FS_planet_texture.fsh";
}

void VRPlanet::setNightTexture(QString filename, bool hMirror, bool vMirror)
{
    if (nightTexture)
        delete nightTexture;
    nightTexture = new QOpenGLTexture(QImage(filename).mirrored(hMirror, vMirror));
}

void VRPlanet::draw()
{
    time += 0.05f;

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
        glEnable(GL_TEXTURE0);
        glActiveTexture(GL_TEXTURE0);
        texture->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
        texture->setMagnificationFilter(QOpenGLTexture::Linear);
        //Liaison de la texture
        texture->bind();
    }

    if (nightTexture) {
        glEnable(GL_TEXTURE1);
        glActiveTexture(GL_TEXTURE1);
        nightTexture->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
        nightTexture->setMagnificationFilter(QOpenGLTexture::Linear);
        nightTexture->bind(GL_TEXTURE1);
    }

    program.bind();
    vao.bind();

    QMatrix4x4 matrix;
    matrix = camera->projectionMatrix() * camera->viewMatrix() * this->modelMatrix();
    program.setUniformValue("u_ModelViewProjectionMatrix", matrix);
    program.setUniformValue("u_Opacity", opacity);
    program.setUniformValue("u_Color", globalColor);
    program.setUniformValue("u_Daytime", time);
    if (texture)
        program.setUniformValue("texture0", 0);
    if (nightTexture)
        program.setUniformValue("texture1", 1);

    glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, nullptr);

    vao.release();
    program.release();

    if (nightTexture) {
        nightTexture->release();
        glDisable(GL_TEXTURE1);
    }

    if (texture) {
        texture->release();
        glActiveTexture(GL_TEXTURE0);
        glDisable(GL_TEXTURE0);
        glDisable(GL_TEXTURE_2D);
    }
}
