#ifndef VRWIDGET_H
#define VRWIDGET_H

#include <QWidget>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLFramebufferObject>
#include <QTimer>
#include <QTime>
#include <QMouseEvent>
#include <QPixmap>
#include <QVector3D>
#include <QMessageBox>

#include "vrbody.h"
#include "vrcamera.h"
#include "vrlight.h"
#include "vrscene.h"
#include "vrskybox.h"
#include "vrcurve.h"


class VRWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT

public:
    VRWidget(QWidget *parent = nullptr);
    ~VRWidget();

    QPixmap getImage();

private :
    float angleX = 30;
    float angleY = 30;
    bool animate = false;
    QTimer* timer;
    QPointF oldPos;
    VRBody* body;
    VRBody* plan;
    VRBody* world;
    VRBody* torus;
    VRScene scene;
    VRSkybox* skybox;
    VRCamera* camera;
    VRLight *light;
    QTime startTime;
    QTime lastTime;
    VRCurve* trajectory;

public slots:
    void startAnimation();
    void changeFov(int fov);
    void changeOpacity(int opacity);
    void changeCulling(bool b);
    void changeFilaire(bool b);
    void changeSaturation(int grey);
    void changeSize(int size);

private slots:
    void update();

public:
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void keyPressEvent(QKeyEvent* event) override;

    // QOpenGLWidget interface
protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;
};
#endif // VRWIDGET_H
