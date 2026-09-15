/*
** EPITECH PROJECT, 2026
** Cone
** File description:
** Cone def
*/

#ifndef CONE
    #define CONE

    #include "APrimitive.hpp"

    #include <cmath>

namespace RayTracer {
/** @brief A cone primitive class
 * This class represents a cone primitive in the 3D scene and is responsible for calculating ray intersections and normals for the cone shape.
 * The cone is defined by its center point, radius, height, and axis vector. The axis vector determines the orientation of the cone in 3D space.
 * The hits method calculates the intersection of a ray with the cone, while the getNormal method computes the normal vector at the point of intersection.
 */
class Cone : public RayTracer::APrimitive {
    private:
        double _radius;
        double _height;
        Math::Vector3D _axis;

        /** @brief Get the axis vector of the cone
         * @return The axis vector of the cone
         */
        Math::Vector3D getAxis() const {
            if (_axis.length() == 0)
                return Math::Vector3D(0, 0, 1);
            return _axis.getNormal();
        }
        
        /** @brief Get the apex point of the cone based on the axis vector
         * @param axis The axis vector of the cone
         * @return The apex point of the cone
         */
        Math::Point3D getApex(Math::Vector3D axis) const {
            Math::Vector3D halfAxis = axis * (_height / 2.0);

            return Math::Point3D(
                _center.getX() + halfAxis.getX(),
                _center.getY() + halfAxis.getY(),
                _center.getZ() + halfAxis.getZ()
            );
        }

        /** @brief Get the center point of the base of the cone
         * @param axis The axis vector of the cone
         * @return The center point of the base of the cone
         */
        Math::Point3D getBaseCenter(Math::Vector3D axis) const {
            Math::Vector3D halfAxis = axis * (_height / 2.0);

            return Math::Point3D(
                _center.getX() - halfAxis.getX(),
                _center.getY() - halfAxis.getY(),
                _center.getZ() - halfAxis.getZ()
            );
        }

        /** @brief Check if a point is inside the base of the cone
         * @param point The point to check
         * @param baseCenter The center point of the base of the cone
         * @param axis The axis vector of the cone
         * @return True if the point is inside the base, false otherwise
         */
        bool isInsideBase(Math::Point3D point, Math::Point3D baseCenter, Math::Vector3D axis) const {
            Math::Vector3D fromBase(
                point.getX() - baseCenter.getX(),
                point.getY() - baseCenter.getY(),
                point.getZ() - baseCenter.getZ()
            );
            Math::Vector3D radial = fromBase - (axis * fromBase.dot(axis));

            return radial.dot(radial) <= _radius * _radius;
        }

        /** @brief Check if a ray hits the base of the cone
         * @param ray The ray to check
         * @param baseCenter The center point of the base of the cone
         * @param axis The axis vector of the cone
         * @param closeTo A reference to the closest intersection distance
         */
        void hitBase(RayTracer::Ray &ray, Math::Point3D baseCenter, Math::Vector3D axis, double &closeTo) const {
            Math::Vector3D normal = axis * -1.0;
            double normalDotDirection = normal.dot(ray.getDirection());

            if (std::abs(normalDotDirection) < 0.000001)
                return;

            Math::Vector3D originToBase(
                baseCenter.getX() - ray.getOrigin().getX(),
                baseCenter.getY() - ray.getOrigin().getY(),
                baseCenter.getZ() - ray.getOrigin().getZ()
            );
            double distance = originToBase.dot(normal) / normalDotDirection;

            if (distance <= 0 || (closeTo > 0 && distance >= closeTo))
                return;

            Math::Point3D point = ray.getOrigin() + (ray.getDirection() * distance);
            if (isInsideBase(point, baseCenter, axis))
                closeTo = distance;
        }

    public:
        Cone() : RayTracer::APrimitive(Math::Point3D()), _radius(0), _height(0), _axis(Math::Vector3D(0, 0, 1)) {}
        Cone(Math::Point3D center, double radius, double height, Math::Vector3D axis) : RayTracer::APrimitive(center), _radius(radius), _height(height), _axis(axis) {}
        ~Cone() override = default;

        /** @brief Calculate the intersection of a ray with the cone
         * @param ray The ray to test for intersection
         * @return The point of intersection if the ray hits the cone, or an empty point if there is no intersection
         */
        Math::Point3D hits(RayTracer::Ray &ray) const override {
            if (_radius <= 0 || _height <= 0)
                return Math::Point3D();

            Math::Vector3D axis = getAxis();
            Math::Point3D apex = getApex(axis);
            Math::Vector3D originToApex(
                ray.getOrigin().getX() - apex.getX(),
                ray.getOrigin().getY() - apex.getY(),
                ray.getOrigin().getZ() - apex.getZ()
            );
            Math::Vector3D direction = ray.getDirection();
            double incline = _radius / _height;
            double inclineSquared = incline * incline;
            double directionProjection = direction.dot(axis);
            double originProjection = originToApex.dot(axis);
            double a = direction.dot(direction) - ((1.0 + inclineSquared) * directionProjection * directionProjection);
            double b = 2.0 * (originToApex.dot(direction) - ((1.0 + inclineSquared) * originProjection * directionProjection));
            double c = originToApex.dot(originToApex) - ((1.0 + inclineSquared) * originProjection * originProjection);
            double closeTo = 0;

            if (std::abs(a) > 0.000001) {
                double delta = (b * b) - (4.0 * a * c);

                if (delta >= 0) {
                    double k[2] = {
                        (-b - std::sqrt(delta)) / (2.0 * a),
                        (-b + std::sqrt(delta)) / (2.0 * a)
                    };

                    for (double distance : k) {
                        double projection = originProjection + distance * directionProjection;

                        if (distance > 0 && projection <= 0 && projection >= -_height && (closeTo == 0 || distance < closeTo))
                            closeTo = distance;
                    }
                }
            }

            hitBase(ray, getBaseCenter(axis), axis, closeTo);

            if (closeTo == 0)
                return Math::Point3D();
            return ray.getOrigin() + (ray.getDirection() * closeTo);
        }

        /** @brief Get the normal vector at the point of intersection
         * @param hit The point of intersection
         * @param ray The ray that hit the cone (unused in this implementation)
         * @return The normal vector at the point of intersection
         */
        Math::Vector3D getNormal(Math::Point3D hit, [[maybe_unused]] RayTracer::Ray ray) const override {
            Math::Vector3D axis = getAxis();
            Math::Point3D baseCenter = getBaseCenter(axis);
            Math::Vector3D fromBase(
                hit.getX() - baseCenter.getX(),
                hit.getY() - baseCenter.getY(),
                hit.getZ() - baseCenter.getZ()
            );
            double baseProjection = fromBase.dot(axis);

            if (std::abs(baseProjection) < 0.000001)
                return axis * -1.0;

            Math::Point3D apex = getApex(axis);
            Math::Vector3D fromApex(
                hit.getX() - apex.getX(),
                hit.getY() - apex.getY(),
                hit.getZ() - apex.getZ()
            );
            double apexProjection = fromApex.dot(axis);
            double incline = _radius / _height;
            double inclineSquared = incline * incline;
            Math::Vector3D radial = fromApex - (axis * apexProjection);
            Math::Vector3D normal = radial - (axis * (inclineSquared * apexProjection));

            if (normal.length() == 0)
                return axis;
            return normal.getNormal();
        }

        void applyScaling(Math::Vector3D scaling) override {
            _radius *= scaling.getX();
            _height *= scaling.getY();
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
