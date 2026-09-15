/*
** EPITECH PROJECT, 2026
** Rectangle3D
** File description:
** Rectangle3D def
*/

#ifndef RECTANGLE3D
    #define RECTANGLE3D

    #include "Point3D.hpp"
    #include "Vector3D.hpp"

namespace RayTracer {
class Rectangle3D {
    private:
        Math::Point3D _origin;
        Math::Vector3D _bottom;
        Math::Vector3D _left;

    public:
        Rectangle3D() : _origin(), _bottom(), _left() {}
        Rectangle3D(Math::Point3D origin, Math::Vector3D bottom, Math::Vector3D left) : _origin(origin), _bottom(bottom), _left(left) {}
        ~Rectangle3D() = default;

        /** @brief Get a point on the rectangle based on the u and v coordinates
         * @param u The u coordinate (0 to 1) representing the horizontal position on the rectangle
         * @param v The v coordinate (0 to 1) representing the vertical position on the rectangle
         * @return The point on the rectangle corresponding to the given u and v coordinates
         */
        Math::Point3D pointAt(double u, double v) {
            Math::Vector3D r = (_bottom * u) + (_left * v);
            return Math::Point3D(_origin.getX() + r.getX(), _origin.getY() + r.getY(), _origin.getZ() + r.getZ());
        }
};
}

#endif