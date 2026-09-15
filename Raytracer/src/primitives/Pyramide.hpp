/*
** EPITECH PROJECT, 2026
** Pyramide
** File description:
** Pyramide primitive
*/

#ifndef PYRAMIDE_HPP
#define PYRAMIDE_HPP

#include "APrimitive.hpp"
#include "Triangle.hpp"

#include <cmath>
#include <vector>

namespace RayTracer {
/** @class Pyramide
 * @brief A pyramide primitive
 * This class represents a pyramide primitive in the 3D scene and is responsible for calculating ray intersections and normals for the pyramide shape. 
 * The pyramide is defined by its five vertices (a, b, c, d, e) and is composed of four triangular faces. The hits method calculates the intersection of a ray with 
 * the pyramide by checking each face for intersection and returning the closest hit point. The getNormal method returns the normal vector at the point of 
 * intersection by determining which face was hit and returning its normal vector. The pyramide is constructed using the five vertices, and the center point of 
 * the pyramide is calculated as the average of the vertices' coordinates. The class also includes methods for creating and destroying the pyramide primitive, 
 * as well as verifying the configuration settings for the vertices.
 * @see APrimitive
 */
class Pyramide : public RayTracer::APrimitive {
    private:
        std::vector<RayTracer::Triangle> faces;

        /** @brief Get the center point of the pyramide from its vertices
         * @param a The first vertex of the pyramide
         * @param b The second vertex of the pyramide
         * @param c The third vertex of the pyramide
         * @param d The fourth vertex of the pyramide
         * @param e The fifth vertex of the pyramide
         * @return The center point of the pyramide
         */
        static Math::Point3D getCenterFromPoints(Math::Point3D a, Math::Point3D b, Math::Point3D c, Math::Point3D d, Math::Point3D e) {
            return Math::Point3D(
                (a.getX() + b.getX() + c.getX() + d.getX() + e.getX()) / 5.0,
                (a.getY() + b.getY() + c.getY() + d.getY() + e.getY()) / 5.0,
                (a.getZ() + b.getZ() + c.getZ() + d.getZ() + e.getZ()) / 5.0
            );
        }

        /** @brief Check if two points are the same within a small tolerance
         * @param a The first point to compare
         * @param b The second point to compare
         * @return True if the points are considered the same, false otherwise
         */
        static bool samePoint(Math::Point3D a, Math::Point3D b) {
            return std::abs(a.getX() - b.getX()) < 0.000001
                && std::abs(a.getY() - b.getY()) < 0.000001
                && std::abs(a.getZ() - b.getZ()) < 0.000001;
        }

    public:
        Pyramide() : RayTracer::APrimitive(Math::Point3D()) {}

        /** @brief Create a pyramide primitive from its vertices
         * @param a The first vertex of the pyramide
         * @param b The second vertex of the pyramide
         * @param c The third vertex of the pyramide
         * @param d The fourth vertex of the pyramide
         * @param e The fifth vertex of the pyramide
         */
        Pyramide(Math::Point3D a, Math::Point3D b, Math::Point3D c, Math::Point3D d, Math::Point3D e)
            : RayTracer::APrimitive(getCenterFromPoints(a, b, c, d, e))
        {
            faces.emplace_back(a, c, b);
            faces.emplace_back(a, d, c);
            faces.emplace_back(a, e, b);
            faces.emplace_back(b, e, c);
            faces.emplace_back(c, e, d);
            faces.emplace_back(d, e, a);
        }

        ~Pyramide() override = default;

        Pyramide(const Pyramide &) = delete;
        Pyramide &operator=(const Pyramide &) = delete;
        Pyramide(Pyramide &&) = default;
        Pyramide &operator=(Pyramide &&) = default;

        /** @brief Calculate the intersection of a ray with the pyramide
         * @param ray The ray to test for intersection
         * @return The point of intersection if the ray hits the pyramide, or an empty point if there is no intersection
         * This method checks each face of the pyramide for intersection with the ray and returns the closest hit point. If no faces are hit, it returns an empty point.
         */
        Math::Point3D hits(RayTracer::Ray &ray) const override {
            Math::Point3D hit;
            double shortDist = INFINITY;

            for (int i = 0; i < (int)faces.size(); i++) {
                Math::Point3D tmpHit = faces[i].hits(ray);
                if (tmpHit != Math::Point3D()) {
                    Math::Vector3D vec(
                        tmpHit.getX() - ray.getOrigin().getX(),
                        tmpHit.getY() - ray.getOrigin().getY(),
                        tmpHit.getZ() - ray.getOrigin().getZ()
                    );
                    double dist = vec.length();
                    if (dist < shortDist) {
                        hit = tmpHit;
                        shortDist = dist;
                    }
                }
            }
            return hit;
        }

        /** @brief Get the normal vector at a point on the pyramide
         * @param hitPoint The point on the pyramide for which to calculate the normal
         * @param ray The ray that intersected with the pyramide
         * @return The normal vector at the intersection point, or an empty vector if no normal can be calculated
         */
        Math::Vector3D getNormal(Math::Point3D hitPoint, RayTracer::Ray ray) const override {
            for (const RayTracer::Triangle &face : faces) {
                Math::Point3D faceHit = face.hits(ray);

                if (faceHit != Math::Point3D() && samePoint(faceHit, hitPoint))
                    return face.getNormal(hitPoint, ray);
            }
            return Math::Vector3D();
        }

        void applyTranslation(Math::Vector3D translation) override {
            for (RayTracer::Triangle &face : faces)
                face.applyTranslation(translation);
        }

        void applyScaling(Math::Vector3D scaling) override {
            for (RayTracer::Triangle &face : faces)
                face.applyScaling(scaling);
        }

        void applyRotation(Math::Vector3D rotation) override {
            for (RayTracer::Triangle &face : faces)
                face.applyRotation(rotation);
        }
};
}

#endif
