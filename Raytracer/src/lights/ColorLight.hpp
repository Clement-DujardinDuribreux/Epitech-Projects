/*
** EPITECH PROJECT, 2026
** ColorLight
** File description:
** ColorLight def
*/

#ifndef COLORLIGHT
    #define COLORLIGHT

    #include "ALight.hpp"

    #include <algorithm>

namespace RayTracer {
/** @class ColorLight
 * @brief A point light with color
 * This class implements the ALight interface and represents a point light source that emits light with a specific color. The color of the light is defined by a Vector3D, which represents the RGB components of the light. The getLightDir method calculates the direction of the light from a given point in the scene, while the getLightingValue method returns a constant value indicating that this light contributes fully to the lighting calculations. The getColor method returns the color of the light for use in shading calculations.
 * Example usage:
 * ColorLight redLight(Math::Point3D(0, 10, 0), Math::Vector3D(1, 0, 0)); // Create a red point light at position (0, 10, 0)
 * This light can then be added to the scene and will contribute red light to any objects it illuminates.
 * @see ALight
 */
class ColorLight : public RayTracer::ALight {
    private:
        Math::Vector3D _color;

    public:
        /** @brief Construct a color light with default origin and color
         * This constructor initializes a ColorLight object with a default origin at the point (0, 0, 0) and a default color of (0, 0, 0), which represents black (no light). It is useful for creating a color light without specifying any initial parameters.
         */
        ColorLight() : ALight(), _color(Math::Vector3D()) {}
        /** @brief Construct a color light with specified origin and color
         * @param origin The position of the light in 3D space
         * @param color The color of the light, represented as a Vector3D (RGB)
         * This constructor initializes a ColorLight object with the provided origin and color. It allows for creating a point light with specific properties by specifying its position and the color of the light it emits.
         */
        ColorLight(Math::Point3D origin, Math::Vector3D color) : ALight(origin), _color(color) {}
        ~ColorLight() override = default;

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
         * @return A vector representing the color of the light (RGB)
         */
        Math::Vector3D getColor() const override { return _color; }
};
}

#endif