/*
** EPITECH PROJECT, 2026
** DirectionalColorLight
** File description:
** DirectionalColorLight def
*/

#ifndef DIRECTIONALCOLORLIGHT
    #define DIRECTIONALCOLORLIGHT

    #include "ALight.hpp"


namespace RayTracer {
/** @class DirectionalColorLight
 * @brief A directional light with color and radius
 * This class implements the ALight interface and represents a directional light source that emits light with a specific color and has a defined radius of influence. The direction of the light is determined by a Vector3D, while the radius defines how far the light's influence extends from its origin. The getLightDir method calculates the direction of the light from a given point in the scene, while the getLightingValue method calculates the intensity of the light based on the distance from the point to the light's direction and radius. The getColor method returns the color of the light for use in shading calculations.
 * Example usage:
 * DirectionalColorLight sun(Math::Point3D(0, 10, 0), Math::Vector3D(0, -1, 0), 5.0, Math::Vector3D(1, 1, 0.8)); // Create a directional color light representing sunlight
 * This light can then be added to the scene and will contribute directional light with a warm color to any objects it illuminates within its radius of influence.
 * @see ALight
 */
class DirectionalColorLight : public RayTracer::ALight {
    private:
        Math::Vector3D _directional;
        double _radius;
        Math::Vector3D _color;

    public:
        /** @brief Construct a directional color light with default parameters
         * This constructor initializes a DirectionalColorLight object with a default origin at the point (0, 0, 0), a default direction of (0, -1, 0) representing downward light, a default radius of 0, and a default color of (0, 0, 0) representing black (no light). It is useful for creating a directional color light without specifying any initial parameters.
         */
        DirectionalColorLight() : ALight(), _directional(), _radius(0), _color(Math::Vector3D()) {}
        /** @brief Construct a directional color light with specified parameters
         * @param origin The position of the light in 3D space
         * @param directional The direction of the light, represented as a Vector3D
         * @param radius The radius of influence for the light
         * @param color The color of the light, represented as a Vector3D (RGB)
         * This constructor initializes a DirectionalColorLight object with the provided parameters. It allows for creating a directional color light with specific properties by specifying its position, direction, radius of influence, and the color of the light it emits.
         */
        DirectionalColorLight(Math::Point3D origin, Math::Vector3D directional, double radius, Math::Vector3D color) : ALight(origin), _directional(directional), _radius(radius * 2), _color(color) {}
        ~DirectionalColorLight() override = default;

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
         * @return A vector representing the color of the light (RGB)
         */
        Math::Vector3D getColor() const override { return _color; }
};
}

#endif