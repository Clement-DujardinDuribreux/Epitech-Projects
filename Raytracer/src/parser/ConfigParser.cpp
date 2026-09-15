/*
** EPITECH PROJECT, 2024
** Raytracer
** File description:
** ConfigParser
*/

#include "ConfigParser.hpp"
#include "DlLoader.hpp"
#include "CommonParser.hpp"
#include "BaseRendering.hpp"

#include "ColorMaterial.hpp"

#include <cmath>
#include <dirent.h>

ConfigParser::ConfigParser(const std::string &filePath) : _filePath(filePath) {
    _factory = SceneFactory();
}

/** @brief Construct a ConfigParser::Exception object
 * @param msg The error message
 */
ConfigParser::Exception::Exception(const std::string &msg) : _msg(msg) {}

/** @brief Get the error message from a ConfigParser::Exception
 * @return A pointer to the null-terminated string representing the error message
 */
const char *ConfigParser::Exception::what() const noexcept {
    return _msg.c_str();
}

/** @brief Parse the configuration file
 * This method reads the configuration file and parses its contents into the appropriate data structures.
 */
void ConfigParser::parse() {
    libconfig::Config config;

    _objects.clear();
    _lights.clear();

    try {
        config.readFile(_filePath.c_str());
    } catch (const libconfig::FileIOException &) {
        throw Exception("ConfigParser: could not read file '" + _filePath + "'");
    } catch (const libconfig::ParseException &e) {
        throw Exception(
            "ConfigParser: parse error at " + std::string(e.getFile()) + ":" +
            std::to_string(e.getLine()) + " - " + e.getError());
    }

    try {
        const libconfig::Setting &root = config.getRoot();
        if (!root.exists("camera"))
            throw Exception("ConfigParser: missing 'camera' section");
        parseCamera(root.lookup("camera"));
        if (!root.exists("primitives"))
            throw Exception("ConfigParser: missing 'primitives' section");
        parsePrimitives(root.lookup("primitives"));
        if (!root.exists("lights"))
            throw Exception("ConfigParser: missing 'lights' section");
        parseLights(root.lookup("lights"));
    } catch (const libconfig::SettingNotFoundException &e) {
        throw Exception("ConfigParser: missing setting '" + std::string(e.getPath()) + "'");
    } catch (const libconfig::SettingTypeException &e) {
        throw Exception("ConfigParser: wrong type for setting '" + std::string(e.getPath()) + "'");
    } catch (const std::exception &e) {
        throw Exception("ConfigParser error: " + std::string(e.what()));
    }
}

/** @brief Get the camera configuration
 * @return A reference to the Camera object representing the camera configuration
 */
const RayTracer::Camera &ConfigParser::getCamera() const {
    return _camera;
}

/** @brief Get the list of lights
 * @return A vector of non-owning pointers to ILight objects representing the lights in the scene
 */
std::vector<RayTracer::ILight *> ConfigParser::getLights() const {
    std::vector<RayTracer::ILight *> lights;

    for (const auto &light : _lights)
        lights.push_back(light.get());
    return lights;
}

/** @brief Get the list of primitives
 * @return A vector of non-owning pointers to IPrimitive objects representing the primitives in the scene
 */
std::vector<RayTracer::IPrimitive *> ConfigParser::getObjects() const {
    std::vector<RayTracer::IPrimitive *> objects;

    for (const auto &object : _objects)
        objects.push_back(object.get());
    return objects;
}

/** @brief Get the width of the camera resolution
 * @return The width of the camera resolution
 */
int ConfigParser::getWidth() const {
    return _width;
}

/** @brief Get the height of the camera resolution
 * @return The height of the camera resolution
 */
int ConfigParser::getHeight() const {
    return _height;
}

/** @brief Get the ambient lighting intensity
 * @return The ambient lighting intensity
 */
double ConfigParser::getAmbient() const {
    return _ambient;
}

/** @brief Get the diffuse lighting intensity
 * @return The diffuse lighting intensity
 */
double ConfigParser::getDiffuse() const {
    return _diffuse;
}

/** @brief Parse the camera configuration
 * @param cameraSetting The configuration setting containing the camera data
 */
