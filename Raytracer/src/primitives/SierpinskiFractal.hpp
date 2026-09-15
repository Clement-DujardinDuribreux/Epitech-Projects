/*
** EPITECH PROJECT, 2026
** SierpinskiFractal
** File description:
** SierpinskiFractal primitive
*/

#ifndef SIERPINSKI_FRACTAL
#define SIERPINSKI_FRACTAL

#include "APrimitive.hpp"
#include "Pyramide.hpp"
#include <cmath>
#include <vector>

namespace RayTracer {
/** @class SierpinskiFractal
 * @brief A Sierpinski fractal primitive class
 * This class represents a Sierpinski fractal primitive in the 3D scene and is responsible for calculating ray intersections and normals for the fractal shape.
 * The Sierpinski fractal is defined by its vertices and depth of recursion. The hits method calculates the intersection of a ray with the fractal by checking 
 * each pyramide for intersection and returning the closest hit point. The getNormal method returns the normal vector at the point of intersection by determining 
 * which pyramide was hit and returning its normal vector. The fractal is constructed using the specified vertices and depth, and the center point of the fractal is 
 * calculated as the average of the vertices' coordinates. The class also includes methods for creating and destroying the fractal primitive, as well as verifying the 
 * configuration settings for the vertices and depth.
 * @see APrimitive 
 */
class SierpinskiFractal : public RayTracer::APrimitive {
    private:
        std::vector<RayTracer::Pyramide> _pyramides;

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

        /** @brief Get the midpoint of two points
         * @param a The first point
         * @param b The second point
         * @return The midpoint of the two points
         */
        static Math::Point3D getMidpoint(Math::Point3D a, Math::Point3D b) {
            return Math::Point3D(
                (a.getX() + b.getX()) / 2.0,
                (a.getY() + b.getY()) / 2.0,
                (a.getZ() + b.getZ()) / 2.0
            );
        }

        /** @brief Generate the Sierpinski fractal recursively
         * @param a The first vertex of the current pyramide
         * @param b The second vertex of the current pyramide
         * @param c The third vertex of the current pyramide
         * @param d The fourth vertex of the current pyramide
         * @param e The fifth vertex of the current pyramide
         * @param currentDepth The current depth of recursion
         */
        static bool samePoint(Math::Point3D a, Math::Point3D b) {
            return std::abs(a.getX() - b.getX()) < 0.000001
                && std::abs(a.getY() - b.getY()) < 0.000001
                && std::abs(a.getZ() - b.getZ()) < 0.000001;
        }

        /** @brief Generate the Sierpinski fractal recursively
         * @param a The first vertex of the current pyramide
         * @param b The second vertex of the current pyramide
         * @param c The third vertex of the current pyramide
         * @param d The fourth vertex of the current pyramide
         * @param e The fifth vertex of the current pyramide
         * @param currentDepth The current depth of recursion
         */
        void generateFractal(Math::Point3D a, Math::Point3D b, Math::Point3D c, Math::Point3D d, Math::Point3D e, int currentDepth) {
            if (currentDepth == 0) {
                _pyramides.emplace_back(a, b, c, d, e);
                return;
            }

            Math::Point3D mAB = getMidpoint(a, b);
            Math::Point3D mBC = getMidpoint(b, c);
            Math::Point3D mCD = getMidpoint(c, d);
            Math::Point3D mDA = getMidpoint(d, a);
            Math::Point3D center = getMidpoint(a, c);
            Math::Point3D mAE = getMidpoint(a, e);
            Math::Point3D mBE = getMidpoint(b, e);
            Math::Point3D mCE = getMidpoint(c, e);
            Math::Point3D mDE = getMidpoint(d, e);

            generateFractal(a, mAB, center, mDA, mAE, currentDepth - 1);
            generateFractal(mAB, b, mBC, center, mBE, currentDepth - 1);
            generateFractal(center, mBC, c, mCD, mCE, currentDepth - 1);
            generateFractal(mDA, center, mCD, d, mDE, currentDepth - 1);
            generateFractal(mAE, mBE, mCE, mDE, e, currentDepth - 1);
        }

    public:
        SierpinskiFractal() : RayTracer::APrimitive(Math::Point3D()) {}

        /** @brief Create a Sierpinski fractal primitive from its vertices and depth
         * @param a The first vertex of the fractal
         * @param b The second vertex of the fractal
         * @param c The third vertex of the fractal
         * @param d The fourth vertex of the fractal
         * @param e The fifth vertex of the fractal
         * @param depth The depth of recursion for generating the fractal
         */
        SierpinskiFractal(Math::Point3D a, Math::Point3D b, Math::Point3D c, Math::Point3D d, Math::Point3D e, int depth)
            : RayTracer::APrimitive(getCenterFromPoints(a, b, c, d, e))
        {
            generateFractal(a, b, c, d, e, depth);
        }

        ~SierpinskiFractal() override = default;

        /** @brief Calculate the intersection of a ray with the Sierpinski fractal
         * @param ray The ray to test for intersection
         * @return The point of intersection if the ray hits the fractal, or an empty point if there is no intersection
         * This method checks each pyramide in the fractal for intersection with the ray and returns the closest hit point. If no pyramides are hit, it returns an empty point.
         */
        Math::Point3D hits(RayTracer::Ray &ray) const override {
            Math::Point3D hit;
            double shortDist = INFINITY;

            for (int i = 0; i < (int)_pyramides.size(); i++) {
                Math::Point3D currentHit = _pyramides[i].hits(ray);
                if (currentHit != Math::Point3D()) {
                    double dist = std::sqrt(
                        std::pow(currentHit.getX() - ray.getOrigin().getX(), 2) +
                        std::pow(currentHit.getY() - ray.getOrigin().getY(), 2) +
                        std::pow(currentHit.getZ() - ray.getOrigin().getZ(), 2)
                    );
                    if (dist < shortDist) {
                        hit = currentHit;
                        shortDist = dist;
                    }
                }
            }
            return hit;
        }

        /** @brief Get the normal vector at a point on the Sierpinski fractal
         * @param hitPoint The point on the fractal for which to calculate the normal
         * @param ray The ray that intersected with the fractal
         * @return The normal vector at the intersection point, or an empty vector if no normal can be calculated
         * This method determines which pyramide in the fractal was hit by the ray and returns the normal vector of that pyramide at the point of intersection. If no pyramides are hit, it returns an empty vector.
         */
        Math::Vector3D getNormal(Math::Point3D hitPoint, RayTracer::Ray ray) const override {
            for (const RayTracer::Pyramide &pyramide : _pyramides) {
                Math::Point3D pyramideHit = pyramide.hits(ray);

                if (pyramideHit != Math::Point3D() && samePoint(pyramideHit, hitPoint))
                    return pyramide.getNormal(hitPoint, ray);
            }
            return Math::Vector3D();
        }

        void applyTranslation(Math::Vector3D translation) override {
            for (RayTracer::Pyramide &pyramide : _pyramides)
                pyramide.applyTranslation(translation);
        }

        void applyScaling(Math::Vector3D scaling) override {
            for (RayTracer::Pyramide &pyramide : _pyramides)
                pyramide.applyScaling(scaling);
        }

        void applyRotation(Math::Vector3D rotation) override {
            for (RayTracer::Pyramide &pyramide : _pyramides)
                pyramide.applyRotation(rotation);
        }
};
}

#endif
