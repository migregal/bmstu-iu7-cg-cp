#pragma once

#include <math/Vector.h>

namespace CGCP::light {
    enum LightType {
        Ambient = 0x00,
        Point = 0x01,
        Directional = 0x02
    };

    class LightSource {
    public:
        LightSource(math::Vector3 position, double intensity, LightType type);

        LightType getType();

        math::Vector3 getPosition();

        double getIntensity();

    private:
        math::Vector3 position_;
        double intensity_;
        LightType type_;
    };
}// namespace CGCP::light