void ConfigParser::parseCamera(const libconfig::Setting &cameraSetting) {
    if (cameraSetting.exists("renderer")) {
        if (cameraSetting.lookup("renderer").getType() == libconfig::Setting::TypeString)
            _renderer = _factory.createRenderer(cameraSetting.lookup("renderer"), cameraSetting.lookup("renderer"));
        else
            throw Exception("ConfigParser: unknown renderer '" + std::string(cameraSetting.lookup("renderer")) + "'");
    } else
        _renderer = std::make_unique<RayTracer::BaseRendering>();
    if (cameraSetting.exists("resolution")) {
        const libconfig::Setting &resolution = cameraSetting.lookup("resolution");
        _width = resolution.exists("width") ? static_cast<int>(RayTracer::CommonParser::getNumber(resolution.lookup("width"))) : 1920;
        _height = resolution.exists("height") ? static_cast<int>(RayTracer::CommonParser::getNumber(resolution.lookup("height"))) : 1080;
    } else {
        _width = 1920;
        _height = 1080;
    }
    
    if (_width <= 0 || _height <= 0)
        throw Exception("ConfigParser: camera resolution must be > 0");

    const Math::Point3D position = cameraSetting.exists("position") ? RayTracer::CommonParser::parsePoint(cameraSetting.lookup("position")) : Math::Point3D(0, 0, 0);
    const Math::Vector3D rotation = cameraSetting.exists("rotation") ? RayTracer::CommonParser::parseVector(cameraSetting.lookup("rotation")) : Math::Vector3D(0, 0, 0);

    _fov = cameraSetting.exists("fieldOfView") ? RayTracer::CommonParser::getNumber(cameraSetting.lookup("fieldOfView")) : 90.0;
    if (_fov <= 0.0 || _fov >= 180.0)
        throw Exception("ConfigParser: fieldOfView must be in (0, 180)");

    const double theta = _fov * M_PI / 180.0;
    const double viewportHeight = 2.0 * std::tan(theta / 2.0);
    const double viewportWidth = viewportHeight * (static_cast<double>(_width) / _height);

    Math::Vector3D forward(0.0, 0.0, 1.0);
    Math::Vector3D horizontal(viewportWidth, 0.0, 0.0);
    Math::Vector3D vertical(0.0, viewportHeight, 0.0);

    Math::Vector3D rot(-rotation.getY(), rotation.getX(), rotation.getZ());
    forward = forward.rotateEuler(rot);
    horizontal = horizontal.rotateEuler(rot);
    vertical = vertical.rotateEuler(rot);

    const Math::Point3D screenOrigin(
        position.getX() + forward.getX() - horizontal.getX() / 2.0 - vertical.getX() / 2.0,
        position.getY() + forward.getY() - horizontal.getY() / 2.0 - vertical.getY() / 2.0,
        position.getZ() + forward.getZ() - horizontal.getZ() / 2.0 - vertical.getZ() / 2.0
    );

    _camera = RayTracer::Camera(position, RayTracer::Rectangle3D(screenOrigin, horizontal, vertical));
}

////////////////////////////////////////////////////////////////////////////

/** @brief Parse the primitive configurations
 * @param primitivesSetting The configuration setting containing the primitive data
 * This method iterates through the list of primitives defined in the configuration and creates the corresponding IPrimitive objects using the loaded plugins. It also sets the material and transforms for each primitive if specified in the configuration.
 */
void ConfigParser::parsePrimitives(const libconfig::Setting &primitivesSetting) {
    for (int i = 0; i < primitivesSetting.getLength(); i++) {
        const libconfig::Setting &primitiveList = primitivesSetting[i];
        std::string type = primitiveList.getName();
        if (!_factory.checkValidPrimitiveType(type))
            throw Exception("ConfigParser: unknown primitive type '" + type + "'");
        for (int j = 0; j < primitiveList.getLength(); j++) {
            const libconfig::Setting &primitiveSetting = primitiveList[j];
            auto primitive = _factory.createPrimitive(type, primitiveSetting);
            if (primitiveSetting.exists("material"))
                primitive->setMaterial(parseMaterials(primitiveSetting.lookup("material")));
            else
                primitive->setMaterial(std::make_unique<RayTracer::ColorMaterial>(BASE_COLOR, 128));
            if (primitiveSetting.exists("transforms"))
                parseTransforms(primitiveSetting.lookup("transforms"), primitive.get());
            _objects.push_back(std::move(primitive));
        }
    }
}

