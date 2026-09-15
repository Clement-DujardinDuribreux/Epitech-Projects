/*
** EPITECH PROJECT, 2026
** Torus
** File description:
** Torus def
*/

#ifndef TORUS
    #define TORUS

    #include "APrimitive.hpp"
    #include <cstdlib>
    #include <cmath>
    #include <algorithm>

namespace RayTracer {
/** @class Torus
 * @brief A class representing a torus primitive in the ray tracer scene
 * This class inherits from APrimitive and implements the necessary methods to calculate ray intersections and normals for a torus shape. 
 * The torus is defined by its center point, major radius, minor radius, and axis vector. The hits method calculates the intersection of a ray with the torus by 
 * solving a quartic equation derived from the torus implicit surface equation, while the getNormal method computes the normal vector at the point of intersection 
 * by evaluating the gradient of the torus surface at that point.
 */
class Torus : public RayTracer::APrimitive {
    private:
        double _majorRadius;
        double _minorRadius;
        Math::Vector3D _axis;  

        /** @brief Get the normalized axis vector of the torus
         * @return The normalized axis vector of the torus
         */
        Math::Vector3D getNormalizedAxis() const {
            if (_axis.length() == 0)
                return Math::Vector3D(0, 0, 1);
            return _axis.getNormal();
        }

        /** @brief Extract a pair of roots from the quartic equation solution
         * @param sign The sign to determine which pair of roots to extract
         * @param w The value of w calculated from the quartic solution
         * @param q The value of q calculated from the quartic solution
         * @param p The value of p calculated from the quartic solution
         * @param y The value of y calculated from the quartic solution
         * @param offset The offset to apply to the roots based on the original variable substitution
         * @param roots An array to store the extracted roots
         * @param count A reference to the count of valid roots extracted so far
         */
        void extractRootPair(double sign, double w, double q, double p,
            double y, double offset, double roots[4], int &count) const {
            double qOverW = (std::abs(w) > 1e-10) ? (sign * 2.0 * q / w) : 0.0;
            double inner = -(3.0 * p + 2.0 * y + qOverW);
            if (inner >= 0 && count + 2 <= 4) {
                double s = std::sqrt(inner);
                roots[count++] = (sign * w + s) / 2.0 - offset;
                roots[count++] = (sign * w - s) / 2.0 - offset;
            }
        }

        /** @brief Solve a quartic equation
         * @param c The coefficients of the quartic equation
         * @param roots An array to store the roots
         * @return The number of real roots found
         */
        int solveQuartic(const double c[5], double roots[4]) const {
            double a = c[3] / c[4];
            double b = c[2] / c[4];
            double cc = c[1] / c[4];
            double d = c[0] / c[4];
            double p = b - (3.0 * a * a / 8.0);
            double q = a * a * a / 8.0 - a * b / 2.0 + cc;
            double r = -3.0 * a * a * a * a / 256.0 + a * a * b / 16.0 - a * cc / 4.0 + d;

            double offset = a / 4.0;
            int count = 0;
            if (std::abs(q) < 1e-10) {
                double disc1 = p * p - 4.0 * r;
                if (disc1 < 0) return 0;
                double sq = std::sqrt(disc1);
                double t1 = (-p + sq) / 2.0;
                double t2 = (-p - sq) / 2.0;
                if (t1 >= 0 && count + 2 <= 4) {
                    double s = std::sqrt(t1);
                    roots[count++] = s - offset;
                    roots[count++] = -s - offset;
                }
                if (t2 >= 0 && count + 2 <= 4) {
                    double s = std::sqrt(t2);
                    roots[count++] = s - offset;
                    roots[count++] = -s - offset;
                }
                return count;
            }
            double cp = -p * p / 12.0 - r;
            double cq = -p * p * p / 108.0 + p * r / 3.0 - q * q / 8.0;
            double D  = cq * cq / 4.0 + cp * cp * cp / 27.0;
            double y;
            if (D >= 0) {
                double sqD = std::sqrt(D);
                double S = std::cbrt(-cq / 2.0 + sqD);
                double T = std::cbrt(-cq / 2.0 - sqD);
                y = -5.0 * p / 6.0 + S + T;
            } else {
                double denom = std::sqrt(std::max(0.0, -cp * cp * cp / 27.0));
                if (denom < 1e-10) {
                    y = -5.0 * p / 6.0;
                } else {
                    double arg = std::max(-1.0, std::min(1.0, (-cq / 2.0) / denom));
                    double phi = std::acos(arg);
                    y = -5.0 * p / 6.0 + 2.0 * std::sqrt(std::max(0.0, -cp / 3.0)) * std::cos(phi / 3.0);
                }
            }
            double w = std::sqrt(std::max(0.0, p + 2.0 * y));
            extractRootPair(1.0, w, q, p, y, offset, roots, count);
            extractRootPair(-1.0, w, q, p, y, offset, roots, count);
            return count;
        }

