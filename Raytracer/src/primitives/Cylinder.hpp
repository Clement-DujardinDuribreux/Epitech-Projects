/*
** EPITECH PROJECT, 2026
** Cylinder
** File description:
** Cylinder def
*/

#ifndef CYLINDER
    #define CYLINDER

    #include "APrimitive.hpp"

    #include <cmath>

namespace RayTracer {
/** @class Cylinder 
 * @brief A cylinder primitive class
 * This class represents a cylinder primitive in the 3D scene and is responsible for calculating ray intersections and normals for the cylinder shape.
 * The cylinder is defined by its center point, radius, height, and axis vector. The axis vector determines the orientation of the cylinder in 3D space.
 * The hits method calculates the intersection of a ray with the cylinder, while the getNormal method computes the normal vector at the point of intersection.
 */
class Cylinder : public RayTracer::APrimitive {
    private:
        double _radius;
        double _height;
        Math::Vector3D _axis;

        /** @brief Get the axis vector of the cylinder
         * @return The axis vector of the cylinder
         */
        Math::Vector3D getAxis() const {
            if (_axis.length() == 0)
                return Math::Vector3D(0, 0, 1);
            return _axis.getNormal();
        }

        /** @brief Get the center point of the base of the cylinder
         * @param axis The axis vector of the cylinder
         * @return The center point of the base of the cylinder
         */
        bool isInsidedisk(Math::Point3D point, Math::Point3D diskCenter, Math::Vector3D axis) const {
            Math::Vector3D fromdisk(
                point.getX() - diskCenter.getX(),
                point.getY() - diskCenter.getY(),
                point.getZ() - diskCenter.getZ()
            );
            Math::Vector3D radial = fromdisk - (axis * fromdisk.dot(axis));
            return radial.dot(radial) <= _radius * _radius;
        }

        /** @brief Check if a ray hits the disk of the cylinder
         * @param ray The ray to check
         * @param diskCenter The center point of the disk of the cylinder
         * @param normal The normal vector of the disk
         * @param closeTo A reference to the closest intersection distance
         */
        void hitdisk(RayTracer::Ray &ray, Math::Point3D diskCenter, Math::Vector3D normal, double &closeTo) const {
            double normalDotDirection = normal.dot(ray.getDirection());

            if (std::abs(normalDotDirection) < 0.000001)
                return;

            Math::Vector3D originTodisk(
                diskCenter.getX() - ray.getOrigin().getX(),
                diskCenter.getY() - ray.getOrigin().getY(),
                diskCenter.getZ() - ray.getOrigin().getZ()
            );
            double distance = originTodisk.dot(normal) / normalDotDirection;

            if (distance <= 0 || (closeTo > 0 && distance >= closeTo))
                return;

            Math::Point3D point = ray.getOrigin() + (ray.getDirection() * distance);
            if (isInsidedisk(point, diskCenter, getAxis()))
                closeTo = distance;
        }

    public:
        Cylinder() : RayTracer::APrimitive(Math::Point3D()), _radius(0), _height(0), _axis(Math::Vector3D(0, 0, 1)) {}
        Cylinder(Math::Point3D center, double radius, double height, Math::Vector3D axis) : RayTracer::APrimitive(center), _radius(radius), _height(height), _axis(axis) {}
        ~Cylinder() override = default;

        /** @brief Calculate the intersection of a ray with the cylinder
         * @param ray The ray to test for intersection
         * @return The point of intersection if the ray hits the cylinder, or an empty point if there is no intersection
         */
        Math::Point3D hits(RayTracer::Ray &ray) const override {
            Math::Vector3D axis = getAxis();
            Math::Vector3D oc(
                ray.getOrigin().getX() - _center.getX(),
                ray.getOrigin().getY() - _center.getY(),
                ray.getOrigin().getZ() - _center.getZ()
            );
            Math::Vector3D direction = ray.getDirection();
            Math::Vector3D rayDirectionSide = direction - (axis * direction.dot(axis));
            Math::Vector3D originSide = oc - (axis * oc.dot(axis));
            double a = rayDirectionSide.dot(rayDirectionSide);
            double b = 2.0 * rayDirectionSide.dot(originSide);
            double c = originSide.dot(originSide) - (_radius * _radius);
            double closeTo = 0;

            if (a != 0) {
                double delta = (b * b) - (4.0 * a * c);

                if (delta >= 0) {
                    double k1 = (-b - sqrt(delta)) / (2.0 * a);
                    double k2 = (-b + sqrt(delta)) / (2.0 * a);
                    double halfHeight = _height / 2.0;
                    double k[2] = {k1, k2};

                    for (double distance : k) {
                        double projection = oc.dot(axis) + distance * direction.dot(axis);

                        if (distance > 0 && projection >= -halfHeight && projection <= halfHeight && (closeTo == 0 || distance < closeTo))
                            closeTo = distance;
                    }
                }
            }

            Math::Vector3D halfAxis = axis * (_height / 2.0);
            
            Math::Point3D bottomdisk(
                _center.getX() - halfAxis.getX(),
                _center.getY() - halfAxis.getY(),
                _center.getZ() - halfAxis.getZ()
            );

            Math::Point3D topdisk(
                _center.getX() + halfAxis.getX(),
                _center.getY() + halfAxis.getY(),
                _center.getZ() + halfAxis.getZ()
            );

            hitdisk(ray, bottomdisk, axis * -1.0, closeTo);
            hitdisk(ray, topdisk, axis, closeTo);

            if (closeTo == 0)
                return Math::Point3D();
            return ray.getOrigin() + (ray.getDirection() * closeTo);
        }

        /** @brief Get the normal vector at the point of intersection
         * @param hit The point of intersection
         * @param ray The ray that hit the cylinder (unused in this implementation)
         * @return The normal vector at the point of intersection
         */
        Math::Vector3D getNormal(Math::Point3D hit, [[maybe_unused]] RayTracer::Ray ray) const override {
            Math::Vector3D axis = getAxis();
            Math::Vector3D fromCenter(
                hit.getX() - _center.getX(),
                hit.getY() - _center.getY(),
                hit.getZ() - _center.getZ()
            );
            double projection = fromCenter.dot(axis);
            double halfHeight = _height / 2.0;

            if (std::abs(projection - halfHeight) < 0.000001)
                return axis;
            if (std::abs(projection + halfHeight) < 0.000001)
                return axis * -1.0;

            Math::Vector3D radial = fromCenter - (axis * projection);
            if (radial.length() == 0)
                return axis;
            return radial.getNormal();
        }

        void applyScaling(Math::Vector3D scaling) override {
            _radius *= (scaling.getX() + scaling.getY()) / 2.0;
            _height *= scaling.getZ();
        }

        void applyRotation(Math::Vector3D rotation) override {
            _axis = _axis.rotateEuler(rotation);
        }

        void applyTranslation(Math::Vector3D translation) override {
            _center = Math::Point3D(_center.getX() + translation.getX(), _center.getY() + translation.getY(), _center.getZ() + translation.getZ());
        }
};
}

#endif
