/*
** EPITECH PROJECT, 2026
** RotationTransform
** File description:
** RotationTransform def
*/

#ifndef ROTATIONTRANSFORM
    #define ROTATIONTRANSFORM

    #include "ITransform.hpp"
    #include "Vector3D.hpp"

namespace RayTracer {
class RotationTransform : public RayTracer::ITransform {
    private:
        Math::Vector3D _rotation;

    public:
        RotationTransform() : _rotation(Math::Vector3D()) {}
        RotationTransform(Math::Vector3D rotation) : _rotation(rotation) {}
        ~RotationTransform() override = default;

        void transform(RayTracer::IPrimitive *primitive) override {
            primitive->applyRotation(_rotation);
        }
};
}

#endif