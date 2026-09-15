/*
** EPITECH PROJECT, 2026
** SpotLight
** File description:
** SpotLight def
*/

#ifndef SPOTLIGHT
    #define SPOTLIGHT

    #include "ALight.hpp"

namespace RayTracer {
/** @class SpotLight
 * @brief A spotlight source
 * This class implements the ALight interface and represents a spotlight source that emits light in a specific direction with a defined angle of influence. The getLightDir method calculates the direction of the light from a given point in the scene, while the getLightingValue method calculates the intensity of the light based on the angle between the light's direction and the vector from the light to the point, as well as the defined angle of influence. The getColor method returns a constant white color for this light.
 * Example usage:
 * SpotLight flashlight(Math::Point3D(0, 10, 0), Math::Vector3D(0, -1, 0), 30.0); // Create a spotlight at position (0, 10, 0) pointing downward with a 30 degree angle of influence
 * This light can then be added to the scene and will contribute light to any objects it illuminates within its angle of influence.
 * @see ALight
 */
class SpotLight : public RayTracer::ALight {
    private:
        Math::Vector3D _directional;
        double _angle;

    public:
        /** @brief Construct a spotlight with default parameters
         * This constructor initializes a SpotLight object with a default origin at the point (0, 0, 0), a default direction of (0, -1, 0) representing downward light, and a default angle of 0 degrees. It is useful for creating a spotlight without specifying any initial parameters.
         */
        SpotLight() : ALight(), _directional(), _angle(0) {}
        /** @brief Construct a spotlight with specified parameters
         * @param origin The position of the light in 3D space
         * @param directional The direction of the light, represented as a Vector3D
         * @param angle The angle of influence for the spotlight in degrees
         * This constructor initializes a SpotLight object with the provided parameters. It allows for creating a spotlight with specific properties by specifying its position, direction, and angle of influence.
         */
        SpotLight(Math::Point3D origin, Math::Vector3D directional, double angle) : ALight(origin), _directional(directional), _angle(angle) {}
        ~SpotLight() override = default;

        /** @brief Get the direction of the light from a given point
         * @param p The point in the scene from which to calculate the light direction
         * @return A vector representing the direction of the light from the point p
         */
        Math::Vector3D getLightDir(Math::Point3D p) const override { return Math::Vector3D(_origin.getX() - p.getX(), _origin.getY() - p.getY(), _origin.getZ() - p.getZ()); }

        /** @brief Get the lighting value of the light at a given point
         * @param p The point in the scene for which to calculate the lighting value
         * @return A double value representing the lighting contribution of this light at the point p, based on the angle between the light's direction and the vector from the light to the point, as well as the defined angle of influence
         */
        double getLightingValue([[maybe_unused]]Math::Point3D p) const override {
            double value = _directional.getNormal().dot(Math::Vector3D(p.getX() - _origin.getX(), p.getY() - _origin.getY(), p.getZ() - _origin.getZ()).getNormal());
            double limit = std::cos((_angle / 2.0) * (M_PI / 180.0));
            if (value * 2 <= limit)
                return 0;
            double intensity = (value - limit) / (1.0 - limit);
            if (value <= limit)
                return 1 - std::pow(intensity, 2.0);
            return 1;
        }

        /** @brief Get the color of the light
         * @return A vector representing the color of the light (constant white color in this implementation)
         */
        Math::Vector3D getColor() const override { return Math::Vector3D(255, 255, 255); }
};
}

#endif