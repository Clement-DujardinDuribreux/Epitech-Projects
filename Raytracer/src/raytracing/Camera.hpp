/*
** EPITECH PROJECT, 2026
** Camera
** File description:
** Camera def
*/

#ifndef CAMERA
    #define CAMERA

    #include "Point3D.hpp"
    #include "Rectangle3D.hpp"
    #include "Ray.hpp"

namespace RayTracer {
/** @brief A camera class for generating rays
 * This class represents a camera in the 3D scene and is responsible for generating rays based on the camera's position and orientation.
 */
class Camera {
    private:
        Math::Point3D _origin;
        RayTracer::Rectangle3D _screen;

    public:
        Camera() : _origin(), _screen() {}
        Camera(Math::Point3D origin, RayTracer::Rectangle3D screen) : _origin(origin), _screen(screen) {} 
        ~Camera() = default;

        /** @brief Generate a ray from the camera
         * @param u The u coordinate on the screen
         * @param v The v coordinate on the screen
         * @return The generated ray
         */
        RayTracer::Ray ray(double u, double v) {
            Math::Point3D p = _screen.pointAt(u, v);
            return RayTracer::Ray(_origin, Math::Vector3D(p.getX() - _origin.getX(), p.getY() - _origin.getY(), p.getZ() - _origin.getZ()));
        }
};
}

#endif