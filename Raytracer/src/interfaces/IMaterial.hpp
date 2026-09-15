/*
** EPITECH PROJECT, 2026
** IMaterial
** File description:
** IMaterial def
*/

#ifndef IMATERIAL
    #define IMATERIAL

    #include "Vector3D.hpp"
    #include "Point3D.hpp"
    #include "Ray.hpp"
    #include "IRendering.hpp"
    #include "IPrimitive.hpp"
    #include "ILight.hpp"

    #include <vector>
    #include <memory>

#define BASE_COLOR Math::Vector3D(255, 255, 255)

namespace RayTracer {
/** @class IMaterial
 * @brief An interface for materials
 * This interface defines the basic functionality that all materials in the ray tracer must implement. It includes methods for getting a wrapper material, setting a wrapper material, calculating the rendering color based on the current object, primitives, lights, ray, hit point, and rendering context, getting the shadow color of the material, and calculating the shadow intensity based on a ray, hit point, current object, and list of primitives. Any class that implements this interface can be used as a material for primitives in the ray tracing engine, allowing for flexibility in defining different types of materials with various properties and behaviors.
 * @see ALight
 */
class IMaterial {
    private:

    public:
        virtual ~IMaterial() = default;

        /** @brief Get the wrapper material (non-owning)
         * @return A raw pointer to the wrapper material, or nullptr if there is no wrapper
         */
        virtual RayTracer::IMaterial *getWrapper() const = 0;
        /** @brief Set the wrapper material (takes ownership)
         * @param wrapper A unique_ptr to the material to set as the wrapper
         */
        virtual void setWrapper(std::unique_ptr<RayTracer::IMaterial> wrapper) = 0;

        /** @brief Calculate the rendering color based on the current object, primitives, lights, ray, hit point, and rendering context
         * @param currentObj The primitive currently being rendered
         * @param objs The list of all primitives in the scene
         * @param lights The list of all light sources in the scene
         * @param r The ray being traced
         * @param hit The point of intersection between the ray and the primitive
         * @param context The rendering context containing information about the scene and rendering settings
         * @return A vector representing the color to render for the current object at the hit point, taking into account lighting and material properties
         */
        virtual Math::Vector3D getRendering(RayTracer::IPrimitive *currentObj, std::vector<RayTracer::IPrimitive *> objs, std::vector<RayTracer::ILight *> lights, RayTracer::Ray r, Math::Point3D hit, RayTracer::RenderingContext context) const = 0;
        /** @brief Get the shadow color of the material
         * @return A vector representing the color of the shadow cast by this material
         */
        virtual Math::Vector3D getShadowColor() const = 0;
        /** @brief Calculate the shadow intensity based on a ray, hit point, current object, and list of primitives
         * @param ray The ray being traced
         * @param hit The point of intersection between the ray and the primitive
         * @param currentObj The primitive currently being rendered
         * @param objs The list of all primitives in the scene
         * @return A double value representing the intensity of the shadow at the hit point
         */
        virtual double getShadowIntensity(RayTracer::Ray ray, Math::Point3D hit, RayTracer::IPrimitive *currentObj, std::vector<RayTracer::IPrimitive *> objs) const = 0;
        virtual double getShininess() const = 0;
};
}

#endif