    public:
        Torus() : RayTracer::APrimitive(Math::Point3D()), _majorRadius(0), _minorRadius(0), _axis(Math::Vector3D(0, 0, 1)) {}
        /** @brief Create a torus primitive with specified parameters
         * @param center The center point of the torus
         * @param majorRadius The major radius of the torus (distance from the center to the middle of the tube)
         * @param minorRadius The minor radius of the torus (radius of the tube)
         * @param axis The axis vector around which the torus is oriented
         */
        Torus(Math::Point3D center, double majorRadius, double minorRadius, Math::Vector3D axis) : RayTracer::APrimitive(center), _majorRadius(majorRadius), _minorRadius(minorRadius), _axis(axis) {}
        ~Torus() override = default;
        
        /** @brief Calculate the intersection of a ray with the torus
         * @param ray The ray to test for intersection
         * @return The point of intersection if the ray hits the torus, or an empty point if there is no intersection
         */
        Math::Point3D hits(RayTracer::Ray &ray) const override {
            Math::Vector3D W = getNormalizedAxis();
            Math::Vector3D arbitrary(1, 0, 0);
            if (std::abs(W.getX()) > 0.9)
                arbitrary = Math::Vector3D(0, 1, 0);
            Math::Vector3D U = arbitrary.cross(W).getNormal();
            Math::Vector3D V = W.cross(U).getNormal();
            Math::Vector3D oc(ray.getOrigin().getX() - _center.getX(), ray.getOrigin().getY() - _center.getY(), ray.getOrigin().getZ() - _center.getZ());
            Math::Vector3D direction = ray.getDirection();
            double ox = oc.dot(U);
            double oy = oc.dot(V);
            double oz = oc.dot(W);
            double dx = direction.dot(U);
            double dy = direction.dot(V);
            double dz = direction.dot(W);
            double R2 = _majorRadius * _majorRadius;
            double r2 = _minorRadius * _minorRadius;
            double A  = dx*dx + dy*dy + dz*dz;
            double od = ox*dx + oy*dy + oz*dz;
            double sum = ox*ox + oy*oy + oz*oz;
            double G  = sum + R2 - r2;
            double C[5];
            C[4] = A * A;
            C[3] = 4.0 * A * od;
            C[2] = 2.0 * A * G + 4.0 * od*od - 4.0 * R2 * (dx*dx + dy*dy);
            C[1] = 4.0 * G * od - 8.0 * R2 * (ox*dx + oy*dy);
            C[0] = G*G - 4.0 * R2 * (ox*ox + oy*oy);
            double solutions[4];
            int numhits = solveQuartic(C, solutions);
            if (numhits == 0)
                return Math::Point3D();
            double t_min = INFINITY;
            bool hit = false;
            for (int i = 0; i < numhits; i++) {
                if (solutions[i] > 0.0001 && solutions[i] < t_min) {
                    t_min = solutions[i];
                    hit = true;
                }
            }
            if (!hit)
                return Math::Point3D();
            return ray.getOrigin() + (ray.getDirection() * t_min);
        }

        /** @brief Get the normal vector at a point on the torus
         * @param hit The point of intersection on the torus
         * @param ray The ray that intersected with the torus
         * @return The normal vector at the point of intersection, or an empty vector if no normal can be calculated
         */
        Math::Vector3D getNormal(Math::Point3D hit, [[maybe_unused]] RayTracer::Ray ray) const override {
            Math::Vector3D W = getNormalizedAxis();
            Math::Vector3D arbitrary(1, 0, 0);
            if (std::abs(W.getX()) > 0.9) arbitrary = Math::Vector3D(0, 1, 0);
            Math::Vector3D U = arbitrary.cross(W).getNormal();
            Math::Vector3D V = W.cross(U).getNormal();
            Math::Vector3D oc(hit.getX() - _center.getX(), hit.getY() - _center.getY(), hit.getZ() - _center.getZ());
            double localX = oc.dot(U);
            double localY = oc.dot(V);
            double localZ = oc.dot(W);
            double distToAxis = std::sqrt(localX * localX + localY * localY);
            if (distToAxis < 0.0001)
                return W;
            double k = 1.0 - (_majorRadius / distToAxis);
            Math::Vector3D localNormal(localX * k, localY * k, localZ);
            Math::Vector3D worldNormal = (U * localNormal.getX()) + (V * localNormal.getY()) + (W * localNormal.getZ());
            return worldNormal.getNormal();
        }

        void applyScaling(Math::Vector3D scaling) override {
            _majorRadius *= (scaling.getX() + scaling.getY()) / 2.0;
            _minorRadius *= scaling.getZ();
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