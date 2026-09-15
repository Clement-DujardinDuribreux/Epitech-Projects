/*
** EPITECH PROJECT, 2026
** ALight
** File description:
** ALight def
*/

#ifndef ALIGHT
    #define ALIGHT

    #include "Point3D.hpp"
    #include "ILight.hpp"

namespace RayTracer {
/** @class ALight
 * @brief An abstract class for light sources
 * This class serves as a base for different types of light sources in the ray tracer. It implements the ILight interface and provides a common implementation for storing the origin of the light. Derived classes can extend this base class to implement specific types of lights (e.g., point lights, directional lights, spotlights) while reusing the common functionality provided by ALight.
 * @see ILight
 */
class ALight : public RayTracer::ILight {
    protected:
        Math::Point3D _origin;

    public:
        /** @brief Construct a light at the origin
         * This constructor initializes an ALight object with its origin at the default point (0, 0, 0) in 3D space. It is useful for creating a light source without specifying an initial position, allowing it to be positioned later as needed.
         */
        ALight() : _origin() {}
        /** @brief Construct a light with specified origin
         * @param origin The position of the light in 3D space
         * This constructor initializes an ALight object with the provided origin. It allows for creating a light source at a specific location in 3D space by specifying its origin during construction.
         */
        ALight(Math::Point3D origin) : _origin(origin) {}
        ~ALight() override = default;

        /** @brief Get the origin of the light
         * @return A point representing the position of the light in 3D space
         */
        Math::Point3D getOrigin() const override { return _origin; }
};
}

#endif