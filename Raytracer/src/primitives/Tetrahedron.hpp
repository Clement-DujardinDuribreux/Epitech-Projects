/*
** EPITECH PROJECT, 2026
** Tetrahedron
** File description:
** Tetrahedron primitive
*/

#ifndef TETRAHEDRON
    #define TETRAHEDRON

    #include "APrimitive.hpp"
    #include "Triangle.hpp"

    #include <cmath>

namespace RayTracer {
/** @class Tetrahedron
 * @brief A class representing a tetrahedron primitive in the ray tracer scene
 * This class inherits from APrimitive and implements the necessary methods to calculate ray intersections and normals for a tetrahedron shape. 
 * The tetrahedron is defined by its four vertices. The hits method calculates the intersection of a ray with the tetrahedron by checking each of its triangular faces, 
 * while the getNormal method computes the normal vector at the point of intersection by determining which face was hit and returning its normal.
 * @see APrimitive
 */
class Tetrahedron : public RayTracer::APrimitive {
    private:
        std::vector<RayTracer::Triangle> faces;

        /** @brief Get the center point of the tetrahedron from its vertices
         * @param a The first vertex of the tetrahedron
         * @param b The second vertex of the tetrahedron
         * @param c The third vertex of the tetrahedron
         * @param d The fourth vertex of the tetrahedron
         * @return The center point of the tetrahedron
         */
        static Math::Point3D getCenterFromPoints(Math::Point3D a, Math::Point3D b, Math::Point3D c, Math::Point3D d) {
            return Math::Point3D(
                (a.getX() + b.getX() + c.getX() + d.getX()) / 4.0,
                (a.getY() + b.getY() + c.getY() + d.getY()) / 4.0,
                (a.getZ() + b.getZ() + c.getZ() + d.getZ()) / 4.0
            );
        }

    public:
        Tetrahedron() : RayTracer::APrimitive(Math::Point3D()) {}
        /** @brief Construct a tetrahedron from four vertices
         * @param a The first vertex of the tetrahedron
         * @param b The second vertex of the tetrahedron
         * @param c The third vertex of the tetrahedron
         * @param d The fourth vertex of the tetrahedron
         */
        Tetrahedron(Math::Point3D a, Math::Point3D b, Math::Point3D c, Math::Point3D d) : RayTracer::APrimitive(getCenterFromPoints(a, b, c, d)) {
            faces.emplace_back(a, b, c);
            faces.emplace_back(a, b, d);
            faces.emplace_back(a, c, d);
            faces.emplace_back(b, c, d);
        }
        ~Tetrahedron() override = default;

        /** @brief Calculate the intersection of a ray with the tetrahedron
         * @param ray The ray to check for intersection
         * @return The point of intersection, or an empty point if no intersection is found
         */
        Math::Point3D hits(RayTracer::Ray &ray) const override {
            Math::Point3D hit = Math::Point3D();
            double shortDist = INFINITY;

            for (const RayTracer::Triangle &face : faces) {
                Math::Point3D tmpHit = face.hits(ray);
                double dist = Math::Vector3D(tmpHit.getX() - ray.getOrigin().getX(), tmpHit.getY() - ray.getOrigin().getY(), tmpHit.getZ() - ray.getOrigin().getZ()).length();
                if (tmpHit != Math::Point3D() && dist < shortDist) {
                    hit = tmpHit;
                    shortDist = dist;
                }
            }
            return hit;
        }

        /** @brief Get the normal vector at a point on the tetrahedron
         * @param hit The point of intersection on the tetrahedron
         * @param ray The ray that intersected with the tetrahedron
         * @return The normal vector at the point of intersection, or an empty vector if no normal can be calculated
         */
        Math::Vector3D getNormal([[maybe_unused]] Math::Point3D hit, [[maybe_unused]] RayTracer::Ray ray) const override {
            Math::Vector3D normal;
            double shortDist = INFINITY;

            for (const RayTracer::Triangle &face : faces) {
                Math::Point3D hit = face.hits(ray);
                double dist = Math::Vector3D(hit.getX() - ray.getOrigin().getX(), hit.getY() - ray.getOrigin().getY(), hit.getZ() - ray.getOrigin().getZ()).length();
                if (hit != Math::Point3D() && dist < shortDist) {
                    normal = face.getNormal(hit, ray);
                    shortDist = dist;
                }
            }
            return normal;
        }

        void applyScaling(Math::Vector3D scaling) override {
            for (RayTracer::Triangle &face : faces)
                face.applyScaling(scaling);
        }

        void applyRotation(Math::Vector3D rotation) override {
            for (RayTracer::Triangle &face : faces)
                face.applyRotation(rotation);
        }

        void applyTranslation(Math::Vector3D translation) override {
            for (RayTracer::Triangle &face : faces)
                face.applyTranslation(translation);
        }
};
}

#endif
