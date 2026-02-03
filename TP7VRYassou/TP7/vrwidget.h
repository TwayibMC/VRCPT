#ifndef VRWIDGET_H
#define VRWIDGET_H

#include <QWidget>
#include <QMessageBox>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QWheelEvent>
#include <QTimer>
#include <QTime>
#include <QPainter>

#include "vrbody.h"
#include "vrsphere.h"
#include "vrtorus.h"
#include "vrscene.h"
#include "vrsphericalcamera.h"
#include "vrlight.h"
#include "vrmodel.h"
#include "vrsubjectivecamera.h"
#include "vrairplane.h"
#include "vrbb8.h"


class VRWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT

public:
    VRWidget(QWidget *parent = nullptr);
    ~VRWidget();

    // Attributs pour le rendu
private:
    VRBody* building;
    VRBody* sphere;
    VRBody* torus;
    VRAirplane* avion;
    VRBB8* bb8;

    VRScene scene;
    QString typeCam = "subjective";
    VRCamera* camera;
    VRSubjectiveCamera* camera2;
    VRCamera* camera3;
    bool isCrouched = false;
    bool isCrouching = false;
    bool isGettingUp = false;
    bool isJumping = false;
    bool up = true;
    VRLight* light;

    QTime startTime;
    QTime lastTime;

    QTimer* timer;
    float angleY;
    bool animationOn = false;
    float angleX;
    QPointF oldPos;
    int nbOfPaint = 0;
    float fps = 0.0f;

    QVector3D action;

protected slots:
    void update();

public slots:
    void startAnimation();
    void changeFov(int);
    void setSubjectiveCam();
    void setSpheriqueCamBB8();
    void setSpheriqueCamAvion();

    // QOpenGLWidget interface
protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

    // QWidget interface
protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void keyPressEvent(QKeyEvent* event) override;
    void wheelEvent(QWheelEvent *event) override;
};
#endif // VRWIDGET_H