/** @brief Parse the material configurations
 * @param materialsSetting The configuration setting containing the material data
 * @return A unique_ptr to the IMaterial object representing the parsed material chain
 */
std::unique_ptr<RayTracer::IMaterial> ConfigParser::parseMaterials(const libconfig::Setting &materialsSetting) {
    std::unique_ptr<RayTracer::IMaterial> current = std::make_unique<RayTracer::ColorMaterial>(BASE_COLOR, 128);
    for (int i = 0; i < materialsSetting.getLength(); i++) {
        const libconfig::Setting &materialList = materialsSetting[i];
        std::string type = materialList.getName();
        if (!_factory.checkValidMaterialType(type))
            continue;
        auto material = _factory.createMaterial(type, materialsSetting);
        material->setWrapper(std::move(current));
        current = std::move(material);
    }
    return current;
}

/** @brief Parse the transform configurations
 * @param transformsSetting The configuration setting containing the transform data
 * @param primitive The primitive to which the transforms will be applied
 * This method iterates through the list of transforms defined in the configuration and applies each transform to the specified primitive using the loaded plugins. It checks for valid transform types and applies them in the order they are defined in the configuration.
 */
void ConfigParser::parseTransforms(const libconfig::Setting &transformsSetting, RayTracer::IPrimitive *primitive) {
    for (int j = 0; j < transformsSetting.getLength(); j++) {
        const libconfig::Setting &transformSetting = transformsSetting[j];
        std::string type = transformSetting.getName();
        if (!_factory.checkValidTransformType(type))
            throw Exception("ConfigParser: unknown transform type '" + type + "'");
        auto transform = _factory.createTransform(type, transformsSetting);
        transform->transform(primitive);
    }
}

/** @brief Parse the light configurations
 * @param lightsSetting The configuration setting containing the light data
 * This method iterates through the list of lights defined in the configuration and creates the corresponding ILight objects using the loaded plugins. It also sets the ambient and diffuse lighting intensities, as well as the ambient occlusion parameters if specified in the configuration.
 */
void ConfigParser::parseLights(const libconfig::Setting &lightsSetting) {
    _ambient = lightsSetting.exists("ambient") && lightsSetting.lookup("ambient").getType() != libconfig::Setting::TypeGroup && lightsSetting.lookup("ambient").getType() != libconfig::Setting::TypeList ? RayTracer::CommonParser::getNumber(lightsSetting.lookup("ambient")) : 0.0;
    _diffuse = lightsSetting.exists("diffuse") && lightsSetting.lookup("diffuse").getType() != libconfig::Setting::TypeGroup && lightsSetting.lookup("diffuse").getType() != libconfig::Setting::TypeList ? RayTracer::CommonParser::getNumber(lightsSetting.lookup("diffuse")) : 0.0;
    _AOsamples = lightsSetting.exists("AO") && lightsSetting.lookup("AO").exists("samples") ? static_cast<int>(RayTracer::CommonParser::getNumber(lightsSetting.lookup("AO").lookup("samples"))) : 0;
    _AOradius = lightsSetting.exists("AO") && lightsSetting.lookup("AO").exists("radius") ? RayTracer::CommonParser::getNumber(lightsSetting.lookup("AO").lookup("radius")) : 0.0;
    _AOintensity = lightsSetting.exists("AO") && lightsSetting.lookup("AO").exists("intensity") ? RayTracer::CommonParser::getNumber(lightsSetting.lookup("AO").lookup("intensity")) : 0.0;

    for (int i = 0; i < lightsSetting.getLength(); i++) {
        const libconfig::Setting &lightList = lightsSetting[i];
        std::string type = lightList.getName();
        if (type == "ambient" && lightList.getType() != libconfig::Setting::TypeGroup && lightList.getType() != libconfig::Setting::TypeList) continue;
        if (type == "diffuse" && lightList.getType() != libconfig::Setting::TypeGroup && lightList.getType() != libconfig::Setting::TypeList) continue;
        if (type == "AO") continue;

        if (!_factory.checkValidLightType(type))
            throw Exception("ConfigParser: unknown light type '" + type + "'");
        for (int j = 0; j < lightList.getLength(); j++) {
            const libconfig::Setting &lightSetting = lightList[j];
            _lights.push_back(_factory.createLight(type, lightSetting));
        }
    }
}
