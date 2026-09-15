/*
** EPITECH PROJECT, 2026
** Pixel
** File description:
** Pixel def
*/

#ifndef TILE
    #define TILE

    #include "Vector3D.hpp"
    #include "ILight.hpp"
    #include "IPrimitive.hpp"
    #include <vector>
    #include <iostream>
    #include "Pixel.hpp"
    #include "Camera.hpp"
    #include <cmath>
    #include "IRendering.hpp"

namespace RayTracer {
/** @brief A tile class for rendering a portion of the image
 * This class represents a tile in the image and is responsible for rendering a specific portion of the image based on the provided rendering context. It uses the camera to generate rays for each pixel in the tile and then uses the rendering context to calculate the color of each pixel based on the ray intersections with primitives and lighting in the scene. The rendered pixels are stored in a vector, which can be accessed after rendering is complete.
 */
class Tile {

    public:
        /** @brief A structure to define the format of the tile
         * This structure contains information about the position and size of the tile within the overall image. It includes the x and y position of the tile, as well as the width (xSize) and height (ySize) of the tile. This information is used to determine which portion of the image the tile is responsible for rendering.
         */
        struct TileFormat {
            std::size_t xPosition;
            std::size_t yPosition;
            std::size_t xSize;
            std::size_t ySize;
        };

        /** @brief A structure to define the rendering context for the tile
         * This structure contains all the necessary information for rendering the tile, including a pointer to the renderer, the camera, the list of primitives and lights in the scene, ambient lighting, diffuse multiplier, image dimensions, tile format, a vector to store the rendered pixels, and settings for ambient occlusion. This context is passed to the render method of the tile to perform the rendering of the specified portion of the image.
         */
        struct renderContext {
            RayTracer::IRendering *renderer;
            RayTracer::Camera camera;
            std::vector<RayTracer::IPrimitive *> objs;
            std::vector<RayTracer::ILight *> lights;
            double ambient;
            double diffuseMultiplier;
            int width;
            int height;
            TileFormat tileFormat;
            std::vector<RayTracer::Pixel> pixels;
            int AOsamples;
            double AOradius;
            double AOintensity;
        };

        /** @brief Construct a tile with the specified rendering context
         * @param context The rendering context containing all necessary information for rendering the tile
         * This constructor initializes the tile with the provided rendering context, which includes information about the camera, primitives, lights, and rendering settings. The tile will use this context to render its portion of the image when the render method is called.
         */
        Tile(const renderContext &context) : _context(context) {}
        ~Tile() = default;

        /** @brief Render the tile based on the rendering context
         * This method performs the rendering of the tile by iterating through each pixel in the tile's area, generating rays from the camera for each pixel, and calculating the color of each pixel based on the ray intersections with primitives and lighting in the scene. The resulting colors are stored in the pixels vector of the rendering context, which can be accessed after rendering is complete.
         */
        void render() {
            for (std::size_t y = _context.tileFormat.yPosition; y < _context.tileFormat.ySize + _context.tileFormat.yPosition; y++) {
                for (std::size_t x = _context.tileFormat.xPosition; x < _context.tileFormat.xSize + _context.tileFormat.xPosition; x++) {
                    double u = _context.tileFormat.xSize == 1 ? 0.0 : static_cast<double>(x) / (_context.width - 1);
                    double v = _context.tileFormat.ySize == 1 ? 0.0 : 1.0 - (static_cast<double>(y) / (_context.height - 1));
                    RayTracer::Ray r = _context.camera.ray(u, v);
                    RayTracer::Pixel px;
                    RayTracer::RenderingContext context = {_context.renderer, _context.ambient, _context.diffuseMultiplier, _context.AOsamples, _context.AOradius, _context.AOintensity};
                    px.render(context, _context.objs, _context.lights, r);
                    _context.pixels.push_back(px);
                }
            }
        }

        renderContext _context;
    private:
};
}

#endif