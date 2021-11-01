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

    void Camera::rotate(const math::Vector3 &axis, double phi) {
        camera_.rotate(phi, axis.x(), axis.y(), axis.z());
    }

    void Camera::translate(const math::Vector3 &offset) {
        camera_.translate(offset);
    }
}// namespace CGCP
