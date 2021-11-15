#pragma once

#include <memory>
#include <vector>

#include <bounders/Bounder.h>

#include <material/Material.h>
#include <math/Vector.h>

namespace CGCP::fractal {

    class Fractal {
    public:
        Fractal() = default;

        virtual ~Fractal() = default;

        virtual float raycast(
                math::Vector3 const &ro,
                math::Vector3 const &rd,
                math::Vector4 &rescol,
                float fov,
                math::Vector3 const &c) = 0;

        virtual math::Vector3 calcNormal(math::Vector3 &pos,
                                         float t,
                                         float fovfactor,
                                         const math::Vector3 &c) = 0;

        void setBounder(std::shared_ptr<bounder::Bounder> bounder);

        void setMaterial(const material::Material &material);

        math::Vector3 &getColor();

        double getRoughness();

        double getSpecular();

    protected:
        std::shared_ptr<bounder::Bounder> bounder_;

    private:
        material::Material material_;
    };
}// namespace CGCP::fractal
