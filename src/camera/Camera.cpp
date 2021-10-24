#include <camera/Camera.h>

#include <math/Vector.h>

namespace CGCP {
    Camera::Camera() {
        QMatrix4x4 cameraTransformation;

        QVector3D cameraPosition = cameraTransformation.mapVector(QVector3D(0, 0, 3));
        QVector3D cameraUpDirection = cameraTransformation.mapVector(QVector3D(0, 1, 0));

        camera_.lookAt(cameraPosition, QVector3D(0, 0, 0), cameraUpDirection);
    }

    math::Matrix4x4 &Camera::getCamera() {
        return camera_;
    }
}// namespace CGCP