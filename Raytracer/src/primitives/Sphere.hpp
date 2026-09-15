/*
** EPITECH PROJECT, 2026
** Sphere
** File description:
** Sphere def
*/

#ifndef SPHERE
    #define SPHERE

    #include "APrimitive.hpp"

namespace RayTracer {
/** @class Sphere
 * @brief A class representing a sphere primitive in the ray tracer scene
 * This class inherits from APrimitive and implements the necessary methods to calculate ray intersections and normals for a sphere shape. 
 * The sphere is defined by its center point and radius. The hits method calculates the intersection of a ray with the sphere using the quadratic formula, 
 * while the getNormal method computes the normal vector at the point of intersection by normalizing the vector from the center of the sphere to the hit point.
 * @see APrimitive
 */
class Sphere : public RayTracer::APrimitive {
    private:
        double _radius;

    public:
        Sphere() : RayTracer::APrimitive(Math::Point3D()), _radius(0) {}
        /** @brief Create a sphere primitive with a specified center and radius
         * @param center The center point of the sphere
         * @param radius The radius of the sphere
         */
        Sphere(Math::Point3D center, double radius) : RayTracer::APrimitive(center), _radius(radius) {}
        ~Sphere() override = default;

        /** @brief Calculate the intersection of a ray with the sphere
         * @param ray The ray to test for intersection
         * @return The point of intersection if the ray hits the sphere, or an empty point if there is no intersection
         * This method uses the quadratic formula to determine if and where the ray intersects with the sphere. It returns the closest hit point if there are intersections, or an empty point if there are none.
         */
        Math::Point3D hits(RayTracer::Ray &ray) const override {
            Math::Vector3D oc(
                ray.getOrigin().getX() - _center.getX(),
                ray.getOrigin().getY() - _center.getY(),
                ray.getOrigin().getZ() - _center.getZ()
            );
            double a = ray.getDirection().dot(ray.getDirection());
            double b = 2.0 * ray.getDirection().dot(oc);
            double c = oc.dot(oc) - (_radius * _radius);
            double delta = (b * b) - (4.0 * a * c);
        
            if (delta < 0)
                return Math::Point3D();

            double k1 = (-b - sqrt(delta)) / (2.0 * a);
            double k2 = (-b + sqrt(delta)) / (2.0 * a);
        
            double k = 0;
            if (k1 > 0)
                k = k1;
            else if (k2 > 0)
                k = k2;
            else
                return Math::Point3D();
            Math::Point3D p = ray.getOrigin() + (ray.getDirection() * k);
            return p;
        }

        /** @brief Get the normal vector at a point on the sphere
         * @param hit The point on the sphere for which to calculate the normal
         * @param ray The ray that intersected with the sphere
         * @return The normal vector at the intersection point, or an empty vector if no normal can be calculated
         * This method computes the normal vector at the point of intersection by normalizing the vector from the center of the sphere to the hit point.
         */
        Math::Vector3D getNormal(Math::Point3D hit, [[maybe_unused]] RayTracer::Ray ray) const override {
            return Math::Vector3D(hit.getX() - _center.getX(), hit.getY() - _center.getY(), hit.getZ() - _center.getZ()).getNormal();
        }

        void applyTranslation(Math::Vector3D translation) override {
            _center = _center + translation;
        }

        void applyScaling(Math::Vector3D scaling) override {
            _radius *= std::max({scaling.getX(), scaling.getY(), scaling.getZ()});
        }

        void applyRotation([[maybe_unused]] Math::Vector3D rotation) override {}
};
}

#endif