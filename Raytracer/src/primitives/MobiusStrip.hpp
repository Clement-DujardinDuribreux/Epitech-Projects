/*
** EPITECH PROJECT, 2026
** MobiusStrip
** File description:
** Mobius strip primitive
*/

#ifndef MOBIUSSTRIP
    #define MOBIUSSTRIP

    #include "APrimitive.hpp"

    #include <algorithm>
    #include <cmath>

namespace RayTracer {
/** @class MobiusStrip
 * @brief A class representing a mobius strip primitive
 * This class represents a mobius strip primitive in the 3D scene and is responsible for calculating ray intersections and normals for the mobius strip shape.
 * The mobius strip is defined by its center point, radius, width, and axis vector
 * The hits method calculates the intersection of a ray with the mobius strip, while the getNormal method computes the normal vector at the point of intersection.
 * The mobius strip is a non-orientable surface, which means it has only one side and one edge. This unique property is reflected in the way the distance to the surface is calculated,
 * taking into account the twist of the strip. The getDistance method computes the distance from a point to the mobius strip surface, while the getLocalNormal method calculates the normal vector based on the distance gradient.
 * The transformToLocalSpace and transformToWorldSpace methods are used to convert points and normals between the local coordinate space of the mobius strip and the world coordinate space, allowing for accurate intersection and normal calculations regardless of the mobius strip's orientation in the scene.
 * @see APrimitive
 */
class MobiusStrip : public RayTracer::APrimitive {
    private:
        double _radius;
        double _width;
        Math::Vector3D _axis;

        /** @brief Get the axis vector of the mobius strip
         * @return The axis vector of the mobius strip
         */
        Math::Vector3D getAxis() const {
            if (_axis.length() == 0)
                return Math::Vector3D(0, 0, 1);
            return _axis.getNormal();
        }

        /** @brief Get the basis vectors for the local coordinate system of the mobius strip
         * @param u A reference to a Vector3D that will be set to the first basis vector
         * @param v A reference to a Vector3D that will be set to the second basis vector
         * @param w A reference to a Vector3D that will be set to the third basis vector (the axis)
         */
        void getBasis(Math::Vector3D &u, Math::Vector3D &v, Math::Vector3D &w) const {
            w = getAxis();
            Math::Vector3D referenceAxis(1, 0, 0);

            if (std::abs(w.getX()) > 0.9)
                referenceAxis = Math::Vector3D(0, 1, 0);
            u = referenceAxis.cross(w).getNormal();
            v = w.cross(u).getNormal();
        }

        /** @brief Transform a point from world space to the local space of the mobius strip
         * @param point The point in world space to be transformed
         * @return The transformed point in the local space of the mobius strip
         */
        Math::Vector3D transformToLocalSpace(Math::Point3D point) const {
            Math::Vector3D u;
            Math::Vector3D v;
            Math::Vector3D w;
            Math::Vector3D fromCenter(
                point.getX() - _center.getX(),
                point.getY() - _center.getY(),
                point.getZ() - _center.getZ()
            );

            getBasis(u, v, w);
            return Math::Vector3D(fromCenter.dot(u), fromCenter.dot(v), fromCenter.dot(w));
        }

        /** @brief Transform a vector from the local space of the mobius strip to world space
         * @param local The vector in local space to be transformed
         * @return The transformed vector in world space
         */
        Math::Vector3D transformToWorldSpace(Math::Vector3D local) const {
            Math::Vector3D u;
            Math::Vector3D v;
            Math::Vector3D w;

            getBasis(u, v, w);
            return (u * local.getX()) + (v * local.getY()) + (w * local.getZ());
        }

        /** @brief Calculate the distance from a point to the surface of the mobius strip
         * @param point The point from which to calculate the distance
         * @return The distance from the point to the surface of the mobius strip
         */
        double getDistance(Math::Vector3D point) const {
            double angle = std::atan2(point.getY(), point.getX());
            double radial = std::sqrt(point.getX() * point.getX() + point.getY() * point.getY()) - _radius;
            double mobiusTwistAngle = angle / 2.0;
            double widhtoffset = radial * std::cos(mobiusTwistAngle) + point.getZ() * std::sin(mobiusTwistAngle);
            double normal = -radial * std::sin(mobiusTwistAngle) + point.getZ() * std::cos(mobiusTwistAngle);
            double surpluswidht = std::max(std::abs(widhtoffset) - (_width / 2.0), 0.0);

            return std::sqrt((normal * normal) + (surpluswidht * surpluswidht));
        }

        /** @brief Calculate the normal vector at a point on the surface of the mobius strip using the distance gradient
         * @param point The point on the surface of the mobius strip for which to calculate the normal vector
         * @return The normal vector at the given point on the surface of the mobius strip
         */
        Math::Vector3D getLocalNormal(Math::Vector3D point) const {
            Math::Vector3D distanceGradient(
                getDistance(point + Math::Vector3D(0.001, 0, 0)) - getDistance(point - Math::Vector3D(0.001, 0, 0)),
                getDistance(point + Math::Vector3D(0, 0.001, 0)) - getDistance(point - Math::Vector3D(0, 0.001, 0)),
                getDistance(point + Math::Vector3D(0, 0, 0.001)) - getDistance(point - Math::Vector3D(0, 0, 0.001))
            );

            if (distanceGradient.length() == 0)
                return Math::Vector3D(0, 0, 1);
            return distanceGradient.getNormal();
        }

    public:
        MobiusStrip() : RayTracer::APrimitive(Math::Point3D()), _radius(0), _width(0), _axis(Math::Vector3D(0, 0, 1)) {}
        /** @brief Construct a mobius strip primitive with the given parameters
         * @param center The center point of the mobius strip
         * @param radius The radius of the mobius strip
         * @param width The width of the mobius strip
         * @param axis The axis vector of the mobius strip
         */
        MobiusStrip(Math::Point3D center, double radius, double width, Math::Vector3D axis) : RayTracer::APrimitive(center), _radius(radius), _width(width), _axis(axis) {}
        ~MobiusStrip() override = default;

        /** @brief Calculate the intersection of a ray with the mobius strip
         * @param ray The ray to test for intersection
         * @return The point of intersection if the ray hits the mobius strip, or an empty point if there is no intersection
         */
        Math::Point3D hits(RayTracer::Ray &ray) const override {
            if (_radius <= 0 || _width <= 0)
                return Math::Point3D();

            double distance = 0.001;

            for (int i = 0; i < 700 && distance < (_radius + _width) * 8.0; i++) {
                Math::Point3D point = ray.getOrigin() + (ray.getDirection() * distance);
                double fieldDistance = getDistance(transformToLocalSpace(point));

                if (fieldDistance < 0.03)
                    return point;
                distance += std::max(fieldDistance * 0.45, 0.01);
            }
            return Math::Point3D();
        }

        /** @brief Get the normal vector at the point of intersection
         * @param hit The point of intersection
         * @param ray The ray that hit the mobius strip (unused in this implementation)
         * @return The normal vector at the point of intersection
         */
        Math::Vector3D getNormal(Math::Point3D hit, [[maybe_unused]] RayTracer::Ray ray) const override {
            return transformToWorldSpace(getLocalNormal(transformToLocalSpace(hit))).getNormal();
        }

        void applyScaling(Math::Vector3D scaling) override {
            _radius *= (scaling.getX() + scaling.getY()) / 2.0;
            _width *= scaling.getZ();
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
