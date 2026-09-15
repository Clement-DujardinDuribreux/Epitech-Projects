/*
** EPITECH PROJECT, 2026
** Pixel
** File description:
** Pixel def
*/

#ifndef TILEMAP
    #define TILEMAP

    #include <vector>
    #include <iostream>
    #include <atomic>
    #include <mutex>
    #include "Tile.hpp"
    #include "PoolThread.hpp"

namespace RayTracer {
/** @class TileMap 
 * @brief A tile map class for managing and rendering multiple tiles
 * This class manages a collection of tiles that together cover the entire image. It is responsible for creating the tiles based on the provided rendering context, rendering each tile using a thread pool for parallel execution, and displaying the final rendered image by combining the results from all tiles. The tile map divides the image into smaller sections (tiles) to optimize rendering performance and allow for concurrent processing of different parts of the image.
 */
class TileMap {
    private:
        std::vector<RayTracer::Tile> _tiles;
        RayTracer::Tile::renderContext _baseContext;

    public:
        /** @brief Construct a tile map with the specified rendering context
         * @param context The rendering context containing all necessary information for rendering the tiles
         * This constructor initializes the tile map by dividing the image into smaller tiles based on the provided rendering context. It calculates the position and size of each tile and creates a Tile object for each section of the image. The tiles are stored in a vector, which can be accessed for rendering and displaying the final image.
         */
        TileMap(RayTracer::Tile::renderContext context) : _baseContext(context) {
            double xPosition = 0.0;
            double yPosition = 0.0;
            int nbTilesX = (context.width + 31) / 32;
            int nbTilesY = (context.height + 31) / 32;
            for (double y = 0; y < nbTilesY; y++) {
                for (double x = 0; x < nbTilesX; x++) {
                    RayTracer::Tile::renderContext uniqueContext = context;
                    RayTracer::Tile::TileFormat format;
                    format.xSize = (xPosition + 32 > uniqueContext.width) ? (uniqueContext.width - xPosition) : 32;
                    format.ySize = (yPosition + 32 > uniqueContext.height) ? (uniqueContext.height - yPosition) : 32;
                    format.xPosition = xPosition;
                    format.yPosition = yPosition;
                    uniqueContext.tileFormat = format;
                    RayTracer::Tile tile(uniqueContext);
                    _tiles.push_back(tile);
                    xPosition += format.xSize;
                }
                yPosition += 32;
                xPosition = 0;
            }
        }

        ~TileMap() = default;

        /** @brief Render the tile map using a thread pool
         * This method renders each tile in the tile map using a thread pool for concurrent execution. It iterates through each tile and adds a rendering task to the thread pool for each tile. The thread pool manages the execution of these tasks, allowing multiple tiles to be rendered in parallel, which can significantly improve rendering performance for large images.
         */
        void render() {
            std::atomic<int> completedTiles(0);
            int totalTiles = static_cast<int>(_tiles.size());
            std::mutex printMutex;

            {
                RayTracer::PoolThread poolThread;
                for (RayTracer::Tile &tile : _tiles) {
                    poolThread.addInQueue([&tile, &completedTiles, totalTiles, &printMutex](){
                        tile.render();
                        int completed = completedTiles.fetch_add(1) + 1;
                        std::lock_guard<std::mutex> lock(printMutex);
                        std::cerr << "\r[";
                        int pos = 50 * completed / totalTiles;
                        for (int i = 0; i < 50; ++i) {
                            if (i < pos) std::cerr << "=";
                            else if (i == pos) std::cerr << ">";
                            else std::cerr << " ";
                        }
                        std::cerr << "] " << (completed * 100 / totalTiles) << "% (" << completed << "/" << totalTiles << ")" << std::flush;
                    });
                }
            }
            std::cerr << std::endl;
        }

        /** @brief Display the final rendered image by combining results from all tiles
         * This method displays the final rendered image by iterating through each pixel in the overall image and retrieving the corresponding pixel color from the appropriate tile. It calculates which tile contains each pixel based on the pixel's coordinates and then calls the display method of the Pixel class to output the color of each pixel in a format suitable for PPM image output (R G B).
         */
        std::string display() {
            std::string img;
            int xTile = (_baseContext.width + 31) / 32;
            for (int y = 0; y < _baseContext.height; y++) {
                for (int x = 0; x < _baseContext.width; x++) {
                    int tileId = (x / 32) + ((y / 32) * xTile);
                    int pixelId = (x % _tiles[tileId]._context.tileFormat.xSize) + (y % _tiles[tileId]._context.tileFormat.ySize) * _tiles[tileId]._context.tileFormat.xSize;
                    img += _tiles[tileId]._context.pixels[pixelId].display();
                }
            }
            return img;
        }

};
}

#endif