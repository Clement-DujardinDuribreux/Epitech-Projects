/*
** EPITECH PROJECT, 2026
** Triangle
** File description:
** Triangle primitive
*/

#ifndef TRIANGLE
    #define TRIANGLE

    #include "APrimitive.hpp"

    #include <cmath>

namespace RayTracer {
/** @class Triangle
 * @brief A primitive representing a triangle in 3D space for ray tracing
 * This class inherits from APrimitive and implements the necessary methods to calculate ray intersections and normals for a triangle shape.
 * The triangle is defined by its three vertices, and the hits method uses the Möller–Trumbore intersection algorithm to determine if and where a ray intersects 
 * the triangle. The getNormal method calculates the normal vector at the point of intersection by computing the cross product of two edges of the triangle and 
 * normalizing the result.
 * @see APrimitive
 */
class Triangle : public RayTracer::APrimitive {
    private:
        Math::Point3D _a;
        Math::Point3D _b;
        Math::Point3D _c;

        Math::Vector3D _sideAB;
        Math::Vector3D _sideAC;
        Math::Vector3D _normal;

        void update() {
            _sideAB = createVector(_a, _b);
            _sideAC = createVector(_a, _c);
            Math::Vector3D n = intersection(_sideAB, _sideAC);
            if (n.length() > 0)
                _normal = n.getNormal();
            else
                _normal = Math::Vector3D();
        }

        /** @brief Create a vector from one point to another
         * @param from The starting point of the vector
         * @param to The ending point of the vector
         * @return A Vector3D representing the direction and magnitude from the 'from' point to the 'to' point
         */
        static Math::Vector3D createVector(Math::Point3D from, Math::Point3D to) {
            return Math::Vector3D(
                to.getX() - from.getX(),
                to.getY() - from.getY(),
                to.getZ() - from.getZ()
            );
        }

        /** @brief Get the normalized axis vector of the torus
         * @param first The first vector to use in the cross product
         * @param second The second vector to use in the cross product
         * @return The normalized axis vector of the torus
         */
        static Math::Vector3D intersection(Math::Vector3D first, Math::Vector3D second) {
            return Math::Vector3D(
                first.getY() * second.getZ() - first.getZ() * second.getY(),
                first.getZ() * second.getX() - first.getX() * second.getZ(),
                first.getX() * second.getY() - first.getY() * second.getX()
            );
        }

        /** @brief Get the center point of the triangle from its vertices
         * @param a The first vertex of the triangle
         * @param b The second vertex of the triangle
         * @param c The third vertex of the triangle
         * @return The center point of the triangle
         */
        static Math::Point3D getCenterFromPoints(Math::Point3D a, Math::Point3D b, Math::Point3D c) {
            return Math::Point3D(
                (a.getX() + b.getX() + c.getX()) / 3.0,
                (a.getY() + b.getY() + c.getY()) / 3.0,
                (a.getZ() + b.getZ() + c.getZ()) / 3.0
            );
        }

    public:
        Triangle() : RayTracer::APrimitive(Math::Point3D()), _a(Math::Point3D()), _b(Math::Point3D()), _c(Math::Point3D()) {
            update();
        }
        /** @brief Construct a triangle from three vertices
         * @param a The first vertex of the triangle
         * @param b The second vertex of the triangle
         * @param c The third vertex of the triangle
         */
        Triangle(Math::Point3D a, Math::Point3D b, Math::Point3D c) : RayTracer::APrimitive(getCenterFromPoints(a, b, c)), _a(a), _b(b), _c(c) {
            update();
        }
        ~Triangle() override = default;

        Triangle(const Triangle &) = delete;
        Triangle &operator=(const Triangle &) = delete;
        Triangle(Triangle &&) = default;
        Triangle &operator=(Triangle &&) = default;

        /** @brief Calculate the intersection of a ray with the triangle
         * @param ray The ray to check for intersection
         * @return The point of intersection, or an empty point if no intersection is found
         */
        Math::Point3D hits(RayTracer::Ray &ray) const override {
            Math::Vector3D h = intersection(ray.getDirection(), _sideAC);
            double det = _sideAB.dot(h);

            if (std::abs(det) < 0.000001)
                return Math::Point3D();

            double invDet = 1.0 / det;
            Math::Vector3D originToA = createVector(_a, ray.getOrigin());
            double u = invDet * originToA.dot(h);

            if (u < 0.0 || u > 1.0)
                return Math::Point3D();

            Math::Vector3D q = intersection(originToA, _sideAB);
            double v = invDet * ray.getDirection().dot(q);

            if (v < 0.0 || u + v > 1.0)
                return Math::Point3D();

            double distance = invDet * _sideAC.dot(q);
            if (distance <= 0.000001)
                return Math::Point3D();
            return ray.getOrigin() + (ray.getDirection() * distance);
        }

        /** @brief Get the normal vector at a point on the triangle
         * @param hit The point of intersection on the triangle
         * @param ray The ray that intersected with the triangle
         * @return The normal vector at the point of intersection, or an empty vector if no normal can be calculated
         */
        Math::Vector3D getNormal([[maybe_unused]] Math::Point3D hit, [[maybe_unused]] RayTracer::Ray ray) const override {
            if (_normal.length() == 0)
                return Math::Vector3D();
            return _normal * (_normal.dot(ray.getDirection()) < 0 ? 1 : -1);
        }

        void applyTranslation(Math::Vector3D translation) override {
            _a = _a + translation;
            _b = _b + translation;
            _c = _c + translation;
            _center = getCenterFromPoints(_a, _b, _c);
            update();
        }

        void applyScaling(Math::Vector3D scaling) override {
            _a = Math::Point3D(_center.getX() + (_a.getX() - _center.getX()) * scaling.getX(), _center.getY() + (_a.getY() - _center.getY()) * scaling.getY(), _center.getZ() + (_a.getZ() - _center.getZ()) * scaling.getZ());
            _b = Math::Point3D(_center.getX() + (_b.getX() - _center.getX()) * scaling.getX(), _center.getY() + (_b.getY() - _center.getY()) * scaling.getY(), _center.getZ() + (_b.getZ() - _center.getZ()) * scaling.getZ());
            _c = Math::Point3D(_center.getX() + (_c.getX() - _center.getX()) * scaling.getX(), _center.getY() + (_c.getY() - _center.getY()) * scaling.getY(), _center.getZ() + (_c.getZ() - _center.getZ()) * scaling.getZ());
            update();
        }

        void applyRotation(Math::Vector3D rotation) override {
            Math::Vector3D centerVec(_center.getX(), _center.getY(), _center.getZ());
            Math::Vector3D aVec(_a.getX(), _a.getY(), _a.getZ());
            Math::Vector3D bVec(_b.getX(), _b.getY(), _b.getZ());
            Math::Vector3D cVec(_c.getX(), _c.getY(), _c.getZ());

            Math::Vector3D aRot = Math::Vector3D::rotateAroundPoint(aVec, centerVec, rotation);
            Math::Vector3D bRot = Math::Vector3D::rotateAroundPoint(bVec, centerVec, rotation);
            Math::Vector3D cRot = Math::Vector3D::rotateAroundPoint(cVec, centerVec, rotation);

            _a = Math::Point3D(aRot.getX(), aRot.getY(), aRot.getZ());
            _b = Math::Point3D(bRot.getX(), bRot.getY(), bRot.getZ());
            _c = Math::Point3D(cRot.getX(), cRot.getY(), cRot.getZ());
            _center = getCenterFromPoints(_a, _b, _c);
            update();
        }
};
}

#endif
