#include "vrwidget.h"

VRWidget::VRWidget(QWidget *parent)
    : QOpenGLWidget(parent), QOpenGLFunctions(), scene()
{
    timer = new QTimer();
    angleY = 5;
    position_camera = "Avion";
    grabKeyboard(); // Indispensable pour capter les touches directionnelles
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

        // --- BONUS : GESTION DE LA TRAINÉE ---
        trailPoints.append(avion->getPosition());
        if (trailPoints.size() > 500) {
            trailPoints.removeFirst();
        }

        // --- LOGIQUE DE BORDURE (RESPAWN) ---
        QVector3D pos = avion->getPosition();
        float limite = 480.0f;

        if (qAbs(pos.x()) > limite || qAbs(pos.y()) > limite || qAbs(pos.z()) > limite)
        {
            avion->setPosition(QVector3D(0, 1.5, -1));
            avion->setVelocity(1.0f);
            avion->setAcceleration(1.0f);
            avion->setOrientation(0, 0, 0);

            trailPoints.clear();
            qDebug() << "Sortie de zone ! Respawn à l'origine.";
        }

        // --- LOGIQUE DE SUIVI CAMÉRA ÉLASTIQUE ---
        QVector3D pos_avion = avion->getPosition();
        QQuaternion or_avion = avion->getOrientation();

        if(position_camera == "Avion"){
            QVector3D targetPos = pos_avion - 18 * or_avion.rotatedVector(QVector3D(0,0,-1)) + 6 * or_avion.rotatedVector(QVector3D(0,1,0));
            QVector3D currentCamPos = camera->getPosition();
            QVector3D smoothPos = currentCamPos * 0.85f + targetPos * 0.15f;

            camera->setPosition(smoothPos);
            camera->setTarget(pos_avion);
        }
        else if(position_camera == "Sol"){
            QVector3D targetPos = body->getPosition() + QVector3D(0,12,0);
            QVector3D currentCamPos = camera->getPosition();
            QVector3D smoothPos = currentCamPos * 0.9f + targetPos * 0.1f;

            camera->setPosition(smoothPos);
            camera->setTarget(pos_avion);
        }
        else if(position_camera == "Fixe"){
            camera->setPosition(QVector3D(0,0,0));
            camera->setTarget(QVector3D(0, 0, -1));
        }
        else if(position_camera == "Subjective"){
            QVector3D forward = or_avion.rotatedVector(QVector3D(0, 0, -1));
            QVector3D cockpitOffset = or_avion.rotatedVector(QVector3D(0.0f, 0.4f, 0.0f));
            QVector3D camPos = pos_avion + cockpitOffset + forward * 5.0f;
            camera->setPosition(camPos);
            camera->setTarget(camPos + forward * 5.0f);
        }
    }

    emit displayVelocity(avion->getVelocity());
    emit displayAltitude(avion->getPosition().y());
    emit displayAcceleration(avion->getAcceleration());

    QOpenGLWidget::update();
}

void VRWidget::initializeGL()
{
    initializeOpenGLFunctions();
    glClearColor(0.0f, 0.566f, 0.867f, 1.0f);

    glEnable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE); // Désactivé pour voir les deux faces du terrain

    camera = new VRSphericalCamera();
    camera->setFov(80.0f);
    camera->setZMin(0.1f);
    camera->setZMax(2000.0f);

    light = new VRLight();

    plane = new VRTerrain(256);
    plane->setPosition(QVector3D(0, -30, 0));
    plane->setCamera(camera);
    dynamic_cast<VRTerrain*>(plane)->setHeightFactor(15);
    dynamic_cast<VRTerrain*>(plane)->setHeightmap(":/textures/heightmap.png");
    plane->setTexture(":/textures/terrain.jpg");
    plane->setLight(light);
    plane->initialize();

    body = new VRTexCube();
    body->setCamera(camera);
    body->setScale(20);
    body->setPosition(QVector3D(0, -30, 0));
    body->setTexture(":/textures/brique.png", false, false);
    body->setLight(light);
    body->initialize();

    avion = new VRAirplane();
    avion->setCamera(camera);
    avion->setLight(light);
    avion->initialize();
    avion->translate(QVector3D(0, 1.5, -1));

    world = new VRSphere(5.0);
    world->setCamera(camera);
    world->setPosition(QVector3D(0, 10, -50));
    world->setTexture(":/textures/2k_earth_daymap.jpg", true, true);
    world->setLight(light);
    world->initialize();

    skybox = new VRSkyBox();
    skybox->setCubeTexture(":/textures/skybox/left.jpg", ":/textures/skybox/right.jpg",
                           ":/textures/skybox/front.jpg", ":/textures/skybox/back.jpg",
                           ":/textures/skybox/top.jpg", ":/textures/skybox/bottom.jpg");
    skybox->setCamera(camera);
    skybox->setScale(1000);
    skybox->initialize();

    scene.append(plane);
    scene.append(body);
    scene.append(avion);
    scene.append(world);
    scene.append(skybox);
    scene.setCamera(camera);

    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    timer->start(16);
}

void VRWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    if (!stereoEnabled) {
        glViewport(0, 0, width(), height());
        scene.draw();
        return;
    }

    QVector3D originalPos = camera->getPosition();
    QVector3D originalTarget = camera->getTarget();
    float originalAspect = camera->getAspect();

    QVector3D forward = (originalTarget - originalPos).normalized();
    QVector3D right = QVector3D::crossProduct(forward, camera->getUp()).normalized();
    float halfSeparation = eyeSeparation * 0.5f;

    int halfWidth = width() / 2;
    camera->setAspect(static_cast<float>(halfWidth) / height());

    camera->setPosition(originalPos - right * halfSeparation);
    camera->setTarget(originalTarget - right * halfSeparation);
    glViewport(0, 0, halfWidth, height());
    scene.draw();

    camera->setPosition(originalPos + right * halfSeparation);
    camera->setTarget(originalTarget + right * halfSeparation);
    glViewport(halfWidth, 0, halfWidth, height());
    scene.draw();

    camera->setPosition(originalPos);
    camera->setTarget(originalTarget);
    camera->setAspect(originalAspect);

}

void VRWidget::keyPressEvent(QKeyEvent *event)
{
    // --- CORRECTION : UTILISATION DES AXES LOCAUX ---
    // On récupère l'orientation actuelle pour définir les axes propres à l'avion
    QQuaternion orientation = avion->getOrientation();
    QVector3D localRight = orientation.rotatedVector(QVector3D(1, 0, 0));
    QVector3D localUp    = orientation.rotatedVector(QVector3D(0, 1, 0));
    QVector3D localRoll  = orientation.rotatedVector(QVector3D(0, 0, 1));

    switch (event->key())
    {
    case Qt::Key_Up:
        avion->rotate(-3, localRight); // Piquer sur l'axe latéral local
        break;
    case Qt::Key_Down:
        avion->rotate(3, localRight);  // Cabrer
        break;
    case Qt::Key_Left:
        avion->rotate(3, localUp);      // Tourner sur l'axe vertical local
        avion->rotate(10, localRoll);   // Roll Bonus sur l'axe longitudinal local
        break;
    case Qt::Key_Right:
        avion->rotate(-3, localUp);
        avion->rotate(-10, localRoll);
        break;
    case Qt::Key_W: avion->setAcceleration(avion->getAcceleration() + 0.5f); break;
    case Qt::Key_X: avion->setAcceleration(avion->getAcceleration() - 0.5f); break;
    }
    QOpenGLWidget::update();
}

void VRWidget::startAnimation()
{
    animationOn = !animationOn;
    if (animationOn) {
        lastTime = QTime::currentTime();
        startTime = QTime::currentTime();
    }
}

void VRWidget::changePositionCamera(QString newPos) { position_camera = newPos; }
void VRWidget::resizeGL(int w, int h)
{
    camera->setAspect(static_cast<float>(w) / h);
    glViewport(0, 0, w, h);
}

void VRWidget::changeFov(int fov) { camera->setFov(fov); }
void VRWidget::changeHeightFactor(int factor) { if(plane) dynamic_cast<VRTerrain*>(plane)->setHeightFactor(factor); }
void VRWidget::changeStereo(bool enabled) { stereoEnabled = enabled; }
void VRWidget::mousePressEvent(QMouseEvent *event) { oldPos = event->position(); }
void VRWidget::mouseMoveEvent(QMouseEvent *event) {
    float dx = (event->position().x() - oldPos.x()) / width();
    float dy = (event->position().y() - oldPos.y()) / height();
    if (event->buttons() & Qt::LeftButton) {
        VRSphericalCamera* cam = dynamic_cast<VRSphericalCamera*>(camera);
        if (cam) { cam->setPhi(cam->getPhi() + dy); cam->setTheta(cam->getTheta() + dx); }
    }
    oldPos = event->position();
    QOpenGLWidget::update();
}
void VRWidget::wheelEvent(QWheelEvent *event) {
    VRSphericalCamera* cam = dynamic_cast<VRSphericalCamera*>(camera);
    if (cam) cam->setRho(cam->getRho() - event->angleDelta().y() * 0.05f);
    QOpenGLWidget::update();
}

void VRWidget::changeOpacity(int) {}
void VRWidget::changeSize(int) {}
void VRWidget::changeSaturation(int) {}
void VRWidget::changeCulling() {}
void VRWidget::changeWireframe() {}
