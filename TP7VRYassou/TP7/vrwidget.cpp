#include "vrwidget.h"

VRWidget::VRWidget(QWidget *parent)
    : QOpenGLWidget(parent), QOpenGLFunctions(), scene()
{
    timer = new QTimer();
    angleY = 5;

    grabKeyboard();

    lastTime = QTime::currentTime();
    startTime = QTime::currentTime();

    action = QVector3D(-1,0,0);
}

VRWidget::~VRWidget()
{
}

void VRWidget::update()
{
    QTime currentTime = QTime::currentTime();
    float t = lastTime.msecsTo(currentTime);
    lastTime = currentTime;
    if (animationOn) {
        scene.update(t, true);
    }
    if(typeCam == "subjective"){
        if (isJumping && !isCrouched){
            up = camera2->jump(50,90,up);
            if ((camera->getPosition()).y() == 50){
                isJumping = false;
            }
        }
        if (isCrouching){
            camera2->crouch(20);
        }
        if (isGettingUp){
            camera2->getUp(50);
        }
    }
    if(typeCam == "avion"){
        QVector3D pos_avion = avion->getPosition();
        QQuaternion or_avion = avion->getOrientation();
        camera3->setPosition(pos_avion - 20*or_avion.rotatedVector(QVector3D(0,0,-1)) + 4*or_avion.rotatedVector(QVector3D(0,1,0)));
        camera3->setTarget(pos_avion);
    }

    QOpenGLWidget::update();
}

void VRWidget::changeFov(int fov)
{
    camera->setFov(fov);
    camera2->setFov(fov);
    QOpenGLWidget::update();
}


void VRWidget::setSubjectiveCam()
{
    typeCam = "subjective";
    scene.setCamera(camera2);
    QOpenGLWidget::update();
}

void VRWidget::setSpheriqueCamBB8()
{
    typeCam = "bb8";
    scene.setCamera(camera);
    dynamic_cast<VRSphericalCamera*>(camera)->setPosition(bb8->getPosition() + QVector3D(40,35,0));
    camera->setTarget(bb8->getPosition()+ QVector3D(0,35,0));
    QOpenGLWidget::update();
}

void VRWidget::setSpheriqueCamAvion()
{
    typeCam = "avion";
    scene.setCamera(camera3);
    QVector3D pos_avion = avion->getPosition();
    QQuaternion or_avion = avion->getOrientation();
    camera->setPosition(pos_avion - 20*or_avion.rotatedVector(QVector3D(0,0,-1)) + 4*or_avion.rotatedVector(QVector3D(0,1,0)));
    camera->setTarget(pos_avion);
    QOpenGLWidget::update();
}

void VRWidget::startAnimation()
{
    animationOn = !animationOn;
}


void VRWidget::initializeGL()
{
    initializeOpenGLFunctions();

    glClearColor(0.0f, 0.566f, 0.867f, 1.0f);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);


    camera2 = new VRSubjectiveCamera();
    camera2->setZMax(2000);
    camera2->setPosition(QVector3D(0, 50, -130));
    camera2->setFov(45.0f);
    camera2->setZMin(1);

    light = new VRLight();
    light->setPosition(QVector3D(0, 150, -80));
    light->setAmbient(QColor(180, 180, 180, 255));

    // On utilise un chemin relatif simple
    building = new VRModel("model/IUT_INFO.X");    building->setCamera(camera2);
    building->setLight(light);
    building->setCulling(false);
    building->initialize();

    sphere = new VRSphere(5.0);
    sphere->setCamera(camera2);
    sphere->setPosition(QVector3D(0, 50, -80));
    sphere->setTexture(":/textures/wood.png", true, true);
    sphere->setLight(light);
    sphere->initialize();

    torus = new VRTorus();
    torus->setCamera(camera2);
    torus->setScale(5);
    torus->setPosition(QVector3D(200, 50, -80));
    torus->setFSFileName(":/shaders/FS_lit_checker.fsh");
    torus->setLight(light);
    torus->initialize();

    avion = new VRAirplane();
    avion->setCamera(camera3);
    avion->setLight(light);
    avion->initialize();
    avion->translate(QVector3D(0,20,-20));

    camera3 = new VRSphericalCamera();
    camera3->setFov(60.0f);
    camera3->setZMin(8);
    camera3->setZMax(1000);

    bb8 = new VRBB8();
    bb8->setCamera(camera);
    bb8->setPosition(QVector3D(0, 0, -100));
    bb8->setOrigin(QVector3D(-20,60,0));
    bb8->setLight(light);
    bb8->setScale(0.4f);
    bb8->initialize();

    camera = new VRSphericalCamera();
    camera->setFov(70.0f);
    camera->setZMin(1);
    camera->setZMax(1000);
    camera->setTarget(bb8->getPosition()+QVector3D(0,35,0));
    camera->setPosition(bb8->getPosition() + QVector3D(40,35,0));
    ((VRSphericalCamera*)camera)->setRho(70);

    scene.append(building);
    scene.append(sphere);
    scene.append(torus);
    scene.append(avion);
    scene.append(bb8);
    scene.setCamera(camera2);

    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    timer->start(10);
}

void VRWidget::resizeGL(int w, int h)
{
    float aspect = ((float)w)/h;
    camera->setAspect(aspect);
    camera2->setAspect(aspect);
    glViewport(0, 0, w, h);
}

