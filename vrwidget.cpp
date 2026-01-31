#include "vrwidget.h"
#include "vrsphere.h"
#include "vrsphericalcamera.h"
#include "vrtorus.h"


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
    avion->setOpacity(opacity * 0.01f);
    QOpenGLWidget::update();
}

void VRWidget::changeCulling(bool b)
{
    sphere->setCulling(b);
    QOpenGLWidget::update();
}

void VRWidget::changeFilaire(bool b)
{
    sphere->setWireframe(b);
    QOpenGLWidget::update();
}

void VRWidget::changeSaturation(int g)
{
    sphere->setGlobalColor(QColor(2.55f * g, 2.55f * g, 2.55f * g ));
}

void VRWidget::changeSize(int size)
{
    avion->setScale(size * 0.01f);
}

void VRWidget::update()
{
    QTime currentTime = QTime::currentTime();
    float t = lastTime.msecsTo(currentTime);
    lastTime = currentTime;
    float t2 = startTime.msecsTo(currentTime);

    if (animate) {
        scene.update(t, true);
        sphere->rotate(2.0, QVector3D(0,1,0));
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
        torus->rotate(delta, QVector3D(0,0,1));
        //camPos.setX(camPos.x() - delta);
        break;
    case Qt::Key_Right:
        torus->rotate(-delta, QVector3D(0,0,1));
        //camPos.setX(camPos.x() + delta);
        break;
    case Qt::Key_Up:
        torus->rotate(delta, QVector3D(1,0,0));
        //camPos.setY(camPos.y() + delta);
        break;
    case Qt::Key_Down:
        torus->rotate(-delta, QVector3D(1,0,0));
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
    camera->setFov(45.0f);
    camera->setTarget(QVector3D(0, 50, -80));
    camera->setZMin(1);
    camera->setZMax(1000);
    ((VRSphericalCamera*)camera)->setRho(70);

    light = new VRLight();
    light->setPosition(QVector3D(0, 150, -80));
    light->setAmbient(QColor(50, 50, 50, 255));

    building = new VRModel("/Users/donati/Projects/VR-24/VRProject/Models/IUT_INFO.X");;
    building->setLight(light);
    building->setCulling(false);
    building->initialize();
    building->setSpecStrength(200);
    scene.append(building);


    sphere = new VRSphere(5.0);
    sphere->setPosition(QVector3D(0, 50, -80));
    sphere->setTexture(":/textures/wood.png", true, true);
    sphere->setLight(light);
    sphere->initialize();
    scene.append(sphere),

    torus = new VRTorus(1, 3);
    torus->translate(QVector3D(0, 1,0));
    torus->setFSFileName(":/shaders/FS_lit_checker.fsh");
    torus->setPosition(QVector3D(0, 30, -80));
    torus->initialize();
    torus->setLight(light);
    scene.append(torus);


    avion = new VRModel("/Users/leo/Documents/GitHub/VRProject/Models/GeeBee2.x"); // sur mon PC
    avion = new VRModel("/Users/donati/Projects/VR-24/VRProject/Models/GeeBee2.x");  //sur mon MAC
    avion->setLight(light);
    avion->setScale(3);
    avion->initialize();
    avion->translate(QVector3D(0,10,-80));
    scene.append(avion);

    scene.setCamera(camera);

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

    scene.draw();
}

void VRWidget::wheelEvent(QWheelEvent *event)
{
    VRSphericalCamera* cam = (VRSphericalCamera*) camera;
    float rho = cam->getRho() + event->angleDelta().y()*0.1f;
    cam->setRho(rho);
    QOpenGLWidget::update();
}
