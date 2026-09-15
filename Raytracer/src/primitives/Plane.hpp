/*
** EPITECH PROJECT, 2026
** Plane
** File description:
** Plane def
*/

#ifndef PLANE
    #define PLANE

    #include "APrimitive.hpp"

namespace RayTracer {
/** @class Plane 
 * @brief A plane primitive class
 * This class represents a plane primitive in the 3D scene and is responsible for calculating ray
 * intersections and normals for the plane shape. The plane is defined by its center point, length, width, and normal vector. The hits method calculates the intersection of a ray with the plane, while the getNormal method returns the normal vector at the point of intersection.
 * The hits method first checks if the ray is parallel to the plane by calculating the dot product of the plane's normal vector and the ray's direction. If they are parallel, it returns an empty point. Otherwise, it calculates the intersection point using the formula for ray-plane intersection. It then checks if the intersection point is within the bounds of the plane defined by its length and width. If it is outside the bounds, it returns an empty point; otherwise, it returns the intersection point.
 * The getNormal method simply returns the normalized normal vector of the plane, as the normal is constant across the entire surface of the plane.
 * @see APrimitive 
 */
class Plane : public RayTracer::APrimitive {
    private:
        double _length;
        double _width;
        Math::Vector3D _normal;

    public:
        Plane() : RayTracer::APrimitive(Math::Point3D()), _length(0), _width(0), _normal(Math::Vector3D()) {}
        /** @brief Create a plane primitive
         * @param center The center point of the plane
         * @param length The length of the plane
         * @param width The width of the plane
         * @param normal The normal vector of the plane
         */
        Plane(Math::Point3D center, double length, double width, Math::Vector3D normal) : RayTracer::APrimitive(center), _length(length), _width(width), _normal(normal) {}
        ~Plane() override = default;

        /** @brief Calculate the intersection of a ray with the plane
         * @param ray The ray to test for intersection
         * @return The point of intersection if the ray hits the plane, or an empty point if there is no intersection
         */
        Math::Point3D hits(RayTracer::Ray &ray) const override {
            if (_normal.dot(ray.getDirection()) == 0)
                return Math::Point3D();
            double t = (Math::Vector3D(_center.getX() - ray.getOrigin().getX(), _center.getY() - ray.getOrigin().getY(), _center.getZ() - ray.getOrigin().getZ())).dot(_normal) / _normal.dot(ray.getDirection());
            if (t < 0)
                return Math::Point3D();
            Math::Point3D p = ray.getOrigin() + (ray.getDirection() * t);
            if (p.getX() < _center.getX() - _length / 2 || p.getX() > _center.getX() + _length / 2 ||
                p.getY() < _center.getY() - _width / 2 || p.getY() > _center.getY() + _width / 2 ||
                p.getZ() < _center.getZ() - _length / 2 || p.getZ() > _center.getZ() + _length / 2)
                return Math::Point3D();
            return p;
        }

        /** @brief Get the normal vector at a point on the plane
         * @param hit The point on the plane for which to calculate the normal
         * @param ray The ray that intersected with the plane
         * @return The normal vector at the intersection point, or an empty vector if no normal can be calculated
         * This method returns the normalized normal vector of the plane, as the normal is constant across the entire surface of the plane.
         */
        Math::Vector3D getNormal([[maybe_unused]] Math::Point3D hit, [[maybe_unused]] RayTracer::Ray ray) const override {
            return _normal.getNormal();
        }

        void applyTranslation(Math::Vector3D translation) override {
            _center = _center + translation;
        }

        void applyScaling(Math::Vector3D scaling) override {
            _length *= scaling.getX();
            _width *= scaling.getY();
        }

        void applyRotation(Math::Vector3D rotation) override {
             _normal = _normal.rotateEuler(rotation);
        }
};
}

#endif