/*
** EPITECH PROJECT, 2026
** Raytracer
** File description:
** Raytracer main
*/

#include "Ray.hpp"
#include "TileMap.hpp"
#include "ConfigParser.hpp"
#include <iostream>
#include <vector>
#include <fstream>

/**
 * @brief Main function for the raytracer application
 * @param argc Number of command line arguments
 * @param argv Array of command line arguments
 * @return 0 on success, 84 on failure
 */
int main(int argc, char **argv) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <scene.cfg>" << std::endl;
        return 84;
    }

    std::ofstream file("out.ppm");
    if (!file.is_open()) {
        std::cerr << "Can't open file 'out.ppm'" << std::endl;
        return 84;
    }

    try {
        ConfigParser parser(argv[1]);
        parser.parse();
        
        RayTracer::Tile::renderContext context;
        context.ambient = parser.getAmbient();
        context.camera = parser.getCamera();
        context.lights = parser.getLights();
        context.objs = parser.getObjects();
        context.diffuseMultiplier = parser.getDiffuse();
        context.width = parser.getWidth();
        context.height = parser.getHeight();
        context.AOsamples = parser.getAOsamples();
        context.AOradius = parser.getAOradius();
        context.AOintensity = parser.getAOintensity();
        context.renderer = parser.getRendering();
        file << "P3\n" << context.width << " " << context.height << "\n" << "255" << "\n";
        RayTracer::TileMap tileMap(context);
        tileMap.render();     
        file << tileMap.display();
    } catch (const ConfigParser::Exception &e) {
        std::cerr << e.what() << std::endl;
        return 84;
    } catch (const std::exception &e) {
        std::cerr << "Unexpected error: " << e.what() << std::endl;
        return 84;
    }
    file.close();
    return 0;
}
