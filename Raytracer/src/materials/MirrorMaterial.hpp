/*
** EPITECH PROJECT, 2026
** MirrorMaterial
** File description:
** MirrorMaterial def
*/

#ifndef MIRRORMATERIAL
    #define MIRRORMATERIAL

    #include "AMaterial.hpp"
    #include <cmath>

namespace RayTracer {
/** @class MirrorMaterial
 * @brief A material that reflects light like a mirror
 * This class implements the AMaterial interface and represents a material that has mirror-like reflective properties. It calculates the reflected color based on the incoming ray and the normal at the point of intersection, allowing for realistic reflections in the rendered scene.
 * The getRendering method computes the reflected color by tracing a new ray in the direction of reflection and combining it with the base color of the material. The getShadowColor and getShadowIntensity methods provide information for shadow calculations, allowing the material to interact correctly with shadows in the scene.
 * Example usage:
 * MirrorMaterial mirror(0.8); // Create a mirror material with 80% reflectivity
 * This material can then be assigned to a primitive to give it reflective properties when rendered.
 * @see AMaterial
 */
class MirrorMaterial : public RayTracer::AMaterial {
    private:
        double _reflectivity;

    public:
        /** @brief Construct a mirror material with default reflectivity
         * This constructor initializes a MirrorMaterial object with a default reflectivity of 1.0, which represents a perfect mirror. It is useful for creating a mirror material without specifying an initial reflectivity.
         */
        MirrorMaterial() : _reflectivity(1.0) {}
        /** @brief Construct a mirror material with specified reflectivity
         * @param reflectivity The reflectivity value for the mirror material (between 0 and 1)
         * This constructor initializes a MirrorMaterial object with the provided reflectivity. It allows for creating a mirror material with varying degrees of reflectivity by specifying a value between 0 (no reflection) and 1 (perfect reflection).
         */
        MirrorMaterial(double reflectivity) : _reflectivity(reflectivity) {}
        ~MirrorMaterial() override = default;

        /** @brief Get the rendering color of the mirror material
         * @param currentObj The current primitive being rendered
         * @param objs The list of all primitives in the scene
         * @param lights The list of all lights in the scene
         * @param r The ray being traced
         * @param hit The point of intersection on the primitive
         * @param context The rendering context containing additional information for rendering
         * @return A vector representing the color of the material for rendering purposes, including reflections
         */
        Math::Vector3D getRendering(
            RayTracer::IPrimitive *currentObj,
            std::vector<RayTracer::IPrimitive *> objs,
            std::vector<RayTracer::ILight *> lights,
            RayTracer::Ray r,
            Math::Point3D hit,
            RayTracer::RenderingContext context
        ) const override {
            Math::Vector3D baseColor = BASE_COLOR;
            if (getWrapper())
                baseColor = getWrapper()->getRendering(currentObj, objs, lights, r, hit, context);
            Math::Vector3D normal = currentObj->getNormal(hit, r);
            Math::Vector3D i = r.getDirection().getNormal();
            Math::Vector3D reflected = i - (normal * (2.0 * i.dot(normal)));
            reflected = reflected.getNormal();

            RayTracer::Ray reflectedRay(hit + (normal * 0.001), reflected);

            Math::Vector3D reflectedColor = BASE_COLOR;
            for (RayTracer::IPrimitive *obj : objs) {
                if (obj == currentObj)
                    continue;
                Math::Point3D reflectedHit = obj->hits(reflectedRay);
                if (reflectedHit != Math::Point3D()) {
                    reflectedColor = context.renderer->Render(context, obj, objs, lights, reflectedRay, reflectedHit, obj->getMaterial()->getRendering(obj, objs, lights, reflectedRay, reflectedHit, context), obj->getMaterial()->getShininess());
                    break;
                }
            }

            return baseColor * (1.0 - _reflectivity) + reflectedColor * _reflectivity;
        }

        /** @brief Get the shadow color of the mirror material
         * @return A vector representing the shadow color of the material
         */
        Math::Vector3D getShadowColor() const override {
            if (getWrapper())
                return getWrapper()->getShadowColor() * Math::Vector3D(1, 1, 1);
            return Math::Vector3D(1, 1, 1);
        }

        /** @brief Get the shadow intensity of the mirror material
         * @param ray The ray being traced for shadow calculation
         * @param hit The point of intersection on the primitive
         * @param currentObj The current primitive being rendered
         * @param objs The list of all primitives in the scene
         * @return A double value representing the intensity of the shadow (1 for no shadow, 0 for full shadow)
         */
        double getShadowIntensity([[maybe_unused]] RayTracer::Ray ray, [[maybe_unused]] Math::Point3D hit, 
            [[maybe_unused]] RayTracer::IPrimitive *currentObj,
            [[maybe_unused]] std::vector<RayTracer::IPrimitive *> objs) const override {
            if (getWrapper())
                return getWrapper()->getShadowIntensity(ray, hit, currentObj, objs);
            return 1;
        }
};
}

#endif
