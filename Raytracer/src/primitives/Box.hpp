/*
** EPITECH PROJECT, 2026
** Box
** File description:
** Box def
*/

#ifndef BOX
    #define BOX

    #include <iostream>
    #include <string>
    #include "APrimitive.hpp"
    #include <algorithm>

namespace RayTracer {
class Box : public RayTracer::APrimitive {
    private:
        Math::Point3D _min;
        Math::Point3D _max;
        Math::Vector3D _normal;

        double _length;
        double _width;
        double _height;

    public:
        Box() : RayTracer::APrimitive(Math::Point3D()), _min(Math::Point3D()), _max(Math::Point3D()), _normal(Math::Vector3D()), _length(0), _width(0), _height(0) {}
        /** @brief Create a box primitive
         * @param center The center point of the box
         * @param length The length of the box
         * @param width The width of the box
         * @param height The height of the box
         * @param normal The normal vector of the box
         */
        Box(Math::Point3D center, double length, double width, double height, Math::Vector3D normal) : RayTracer::APrimitive(center), _normal(normal), _length(length), _width(width), _height(height) {
            _min = Math::Point3D(center.getX() - length / 2, center.getY() - width / 2, center.getZ() - height/2);
            _max = Math::Point3D(center.getX() + length / 2, center.getY() + width / 2, center.getZ() + height/2);
        }
        ~Box() override = default;
        /** @brief Calculate the intersection of a ray with the box
         * @param ray The ray to test for intersection
         * @return The point of intersection if the ray hits the box, or an empty point if there is no intersection
         */
        Math::Point3D hits(RayTracer::Ray &ray) const override {
            Math::Vector3D W = _normal.getNormal();
            Math::Vector3D arbitrary = (std::abs(W.getX()) < 0.9) ? Math::Vector3D(1, 0, 0) : Math::Vector3D(0, 1, 0);
            Math::Vector3D U = W.cross(arbitrary).getNormal();
            Math::Vector3D V = W.cross(U).getNormal();
            Math::Point3D origin = ray.getOrigin();
            Math::Vector3D dir = ray.getDirection();
            Math::Vector3D delta(origin.getX() - _center.getX(), origin.getY() - _center.getY(), origin.getZ() - _center.getZ());
            Math::Vector3D local_origin(delta.dot(U), delta.dot(V), delta.dot(W));
            Math::Vector3D local_dir(dir.dot(U), dir.dot(V), dir.dot(W));
            double hx = _max.getX() - _center.getX();
            double hy = _max.getY() - _center.getY();
            double hz = _max.getZ() - _center.getZ();
            double tmin = -1e10, tmax = 1e10;
            if (std::abs(local_dir.getX()) > 1e-6) {
                double t1 = (-hx - local_origin.getX()) / local_dir.getX();
                double t2 = (hx - local_origin.getX()) / local_dir.getX();
                if (t1 > t2) 
                    std::swap(t1, t2);
                tmin = std::max(tmin, t1);
                tmax = std::min(tmax, t2);
            } else if (local_origin.getX() < -hx || local_origin.getX() > hx) {
                return Math::Point3D();
            }
            if (std::abs(local_dir.getY()) > 1e-6) {
                double t1 = (-hy - local_origin.getY()) / local_dir.getY();
                double t2 = (hy - local_origin.getY()) / local_dir.getY();
                if (t1 > t2) 
                    std::swap(t1, t2);
                tmin = std::max(tmin, t1);
                tmax = std::min(tmax, t2);
            } else if (local_origin.getY() < -hy || local_origin.getY() > hy) {
                return Math::Point3D();
            }
            if (std::abs(local_dir.getZ()) > 1e-6) {
                double t1 = (-hz - local_origin.getZ()) / local_dir.getZ();
                double t2 = (hz - local_origin.getZ()) / local_dir.getZ();
                if (t1 > t2) 
                    std::swap(t1, t2);
                tmin = std::max(tmin, t1);
                tmax = std::min(tmax, t2);
            } else if (local_origin.getZ() < -hz || local_origin.getZ() > hz) {
                return Math::Point3D();
            }
            if (tmin > tmax || tmax < 0) {
                return Math::Point3D();
            }
            double t = (tmin > 0.0001) ? tmin : tmax;
            if (t < 0.0001) 
                return Math::Point3D();
            return origin + dir * t;
        }
        
        /** @brief Get the normal vector at the point of intersection
         * @param hit The point of intersection
         * @param ray The ray that hit the box (unused in this implementation)
         * @return The normal vector at the point of intersection
         */
        Math::Vector3D getNormal(Math::Point3D hit, [[maybe_unused]] RayTracer::Ray ray) const override {
            Math::Vector3D W = _normal.getNormal();
            Math::Vector3D arbitrary = (std::abs(W.getX()) < 0.9) ? Math::Vector3D(1, 0, 0) : Math::Vector3D(0, 1, 0);
            Math::Vector3D U = W.cross(arbitrary).getNormal();
            Math::Vector3D V = W.cross(U).getNormal();
            Math::Vector3D delta(hit.getX() - _center.getX(), hit.getY() - _center.getY(), hit.getZ() - _center.getZ());
            Math::Vector3D local_hit(delta.dot(U), delta.dot(V), delta.dot(W));
            double hx = _max.getX() - _center.getX();
            double hy = _max.getY() - _center.getY();
            double hz = _max.getZ() - _center.getZ();
            double epsilon = 0.001;
            if (std::abs(local_hit.getX() - hx) < epsilon) 
                return U;
            if (std::abs(local_hit.getX() + hx) < epsilon) 
                return U * (-1);
            if (std::abs(local_hit.getY() - hy) < epsilon) 
                return V;
            if (std::abs(local_hit.getY() + hy) < epsilon) 
                return V * (-1);
            if (std::abs(local_hit.getZ() - hz) < epsilon) 
                return W;
            if (std::abs(local_hit.getZ() + hz) < epsilon) 
                return W * (-1);
            return Math::Vector3D(0, 1, 0);
        }

        void applyTranslation(Math::Vector3D translation) override {
            _center = Math::Point3D(_center.getX() + translation.getX(), _center.getY() + translation.getY(), _center.getZ() + translation.getZ());
            _min = Math::Point3D(_min.getX() + translation.getX(), _min.getY() + translation.getY(), _min.getZ() + translation.getZ());
            _max = Math::Point3D(_max.getX() + translation.getX(), _max.getY() + translation.getY(), _max.getZ() + translation.getZ());
        }

        void applyScaling(Math::Vector3D scaling) override {
            _length *= scaling.getX();
            _width *= scaling.getY();
            _height *= scaling.getZ();
            _min = Math::Point3D(_center.getX() - _length / 2, _center.getY() - _width / 2, _center.getZ() - _height / 2);
            _max = Math::Point3D(_center.getX() + _length / 2, _center.getY() + _width / 2, _center.getZ() + _height / 2);
        }

        void applyRotation(Math::Vector3D rotation) override {
            _normal = _normal.rotateEuler(rotation);
        }
};
}
#endif