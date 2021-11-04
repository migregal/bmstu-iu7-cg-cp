#include <light/Light.h>

namespace CGCP::light {
    LightSource::LightSource(math::Vector3 position, double intensity, LightType type)
        : position_(position), intensity_(intensity), type_(type) {}

    LightType LightSource::getType() {
        return type_;
    }

    math::Vector3 LightSource::getPosition() {
        return position_;
    }

    double LightSource::getIntensity() {
        return intensity_;
    }
}// namespace CGCP::light
