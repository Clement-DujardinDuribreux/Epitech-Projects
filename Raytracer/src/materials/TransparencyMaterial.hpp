/*
** EPITECH PROJECT, 2026
** TransparencyMaterial
** File description:
** TransparencyMaterial def
*/

#ifndef TRANSPARENCYMATERIAL
    #define TRANSPARENCYMATERIAL

    #include "AMaterial.hpp"

namespace RayTracer {
/** @class TransparencyMaterial 
 * @brief A material that simulates transparency
 * This class implements the AMaterial interface and represents a material that allows light to pass through it, simulating transparency. The transparency level is defined by a double value between 0 (fully opaque) and 1 (fully transparent). The getRendering method calculates the color of the material by blending the base color with the colors of any objects behind it, based on the transparency level. The getShadowColor and getShadowIntensity methods provide information for shadow calculations, allowing the material to interact correctly with shadows in the scene.
 * Example usage:
 * TransparencyMaterial glass(0.5); // Create a transparency material with 50% transparency
 * This material can then be assigned to a primitive to give it transparent properties when rendered.
 * @see AMaterial
*/
class TransparencyMaterial : public RayTracer::AMaterial {
    private:
        Math::Vector3D _color;
        double _transparency;

    public:
        /** @brief Construct a transparency material with default transparency
         * This constructor initializes a TransparencyMaterial object with a default transparency of 0.0, which represents a fully opaque material. It is useful for creating a transparency material without specifying an initial transparency level.
         */
        TransparencyMaterial() : _transparency(0.0) {}
        /** @brief Construct a transparency material with specified transparency
         * @param transparency The transparency level for the material (between 0 and 1)
         * This constructor initializes a TransparencyMaterial object with the provided transparency level. It allows for creating a transparency material with varying degrees of transparency by specifying a value between 0 (fully opaque) and 1 (fully transparent).
         */
        TransparencyMaterial(double transparency) : _transparency(transparency) {}
        ~TransparencyMaterial() override = default;

        /** @brief Get the rendering color of the transparency material
         * @param currentObj The current primitive being rendered
         * @param objs The list of all primitives in the scene
         * @param lights The list of all lights in the scene
         * @param r The ray being traced
         * @param hit The point of intersection on the primitive
         * @param context The rendering context containing additional information for rendering
         * @return A vector representing the color of the material for rendering purposes, including transparency effects
         */
        Math::Vector3D getRendering(
            RayTracer::IPrimitive *currentObj,
            std::vector<RayTracer::IPrimitive *> objs,
            std::vector<RayTracer::ILight *> lights,
            RayTracer::Ray r, Math::Point3D hit, 
            RayTracer::RenderingContext context
        ) const override {
            Math::Vector3D color = BASE_COLOR;
            if (getWrapper())
                color = getWrapper()->getRendering(currentObj, objs, lights, r, hit, context);
            RayTracer::Ray newRay(hit + (r.getDirection().getNormal() * 0.001), r.getDirection());
            for (RayTracer::IPrimitive *obj : objs) {
                if (obj == currentObj)
                    continue;
                Math::Point3D newHit = obj->hits(newRay);
                if (newHit != Math::Point3D()) {;
                    Math::Vector3D colorTransparency = context.renderer->Render(context, obj, objs, lights, newRay, newHit, obj->getMaterial()->getRendering(obj, objs, lights, newRay, newHit, context), obj->getMaterial()->getShininess());
                    return (color * (1.0 - _transparency)) + (colorTransparency * _transparency);
                }
            }
            return (color * (1.0 - _transparency)) + (BASE_COLOR * _transparency);
        }

        /** @brief Get the shadow color of the transparency material
         * @return A vector representing the shadow color of the material, blended based on transparency
         */
        Math::Vector3D getShadowColor() const override {
            double t = _transparency * 2.5;
            if (t > 1.0) t = 1.0;
            return Math::Vector3D(1, 1, 1) * (1.0 - t) + _color.getNormal() * t;
        }

        /** @brief Get the shadow intensity of the transparency material
         * @param ray The ray being traced for shadow calculation
         * @param hit The point of intersection on the primitive
         * @param currentObj The current primitive being rendered
         * @param objs The list of all primitives in the scene
         * @return A double value representing the intensity of the shadow, reduced based on transparency
         */
        double getShadowIntensity(RayTracer::Ray ray, Math::Point3D hit, RayTracer::IPrimitive *currentObj, std::vector<RayTracer::IPrimitive *> objs) const override {
            RayTracer::Ray newRay(hit, ray.getDirection()); 
            for (RayTracer::IPrimitive *obj : objs) {
                if (obj == currentObj)
                    continue;
                Math::Point3D newHit = obj->hits(newRay);
                if (newHit != Math::Point3D())
                    return 1.0 - _transparency + obj->getMaterial()->getShadowIntensity(newRay, newHit, obj, objs) * _transparency;
            }
            if (getWrapper())
                return getWrapper()->getShadowIntensity(ray, hit, currentObj, objs) * (1.0 - _transparency);
            return 1.0 - _transparency;
        }
};
}

#endif