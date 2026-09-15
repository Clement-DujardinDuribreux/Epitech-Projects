/*
** EPITECH PROJECT, 2026
** TranslationTransform
** File description:
** TranslationTransform def
*/

#ifndef TRANSLATIONTRANSFORM
    #define TRANSLATIONTRANSFORM

    #include "ITransform.hpp"
    #include "Vector3D.hpp"

namespace RayTracer {
class TranslationTransform : public RayTracer::ITransform {
    private:
        Math::Vector3D _translation;

    public:
        TranslationTransform() : _translation(Math::Vector3D()) {}
        TranslationTransform(Math::Vector3D translation) : _translation(translation) {}
        ~TranslationTransform() override = default;

        void transform(RayTracer::IPrimitive *primitive) override {
            primitive->applyTranslation(_translation);
        }
};
}

#endif