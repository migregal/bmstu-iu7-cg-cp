#include <light/Light.h>

namespace CGCP::light {
    LightSource::LightSource(math::Vector3 position, double intensity, LightType type)
        : position_(position.normalized()), intensity_(intensity), type_(type) {}

    LightType LightSource::getType() {
        return type_;
    }

    void LightSource::setPosition(const math::Vector3 &position) {
        position_ = position;
    }


    math::Vector3 LightSource::getPosition() {
        return position_;
    }

    void LightSource::setIntensivity(double intensivity) {
        intensity_ = intensivity;
    }

    double LightSource::getIntensity() {
        return intensity_;
    }
}// namespace CGCP::light
