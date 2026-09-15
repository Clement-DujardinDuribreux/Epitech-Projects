/*
** EPITECH PROJECT, 2026
** Pixel
** File description:
** Pixel def
*/

#ifndef PIXEL
    #define PIXEL

    #include "Vector3D.hpp"
    #include "ILight.hpp"
    #include "IMaterial.hpp"
    #include "IPrimitive.hpp"
    #include "Ray.hpp"
    #include "IRendering.hpp"
    #include <vector>
    #include <iostream>

namespace RayTracer {
/** @brief A pixel class for representing a point in the image
 * This class represents a pixel in the 3D scene and is responsible for storing and updating the color of the pixel.
 */
class Pixel {
    private:
        Math::Vector3D _color;

    public:
        Pixel() : _color(BASE_COLOR) {}
        ~Pixel() = default;

        /** @brief Display the color of the pixel
         * This method outputs the color of the pixel in a format suitable for PPM image output (R G B).
         */
        std::string display() const {
            return std::to_string(std::max(0, std::min(255, static_cast<int>(_color.getX())))) + " " + std::to_string(std::max(0, std::min(255, static_cast<int>(_color.getY())))) + " " + std::to_string(std::max(0, std::min(255, static_cast<int>(_color.getZ())))) + "\n";
        }

        /** @brief Get the closest hit of a ray from a list of hits
         * @param hits The list of hits
         * @param origin The origin point
         * @return The closest hit
         */
        static std::pair<Math::Point3D, RayTracer::IPrimitive *> getClosest(std::vector<std::pair<Math::Point3D, RayTracer::IPrimitive *>> hits, Math::Point3D origin) {
            size_t r = 0;
            double rl = Math::Vector3D(hits[0].first.getX() - origin.getX(), hits[0].first.getY() - origin.getY(), hits[0].first.getZ() - origin.getZ()).length();
            for (size_t i = 1; i < hits.size(); i++) {
                double l = Math::Vector3D(hits[i].first.getX() - origin.getX(), hits[i].first.getY() - origin.getY(), hits[i].first.getZ() - origin.getZ()).length();
                if (l < rl) {
                    r = i;
                    rl = l;
                }
            }
            return hits[r];
        }

        /** @brief Render the color of the pixel based on the ray intersection with primitives and lighting 
         * @param context The rendering context containing information about the scene and rendering settings
         * @param objs The list of primitives in the scene
         * @param lights The list of light sources in the scene
         * @param ray The ray being traced for this pixel
         * This method calculates the color of the pixel by determining the closest intersection of the ray with the primitives 
         * in the scene, and then applying the rendering algorithm defined in the material of the intersected primitive. 
         * It takes into account the lighting from all light sources, shadows, and ambient occlusion to produce the 
         * final color of the pixel. The resulting color is stored in the _color member variable, which can then be 
         * displayed using the display() method.
         */
        void render(RayTracer::RenderingContext context, std::vector<RayTracer::IPrimitive *> objs, std::vector<RayTracer::ILight *> lights, RayTracer::Ray ray) {
            std::vector<std::pair<Math::Point3D, RayTracer::IPrimitive *>> hits;

            for (RayTracer::IPrimitive *obj : objs) {
                Math::Point3D hit = obj->hits(ray);
                if (hit != Math::Point3D())
                    hits.push_back({hit, obj});
            }
            if (hits.empty())
                return;
            std::pair<Math::Point3D, RayTracer::IPrimitive *> pair= Pixel::getClosest(hits, ray.getOrigin());
            Math::Point3D hit = pair.first;
            RayTracer::IPrimitive *obj = pair.second;
                                    
            if (!obj->getMaterial())
                return;
            _color = context.renderer->Render(context, obj, objs, lights, ray, hit, obj->getMaterial()->getRendering(obj, objs, lights, ray, hit, context), obj->getMaterial()->getShininess());
            //_color = obj->getMaterial()->getRendering(obj, objs, lights, ray, hit, context);
        }
};
}

#endif