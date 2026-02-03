#include "vrstereocamera.h"

VRStereoCamera::VRStereoCamera() : VRCamera()
{
   eyeDistance = 4;
   focalDistance = 60;
   cameraType = VR_CAMERA_MONO;
}

float VRStereoCamera::getEyeDistance() const
{
    return eyeDistance;
}

void VRStereoCamera::setEyeDistance(float newEyeDistance)
{
    eyeDistance = newEyeDistance;
}

float VRStereoCamera::getFocalDistance() const
{
    return focalDistance;
}

void VRStereoCamera::setFocalDistance(float newFocalDistance)
{
    focalDistance = newFocalDistance;
}

QMatrix4x4 VRStereoCamera::viewMatrix()
{
    if(cameraType==VR_CAMERA_LEFT){
        return VRStereoCamera::leftViewMatrix();
    }
    if(cameraType==VR_CAMERA_RIGHT){
        return VRStereoCamera::rightViewMatrix();
    }
    else{
        return VRCamera::viewMatrix();
    }
}

QMatrix4x4 VRStereoCamera::projectionMatrix()
{
    if(cameraType==VR_CAMERA_LEFT){
        return VRStereoCamera::leftProjectionMatrix();
    }
    if(cameraType==VR_CAMERA_RIGHT){
        return VRStereoCamera::rightProjectionMatrix();
    }
    else{
        return VRCamera::projectionMatrix();
    }
}

QMatrix4x4 VRStereoCamera::leftViewMatrix()
{
    QVector3D v= (QVector3D::crossProduct(target-position,up)).normalized();
    QVector3D posOG=position-(eyeDistance/2)*v;
    QVector3D cibleOG=target-(eyeDistance/2)*v;
    QMatrix4x4 view;
    view.lookAt(posOG,cibleOG,up);
    return view;
}

QMatrix4x4 VRStereoCamera::rightViewMatrix()
{
    QVector3D v= (QVector3D::crossProduct(target-position,up)).normalized();
    QVector3D posOG=position+(eyeDistance/2)*v;
    QVector3D cibleOG=target+(eyeDistance/2)*v;
    QMatrix4x4 view;
    view.lookAt(posOG,cibleOG,up);
    return view;
}

QMatrix4x4 VRStereoCamera::leftProjectionMatrix()
{
    QMatrix4x4 proj;
    float top= zMin*qTan( qDegreesToRadians(fov)/float(2));
    float bottom=-top;
    float a= focalDistance*(top/zMin)*aspect;
    float b=a-eyeDistance/float(2);
    float c=a+eyeDistance/float(2);
    float left=-b*zMin/focalDistance;
    float right=c*zMin/focalDistance;
    proj.frustum(left,right,bottom,top,zMin,zMax);
    return proj;
}
QMatrix4x4 VRStereoCamera::rightProjectionMatrix()
{
    QMatrix4x4 proj;
    float top=zMin*qTan( qDegreesToRadians(fov)/float(2));
    float bot=-top;
    float a= focalDistance*(top/zMin)*aspect;
    float b=a+eyeDistance/float(2);
    float c=a-eyeDistance/float(2);
    float left=-b*zMin/focalDistance;
    float right=c*zMin/focalDistance;
    proj.frustum(left,right,bot,top,zMin,zMax);
    return proj;
}

int VRStereoCamera::getCameraType() const
{
    return cameraType;
}

void VRStereoCamera::setCameraType(int newCameraType)
{
    cameraType = newCameraType;
}
