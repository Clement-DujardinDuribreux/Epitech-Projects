/*
** EPITECH PROJECT, 2026
** IRendering
** File description:
** IRendering def
*/

#ifndef IRENDERING
    #define IRENDERING

    #include "Vector3D.hpp"
    #include "Point3D.hpp"
    #include "Ray.hpp"

    #include <vector>

namespace RayTracer {

class IPrimitive;
class ILight;

struct RenderingContext;

/** @class IRendering
 * @brief An interface for rendering
 * This interface defines the basic functionality for rendering a pixel based on the ray intersection with primitives and lighting in the scene. It includes a method for rendering the color of a pixel given the rendering context, the current primitive being rendered, the list of primitives in the scene, the list of light sources in the scene, the ray that hit the primitive, the point of intersection, and the base color of the primitive. Any class that implements this interface can be used as a renderer in the ray tracing engine, allowing for different rendering techniques to be implemented and used interchangeably.
 */
class IRendering {
    private:

    public:
        virtual ~IRendering() = default;

        /** @brief Render the color of the pixel based on the ray intersection with primitives and lighting 
         * @param context The rendering context containing information about the scene and rendering settings
         * @param currentObj The primitive currently being rendered
         * @param objs The list of primitives in the scene
         * @param lights The list of light sources in the scene
         * @param r The ray that hit the primitive
         * @param hit The point of intersection between the ray and the primitive
         * @param baseColor The base color of the primitive at the hit point
         * @return A vector representing the final color to render for the pixel, taking into account lighting and material properties
         */
        virtual Math::Vector3D Render(RayTracer::RenderingContext context, RayTracer::IPrimitive *currentObj, std::vector<RayTracer::IPrimitive *> objs, std::vector<RayTracer::ILight *> lights, RayTracer::Ray r, Math::Point3D hit, Math::Vector3D baseColor, int shininess) const = 0;
};

/** @struct RenderingContext
 * @brief A structure for holding rendering context information
 * This structure contains information about the rendering context, including a pointer to the renderer being used, ambient lighting intensity, diffuse multiplier for lighting calculations, the number of samples for ambient occlusion, the radius for ambient occlusion sampling, and the intensity of ambient occlusion. This context is passed to the Render method of the IRendering interface to provide necessary information for rendering calculations.
 */
struct RenderingContext {
    RayTracer::IRendering *renderer;
    double ambient;
    double diffuseMultiplier;
    int AOsamples;
    double AOradius;
    double AOintensity;
};

}

#endif