void VRWidget::paintGL()
{
    QPainter p(this);

    //début du code OpenGL
    p.beginNativePainting();
    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    scene.draw();
    glDisable(GL_DEPTH_TEST);
    p.endNativePainting();
    //fin du code OpenGL


    nbOfPaint ++;

    //Calcul du fps
    if (nbOfPaint > 100)
    {
        QTime currentTime = QTime::currentTime();
        float elapsedTime = startTime.msecsTo(currentTime) * 0.001f;
        fps = nbOfPaint/elapsedTime;
        nbOfPaint = 0;
        startTime = currentTime;
    }
    p.setPen(Qt::yellow);
    p.setFont(QFont("Arial", 12));
    p.drawText(5, 5, width(), height(), Qt::AlignLeft, "Fps = " + QString::number(fps, 'f',2) );

}

void VRWidget::mousePressEvent(QMouseEvent *event)
{
    oldPos = event->position();
}

void VRWidget::mouseMoveEvent(QMouseEvent *event)
{

    float dx = (event->position().x() - oldPos.x()) / width();
    float dy = (event->position().y() - oldPos.y()) / height();
    if (event->buttons() & Qt::LeftButton) {
        if(typeCam == "subjective"){
            VRSubjectiveCamera* cam2 = (VRSubjectiveCamera*)camera2;
            cam2->turn(dx);
            cam2->tilt(dy);
        }
        if(typeCam=="bb8") {
            VRSphericalCamera* cam = (VRSphericalCamera*)camera;
            cam->setPhi(cam->getPhi() + dy);
            cam->setTheta(cam->getTheta() + dx);
        }
        else{
            VRSphericalCamera* cam3 = (VRSphericalCamera*)camera3;
            cam3->setPhi(cam3->getPhi() + dy);
            cam3->setTheta(cam3->getTheta() + dx);
        }
        QOpenGLWidget::update();
    }
    oldPos = event->pos();
}

void VRWidget::keyPressEvent(QKeyEvent *event)
{
    //QVector3D camPos = avion->getPosition();
    float delta = 2;
    switch (event->key())
    {
    case Qt::Key_C:
        if (!isCrouched){
            isCrouching = true;
            isGettingUp = false;
        }
        else{
            isCrouching = false;
            isGettingUp = true;
        }
        isCrouched = !isCrouched;
        break;

    case Qt::Key_Space:
        if (!isCrouched){
            isJumping = true;
            up = true;
        }
        break;
    case Qt::Key_Left:
        camera2->lateral(-delta);
        break;
    case Qt::Key_Right:
        camera2->lateral(delta);
        break;
    case Qt::Key_Up:
        camera2->move(delta);
        break;
    case Qt::Key_Down:
        camera2->move(-delta);
        break;
    case Qt::Key_W:
        {
        avion->setAcceleration(1.1*avion->getAcceleration());
        avion->setVelocity(avion->getVelocity()*2);
        break;
        }
    case Qt::Key_X:
        avion->setAcceleration(0.9*avion->getAcceleration());
        avion->setVelocity(avion->getVelocity()/2);
        break;
    case Qt::Key_Z:
        if(typeCam == "bb8"){
            dynamic_cast<VRBB8*>(bb8)->move(delta, action, QVector3D(-1,0,0), QVector3D(0,0,1));
            action = QVector3D(-1,0,0);
            camera->setTarget(bb8->getPosition()+QVector3D(0,35,0));
            dynamic_cast<VRSphericalCamera*>(camera)->setPosition(camera->getPosition() + delta * action);
        }
        if(typeCam == "avion"){
            avion->rotate(7,avion->getOrientation().rotatedVector(QVector3D(1,0,0)));
        }
        break;
    case Qt::Key_S:
        if(typeCam=="bb8"){
            dynamic_cast<VRBB8*>(bb8)->move(delta, action, QVector3D(1,0,0), QVector3D(0,0,1));
            action = QVector3D(1,0,0);
            camera->setTarget(bb8->getPosition()+QVector3D(0,35,0));
            dynamic_cast<VRSphericalCamera*>(camera)->setPosition(camera->getPosition() + delta * action);
        }
        if(typeCam=="avion"){
            avion->rotate(-7,avion->getOrientation().rotatedVector(QVector3D(1,0,0)));
        }
        break;
    case Qt::Key_Q:
        if(typeCam == "bb8"){
            dynamic_cast<VRBB8*>(bb8)->move(delta, action, QVector3D(0,0,1), QVector3D(-1,0,0));
            action = QVector3D(0,0,1);
            camera->setTarget(bb8->getPosition()+QVector3D(0,35,0));
            dynamic_cast<VRSphericalCamera*>(camera)->setPosition(camera->getPosition() + delta * action);
        }
        if(typeCam=="avion"){
            avion->rotate(4,avion->getOrientation().rotatedVector(QVector3D(0,1,0)));
            avion->rotate(7,avion->getOrientation().rotatedVector(QVector3D(0,0,1)));
        }
        break;
    case Qt::Key_D:
        if(typeCam=="bb8"){
            dynamic_cast<VRBB8*>(bb8)->move(delta, action, QVector3D(0,0,-1), QVector3D(-1,0,0));
            action = QVector3D(0,0,-1);
            camera->setTarget(bb8->getPosition()+QVector3D(0,35,0));
            dynamic_cast<VRSphericalCamera*>(camera)->setPosition(camera->getPosition() + delta * action);
        }
        if(typeCam=="avion"){
            avion->rotate(-4,avion->getOrientation().rotatedVector(QVector3D(0,1,0)));
            avion->rotate(-7,avion->getOrientation().rotatedVector(QVector3D(0,0,1)));
        }
        break;
    }
    //avion->setPosition(camPos);
    QOpenGLWidget::update();
}


void VRWidget::wheelEvent(QWheelEvent *event)
{
    VRSphericalCamera* cam = (VRSphericalCamera*) camera;
    float rho = cam->getRho() + event->angleDelta().y()*0.1f;
    cam->setRho(rho);
    QOpenGLWidget::update();
}
