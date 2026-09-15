/*
** EPITECH PROJECT, 2026
** Ray
** File description:
** Ray def
*/

#ifndef RAY
    #define RAY

    #include "Vector3D.hpp"
    #include "Point3D.hpp"

namespace RayTracer {
class Ray {
    private:
        Math::Point3D _origin;
        Math::Vector3D _direction;

    public:
        Ray() : _origin(), _direction() {}
        Ray(Math::Point3D origin, Math::Vector3D direction) : _origin(origin), _direction(direction) {}
        ~Ray() = default;

        /** @brief Get the origin of the ray
         * @return The origin point
         */
        Math::Point3D getOrigin() const { return _origin; }

        /** @brief Get the direction of the ray
         * @return The direction vector
         */
        Math::Vector3D getDirection() const { return _direction; }
};
}

#endif