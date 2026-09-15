/*
** EPITECH PROJECT, 2026
** ILight
** File description:
** ILight def
*/

#ifndef ILIGHT
    #define ILIGHT

    #include "Point3D.hpp"
    #include "Vector3D.hpp"

namespace RayTracer {
/** @class ILight
 * @brief An interface for light sources
 * This interface defines the basic functionality that all light sources in the ray tracer must implement. It includes methods for getting the origin of the light, calculating the lighting value at a given point, determining the direction of the light from a point, and retrieving the color of the light. Any class that implements this interface can be used as a light source in the ray tracing engine, allowing for flexibility in defining different types of lights such as point lights, spotlights, directional lights, etc.
 * @see ALight
 */
class ILight {
    private:

    public:
        /** @brief Destruct the light
         */
        virtual ~ILight() = default;

        /** @brief Get the origin of the light
         * @return A point representing the position of the light in 3D space
         */
        virtual Math::Point3D getOrigin() const = 0;

        /** @brief Get the lighting value of the light at a given point
         * @param p The point in the scene for which to calculate the lighting value
         * @return A double value representing the lighting contribution of this light at the point p
         */
        virtual double getLightingValue(Math::Point3D p) const = 0;
        /** @brief Get the direction of the light from a given point
         * @param p The point in the scene from which to calculate the light direction
         * @return A vector representing the direction of the light from the point p
         */
        virtual Math::Vector3D getLightDir(Math::Point3D p) const = 0;
        /** @brief Get the color of the light
         * @return A vector representing the color of the light (RGB)
         */
        virtual Math::Vector3D getColor() const = 0;
};
}

#endif