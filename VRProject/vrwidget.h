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
#include <QList>
#include <QVector3D>

#include "vrbody.h"
#include "vrcamera.h"
#include "vrcube.h"
#include "vrplane.h"
#include "vrsphere.h"
#include "vrplanet.h"
#include "vrtorus.h"
#include "vrtexcube.h"
#include "vrdice.h"
#include "vrscene.h"
#include "vrsphericalcamera.h"
#include "vrskybox.h"
#include "vrlight.h"
#include "vrcurve.h"
#include "vrhelix.h"
#include "vrmodel.h"
#include "vrairplane.h"
#include "vrterrain.h"

class VRWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT

public:
    VRWidget(QWidget *parent = nullptr);
    ~VRWidget();

private:
    // --- Variables pour les Bonus et le HUD ---
    int respawnTimer = 0;               // Pour l'affichage du message "RESPAWN"
    float HUD_opacity = 0.0f;           // Pour l'effet de fondu du HUD
    QList<QVector3D> trailPoints;       // Liste des positions pour la traînée (Bonus)

    // --- Objets de la scène ---
    VRBody* body;
    VRBody* plane;
    VRBody* world;
    VRBody* torus;

    VRSkyBox* skybox;
    VRScene scene;
    VRCamera* camera;
    VRLight* light;

    // --- Avions contrôlés ---
    VRAirplane* avion;
    VRAirplane* avion2;
    VRAirplane* avion3;

    // --- Logique et Animation ---
    QTime startTime;
    QTime lastTime;
    VRCurve* trajectory;
    QTimer* timer;

    bool animationOn = false;
    bool stereoEnabled = false;
    float eyeSeparation = 0.4f;
    QString position_camera = "Fixe";   // Correction de casse pour correspondre aux tests

    float angleY;
    float angleX;
    QPointF oldPos;

protected slots:
    void update();

public slots:
    void startAnimation();
    void changeFov(int);
    void changeOpacity(int);
    void changeSize(int value);
    void changeSaturation(int value);
    void changeCulling();
    void changeWireframe();
    void changeHeightFactor(int newFactor);
    void changePositionCamera(QString newPos);
    void changeStereo(bool enabled);

signals:
    void displayVelocity(double);
    void displayAltitude(double);
    void displayAcceleration(double);

protected:
    // --- Interface OpenGL ---
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

    // --- Interface Événements ---
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void keyPressEvent(QKeyEvent* event) override;
    void wheelEvent(QWheelEvent *event) override;
};

#endif // VRWIDGET_H
