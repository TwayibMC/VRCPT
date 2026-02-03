#include "vrmesh.h"

VRMesh::VRMesh()
    :VRBody()
{
    VSFileName = ":/shaders/VS_lit_texture.vsh";
    FSFileName = ":/shaders/FS_lit_texture.fsh";
}

VRMesh::~VRMesh()
{
    delete vertices;
    delete indices;
}

void VRMesh::setVertices(QVector<VRVertex> *vertices)
{
    this->vertices = vertices;
    this->numVertices = this->vertices->length();
}

void VRMesh::setIndices(QVector<uint> *indices)
{
    this->indices = indices;
    numIndices = this->indices->length();
}

void VRMesh::initializeBuffer()
{
    vbo.bind();
    vbo.allocate(&vertices->at(0), numVertices * sizeof(VRVertex));
    vbo.setUsagePattern(QOpenGLBuffer::StaticDraw);
    vbo.release();

    ibo.bind();
    ibo.allocate(&indices->at(0), numIndices*sizeof (uint));
    ibo.setUsagePattern(QOpenGLBuffer::StaticDraw);
    ibo.release();
}

void VRMesh::draw()
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    if (this->wireframe)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    else
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    glDisable(GL_CULL_FACE);

    if (texture) {
        glEnable(GL_TEXTURE_2D);
        glEnable(GL_TEXTURE0);
        glActiveTexture(GL_TEXTURE0);
        //m_texture->setWrapMode(QOpenGLTexture::ClampToEdge);
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

    program.setUniformValue("light_ambient_color", light->getAmbient());
    program.setUniformValue("light_diffuse_color", light->getDiffuse());
    program.setUniformValue("light_specular_color", light->getSpecular());
    program.setUniformValue("light_position", light->getPosition());
    program.setUniformValue("light_specular_strength", this->getSpecStrength());

    program.setUniformValue("eye_position", camera->getPosition());

    glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, nullptr);

    vao.release();
    program.release();

    if (texture) {
        texture->release();
        glDisable(GL_TEXTURE0);
        glDisable(GL_TEXTURE_2D);
    }
}
