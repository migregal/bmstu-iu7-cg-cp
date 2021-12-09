#pragma once

#include <math/Matrix.h>
#include <math/Vector.h>

namespace CGCP {
    class Camera {
    public:
        Camera();

        virtual ~Camera() = default;

        void setCamera(const math::Matrix4x4 &pos);

        math::Matrix4x4 &getCamera();

        void rotate(const math::Vector3 &axis, double phi);

        void translate(const math::Vector3 &offset);

    private:
        math::Matrix4x4 camera_;
    };
}// namespace CGCP
