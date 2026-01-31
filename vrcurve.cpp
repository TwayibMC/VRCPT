#include "vrcurve.h"

VRCurve::VRCurve()
    :VRBody()
{
    minT = 0;
    maxT = 1;
    numSeg = 100;
    lineWidth = 2;
}

void VRCurve::draw()
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glLineWidth(lineWidth);
    //glLineStipple() //à regarder comment ça fonctionne ?
    glDisable(GL_CULL_FACE);

    program.bind();
    vao.bind();

    QMatrix4x4 matrix;
    matrix = camera->projectionMatrix() * camera->viewMatrix() * this->modelMatrix();
    program.setUniformValue("u_ModelViewProjectionMatrix", matrix);
    program.setUniformValue("u_opacity", opacity);
    program.setUniformValue("u_Color", globalColor);

    glDrawArrays(GL_LINE_STRIP, 0, numSeg+1);

    vao.release();
    program.release();
}

void VRCurve::initializeBuffer()
{
    numVertices = numSeg + 1;

    QVector3D* vertexData = new QVector3D[numVertices];
    double t = minT;
    double deltaT = (maxT - minT)/numSeg;
    for (int i = 0; i <= numSeg; i++, t += deltaT) {
        vertexData[i] = pos(t);
    }

    //remplissage du Vertex Buffer Object
    vbo.bind();
    vbo.allocate(vertexData, numVertices * int(sizeof(QVector3D)));
    vbo.setUsagePattern(QOpenGLBuffer::StaticDraw);
    vbo.release();
}

void VRCurve::initializeVAO()
{
    //Initialisation du VAO
    vao.bind();
    vbo.bind();
    ibo.bind();

    //Définition du seul attribut position (la couleur est uniforme avec le VS_simpler)
    program.setAttributeBuffer("rv_Position", GL_FLOAT, 0, 3);
    program.enableAttributeArray("rv_Position");

    //Libération
    vao.release();
    program.release();
}

float VRCurve::getLineWidth() const
{
    return lineWidth;
}

void VRCurve::setLineWidth(float lineWidth)
{
    this->lineWidth = lineWidth;
}

int VRCurve::getNumSeg() const
{
    return numSeg;
}

void VRCurve::setNumSeg(int numSeg)
{
    this->numSeg = numSeg;
}


double VRCurve::getMinT() const
{
    return minT;
}

void VRCurve::setMinT(double minT)
{
    this->minT = minT;
}

double VRCurve::getMaxT() const
{
    return maxT;
}

void VRCurve::setMaxT(double maxT)
{
    this->maxT = maxT;
}

