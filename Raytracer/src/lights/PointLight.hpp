/*
** EPITECH PROJECT, 2026
** PointLight
** File description:
** PointLight def
*/

#ifndef POINTLIGHT
    #define POINTLIGHT

    #include "ALight.hpp"

namespace RayTracer {
/** @class PointLight
 * @brief A point light source
 * This class implements the ALight interface and represents a point light source that emits light in all directions from a specific point in space. The getLightDir method calculates the direction of the light from a given point in the scene, while the getLightingValue method returns a constant value indicating that this light contributes fully to the lighting calculations. The getColor method returns a constant white color for this light.
 * Example usage:
 * PointLight lamp(Math::Point3D(0, 10, 0)); // Create a point light at position (0, 10, 0)
 * This light can then be added to the scene and will contribute light to any objects it illuminates.
 * @see ALight
 */
class PointLight : public RayTracer::ALight {
    private:

    public:
        /** @brief Construct a point light with default origin
         * This constructor initializes a PointLight object with a default origin at the point (0, 0, 0). It is useful for creating a point light without specifying any initial parameters.
         */
        PointLight() : ALight() {}
        /** @brief Construct a point light with specified origin
         * @param origin The position of the light in 3D space
         * This constructor initializes a PointLight object with the provided origin. It allows for creating a point light at a specific location in 3D space by specifying its origin.
         */
        PointLight(Math::Point3D origin) : ALight(origin) {}
        ~PointLight() override = default;

        /** @brief Get the direction of the light from a given point
         * @param p The point in the scene from which to calculate the light direction
         * @return A vector representing the direction of the light from the point p
         */
        Math::Vector3D getLightDir(Math::Point3D p) const override { return Math::Vector3D(_origin.getX() - p.getX(), _origin.getY() - p.getY(), _origin.getZ() - p.getZ()); }
        /** @brief Get the lighting value of the light at a given point
         * @param p The point in the scene for which to calculate the lighting value
         * @return A double value representing the lighting contribution of this light at the point p (constant value of 1 in this implementation)
         */
        double getLightingValue([[maybe_unused]]Math::Point3D p) const override { return 1; }
        /** @brief Get the color of the light
         * @return A vector representing the color of the light (constant white color in this implementation)
         */
        Math::Vector3D getColor() const override { return Math::Vector3D(255, 255, 255); }
};
}

#endif