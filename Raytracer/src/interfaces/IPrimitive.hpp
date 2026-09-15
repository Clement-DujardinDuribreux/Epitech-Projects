/*
** EPITECH PROJECT, 2026
** IPrimitive
** File description:
** IPrimitive def
*/

#ifndef IPRIMITIVE
    #define IPRIMITIVE

    #include "Vector3D.hpp"
    #include "Point3D.hpp"
    #include "IMaterial.hpp"
    #include "Ray.hpp"
    #include <memory>

namespace RayTracer {

class IMaterial;

/** @class IPrimitive
 * @brief An interface for primitives
 * This interface defines the basic functionality that all primitives in the ray tracer must implement. It includes methods for getting and setting the center point of the primitive, getting and setting the material of the primitive, calculating the intersection of a ray with the primitive, and getting the normal vector at a hit point based on a ray. Any class that implements this interface can be used as a primitive in the ray tracing engine, allowing for flexibility in defining different types of geometric objects such as spheres, planes, boxes, etc.
 * @see ALight
 */
class IPrimitive {
    private:

    public:
        virtual ~IPrimitive() = default;

        /** @brief Get the center point of the primitive
         * @return A point representing the center of the primitive in 3D space
         */
        virtual Math::Point3D getCenter() const = 0;
        /** @brief Get the material of the primitive (non-owning)
         * @return A raw pointer to the material associated with the primitive
         */
        virtual RayTracer::IMaterial *getMaterial() const = 0;

        /** @brief Set the material of the primitive (takes ownership)
         * @param material A unique_ptr to the material to associate with the primitive
         */
        virtual void setMaterial(std::unique_ptr<RayTracer::IMaterial> material) = 0;
        /** @brief Set the center point of the primitive
         * @param center A point representing the new center of the primitive in 3D space
         */
        virtual void setCenter(Math::Point3D center) = 0;

        /** @brief Calculate the intersection of a ray with the primitive
         * @param ray The ray to intersect with the primitive
         * @return A point representing the intersection point, or an invalid point if no intersection occurs
         */
        virtual Math::Point3D hits(RayTracer::Ray &ray) const = 0;
        /** @brief Get the normal vector at a hit point based on a ray
         * @param hit The point of intersection between the ray and the primitive
         * @param ray The ray that intersected with the primitive
         * @return A vector representing the normal at the hit point
         */
        virtual Math::Vector3D getNormal(Math::Point3D hit, RayTracer::Ray ray) const = 0;

        virtual void applyTranslation(Math::Vector3D translation) = 0;
        virtual void applyScaling(Math::Vector3D scaling) = 0;
        virtual void applyRotation(Math::Vector3D rotation) = 0;
};
}

#endif