#include "vrwidget.h"
#include "vrdice.h"
#include "vrplane.h"
#include "vrsphere.h"
#include "vrsphericalcamera.h"
#include "vrtorus.h"
#include "vrhelix.h"
#include "vrbounce.h"
#include <cmath>



VRWidget::VRWidget(QWidget *parent)
    : QOpenGLWidget(parent), QOpenGLFunctions(), scene()
{
    timer = new QTimer();
    grabKeyboard();
}

VRWidget::~VRWidget() {}

QPixmap VRWidget::getImage()
{
    makeCurrent();

    QOpenGLFramebufferObject* fbo = new QOpenGLFramebufferObject(this->width(), this->height());
    fbo->setAttachment(QOpenGLFramebufferObject::CombinedDepthStencil);
    fbo->bind();
    glViewport(0, 0, this->width(), this->height());
    paintGL();
    QImage img(fbo->toImage(true));
    fbo->release();

    return QPixmap::fromImage(img);
}

void VRWidget::startAnimation()
{
    animate = !animate;
    startTime = QTime::currentTime();
    lastTime = QTime::currentTime();
    QOpenGLWidget::update();
}

void VRWidget::changeFov(int fov)
{
    //this->fov = fov;
    camera->setFov(fov);
    QOpenGLWidget::update();
}

void VRWidget::changeOpacity(int opacity)
{
    this->world->setOpacity(opacity * 0.01f);
    QOpenGLWidget::update();
}

void VRWidget::changeCulling(bool b)
{
    this->body->setCulling(b);
    QOpenGLWidget::update();
}

void VRWidget::changeFilaire(bool b)
{
    this->world->setWireframe(b);
    QOpenGLWidget::update();
}

void VRWidget::changeSaturation(int g)
{
    body->setGlobalColor(QColor(2.55f * g, 2.55f * g, 2.55f * g ));
}

void VRWidget::changeSize(int size)
{
    plan->setScale(size * 0.01f);
}

void VRWidget::update()
{
    QTime currentTime = QTime::currentTime();
    float t = lastTime.msecsTo(currentTime);
    lastTime = currentTime;
    float t2 = startTime.msecsTo(currentTime);

    if (animate) {
        scene.update(t, true);
        double elapsedSeconds = t2 * 0.001;
        double bounceDuration = trajectory->getMaxT();
        double bounceTime = bounceDuration > 0.0 ? std::fmod(elapsedSeconds, bounceDuration) : elapsedSeconds;
        world->setPosition(trajectory->pos(bounceTime));
    }

    QOpenGLWidget::update();
}

void VRWidget::mousePressEvent(QMouseEvent *event)
{
    oldPos = event->position();
}

void VRWidget::mouseMoveEvent(QMouseEvent *event)
{
    QPointF newPos = event->position();
    float dx = (newPos.x() - oldPos.x())/width();
    float dy = (newPos.y() - oldPos.y())/height();

    if (event->buttons() & Qt::LeftButton) {
        VRSphericalCamera* cam = (VRSphericalCamera*)camera;
        cam->setPhi(cam->getPhi() + dy);
        cam->setTheta(cam->getTheta() + dx);
        QOpenGLWidget::update();
    }

    oldPos = newPos;
}

void VRWidget::keyPressEvent(QKeyEvent *event)
{
    //QVector3D camPos = camera->getPosition();
    float delta = 0.4f;
    switch (event->key())
    {
    case Qt::Key_Left:
        plan->rotate(delta, QVector3D(0,0,1));
        //camPos.setX(camPos.x() - delta);
        break;
    case Qt::Key_Right:
        plan->rotate(-delta, QVector3D(0,0,1));
        //camPos.setX(camPos.x() + delta);
        break;
    case Qt::Key_Up:
        plan->rotate(delta, QVector3D(1,0,0));
        //camPos.setY(camPos.y() + delta);
        break;
    case Qt::Key_Down:
        plan->rotate(-delta, QVector3D(1,0,0));
        //camPos.setY(camPos.y() - delta);
        break;
    }
    //camera->setPosition(camPos);
    QOpenGLWidget::update();
}

void VRWidget::initializeGL()
{
    initializeOpenGLFunctions();

    glClearColor(0.0f, 0.566f, 0.867f, 1.0f);

    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);
    glDisable(GL_CULL_FACE); //Culling désactivé par défaut

    glEnable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    camera = new VRSphericalCamera();
    //camera->setPosition(QVector3D(0, 8, 8));
    camera->setFov(45.0f);
    camera->setTarget(QVector3D());
    camera->setZMin(8.0f);
    camera->setZMax(250);

    light = new VRLight();

    body = new VRDice();
    body->setCamera(camera);
    body->setPosition(QVector3D(0, 4, 0));
    body->setTexture(":/textures/dice_texture.jpg", false, false);
    body->setLight(light);
    body->initialize();
    scene.append(body);

    plan = new VRPlane();
    plan->setCamera(camera);
    plan->setTexture(":/textures/wood.png");
    plan->initialize();
    plan->setLight(light);
    scene.append(plan);

    world = new VRSphere();
    world->setCamera(camera);
    world->setPosition(QVector3D(0, 0, 0));
    world->setTexture(":/textures/2k_earth_daymap");
    world->initialize();
    world->setLight(light);
    scene.append(world);

    torus = new VRTorus(1, 3);
    torus->setCamera(camera);
    torus->translate(QVector3D(0, 1,0));
    torus->setFSFileName(":/shaders/FS_lit_checker.fsh");
    torus->initialize();
    torus->setLight(light);
    scene.append(torus);

    skybox = new VRSkybox();
    skybox->setCubeTexture(":/textures/skybox/left.jpg",
                           ":/textures/skybox/right.jpg",
                           ":/textures/skybox/front.jpg",
                           ":/textures/skybox/back.jpg",
                           ":/textures/skybox/top.jpg",
                           ":/textures/skybox/bottom.jpg"
                           );
    skybox->setCamera(camera);
    skybox->setPosition(QVector3D());
    skybox->setScale(80);
    skybox->initialize();
    scene.append(skybox);

    trajectory = new VRBounce(3.0, 1.2);
    trajectory->setCamera(camera);
    trajectory->setPosition(QVector3D(0, 0, 0));
    trajectory->initialize();
    scene.append(trajectory);


    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    timer->start(30);
}

void VRWidget::resizeGL(int w, int h)
{
    float aspect = ((float)w)/h;
    camera->setAspect(aspect);
}

void VRWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //body->draw();
    //plan->draw();
    //world->draw();
    //torus->draw();
    scene.draw();
}
