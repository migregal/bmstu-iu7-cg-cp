#pragma once

#include <math/Matrix.h>

namespace CGCP {
    class Camera {
    public:
        Camera();

        virtual ~Camera() = default;

        math::Matrix4x4 &getCamera();

    private:
        math::Matrix4x4 camera_;
    };
}// namespace CGCP