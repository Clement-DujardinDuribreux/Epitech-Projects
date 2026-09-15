/*
** EPITECH PROJECT, 2026
** ScaleTransform
** File description:
** ScaleTransform def
*/

#ifndef SCALETRANSFORM
    #define SCALETRANSFORM

    #include "ITransform.hpp"
    #include "Vector3D.hpp"

namespace RayTracer {
class ScaleTransform : public RayTracer::ITransform {
    private:
        Math::Vector3D _scale;

    public:
        ScaleTransform() : _scale(Math::Vector3D(1, 1, 1)) {}
        ScaleTransform(Math::Vector3D scale) : _scale(scale) {}
        ~ScaleTransform() override = default;

        void transform(RayTracer::IPrimitive *primitive) override {
            primitive->applyScaling(_scale);
        }
};
}

#endif