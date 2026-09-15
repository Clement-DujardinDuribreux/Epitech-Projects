/*
** EPITECH PROJECT, 2026
** DirectionalLight
** File description:
** DirectionalLight def
*/

#ifndef DIRECTIONALLIGHT
    #define DIRECTIONALLIGHT

    #include "ALight.hpp"

namespace RayTracer {
/** @class DirectionalLight
 * @brief A directional light with radius
 * This class implements the ALight interface and represents a directional light source that has a defined radius of influence. The direction of the light is determined by a Vector3D, while the radius defines how far the light's influence extends from its origin. The getLightDir method calculates the direction of the light from a given point in the scene, while the getLightingValue method calculates the intensity of the light based on the distance from the point to the light's direction and radius. The getColor method returns a constant white color for this light.
 * Example usage:
 * DirectionalLight sun(Math::Point3D(0, 10, 0), Math::Vector3D(0, -1, 0), 5.0); // Create a directional light representing sunlight
 * This light can then be added to the scene and will contribute directional light to any objects it illuminates within its radius of influence.
 * @see ALight
 */
class DirectionalLight : public RayTracer::ALight {
    private:
        Math::Vector3D _directional;
        double _radius;

    public:
        /** @brief Construct a directional light with default parameters
         * This constructor initializes a DirectionalLight object with a default origin at the point (0, 0, 0), a default direction of (0, -1, 0) representing downward light, and a default radius of 0. It is useful for creating a directional light without specifying any initial parameters.
         */
        DirectionalLight() : ALight(), _directional(), _radius(0) {}
        /** @brief Construct a directional light with specified parameters
         * @param origin The position of the light in 3D space
         * @param directional The direction of the light, represented as a Vector3D
         * @param radius The radius of influence for the light
         * This constructor initializes a DirectionalLight object with the provided parameters. It allows for creating a directional light with specific properties by specifying its position, direction, and radius of influence.
         */
        DirectionalLight(Math::Point3D origin, Math::Vector3D directional, double radius) : ALight(origin), _directional(directional), _radius(radius * 2) {}
        ~DirectionalLight() override = default;

        /** @brief Get the direction of the light from a given point
         * @param p The point in the scene from which to calculate the light direction
         * @return A vector representing the direction of the light from the point p
         */
        Math::Vector3D getLightDir(Math::Point3D p) const override { 
            Math::Vector3D v(p.getX() - _origin.getX(), p.getY() - _origin.getY(), p.getZ() - _origin.getZ());
            Math::Vector3D dir = _directional.getNormal();
            double t = v.dot(dir);
            return Math::Vector3D(-dir.getX() * t, -dir.getY() * t, -dir.getZ() * t);
        }

        /** @brief Get the lighting value of the light at a given point
         * @param p The point in the scene for which to calculate the lighting value
         * @return A double value representing the lighting contribution of this light at the point p, based on distance and radius of influence
         */
        double getLightingValue([[maybe_unused]]Math::Point3D p) const override {
            Math::Vector3D v(p.getX() - _origin.getX(), p.getY() - _origin.getY(), p.getZ() - _origin.getZ());
            Math::Vector3D dir = _directional.getNormal();
            double t = v.dot(dir);
            if (t < 0)
                return 0;
            double vLen = v.length();
            double distSq = (vLen * vLen) - (t * t);
            if (distSq < 0) distSq = 0; 
            double dist = std::sqrt(distSq);
            if (dist > _radius * 2)
                return 0;
            double intensity = 1.0 - (dist / _radius);
            if (dist > _radius)
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