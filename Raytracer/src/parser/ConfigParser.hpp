/*
** EPITECH PROJECT, 2026
** Raytracer
** File description:
** ConfigParser
*/

#ifndef CONFIGPARSER
#define CONFIGPARSER

#include <string>
#include <vector>
#include <memory>
#include <exception>
#include <libconfig.h++>
#include <map>

#include "Camera.hpp"
#include "IPrimitive.hpp"
#include "ILight.hpp"
#include "IMaterial.hpp"
#include "ITransform.hpp"
#include "Point3D.hpp"
#include "Vector3D.hpp"
#include "IRendering.hpp"

/** @class ConfigParser 
 * @brief A parser for reading configuration files and creating ray tracing objects
 * This class is responsible for parsing a configuration file that defines the camera, primitives, lights, materials, transforms, and rendering settings for a ray tracing scene. It loads plugins for custom primitives, lights, materials, transforms, and rendering techniques, and creates the corresponding objects based on the configuration. The parsed data can then be accessed through getter methods to be used in the ray tracing process.
 * The ConfigParser class uses the libconfig library to read and parse the configuration file, and it handles various exceptions that may occur during the parsing process, such as file I/O errors, parse errors, missing settings, and type errors. It also provides detailed error messages to help identify issues in the configuration file.
 * @see Camera, IPrimitive, ILight, IMaterial, ITransform, IRendering
 */
#include "SceneFactory.hpp"

class ConfigParser {
public:
    ConfigParser(const std::string &filePath);
    ~ConfigParser() = default;

    void parse();

    const RayTracer::Camera &getCamera() const;
    std::vector<RayTracer::ILight *> getLights() const;
    std::vector<RayTracer::IPrimitive *> getObjects() const;
    int getWidth() const;
    int getHeight() const;
    double getAmbient() const;
    double getDiffuse() const;
    /** @brief Get the number of samples for ambient occlusion
     * @return The number of samples for ambient occlusion
     */
    int getAOsamples() const { return _AOsamples; }
    /** @brief Get the radius for ambient occlusion
     * @return The radius for ambient occlusion
     */
    double getAOradius() const { return _AOradius; }
    /** @brief Get the intensity for ambient occlusion
     * @return The intensity for ambient occlusion
     */
    double getAOintensity() const { return _AOintensity; }
    /** @brief Get the rendering object (non-owning)
     * @return A raw pointer to the IRendering object
     */
    RayTracer::IRendering *getRendering() const { return _renderer.get(); }

    class Exception : public std::exception {
    public:
        Exception(const std::string &msg);
        const char *what() const noexcept override;

    private:
        std::string _msg;
    };

private:

    std::string _filePath;
    SceneFactory _factory;

    void parseCamera(const libconfig::Setting &cameraSetting);
    void parsePrimitives(const libconfig::Setting &primitivesSetting);
    std::unique_ptr<RayTracer::IMaterial> parseMaterials(const libconfig::Setting &materialsSetting);
    void parseTransforms(const libconfig::Setting &transformsSetting, RayTracer::IPrimitive *primitive);
    void parseLights(const libconfig::Setting &lightsSetting);

    RayTracer::Camera _camera;
    int _width = 0;
    int _height = 0;
    double _fov = 0.0;

    double _ambient = 0.0;
    double _diffuse = 0.0;

    int _AOsamples = 0;
    double _AOradius = 0.0;
    double _AOintensity = 0.0;

    std::vector<std::unique_ptr<RayTracer::IPrimitive>> _objects;
    std::vector<std::unique_ptr<RayTracer::ILight>> _lights;

    std::unique_ptr<RayTracer::IRendering> _renderer;
};

#endif /* CONFIGPARSER */
