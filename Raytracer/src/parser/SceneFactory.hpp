/*
** EPITECH PROJECT, 2026
** Raytracer
** File description:
** ConfigParser
*/

#ifndef SCENEFACTORY
#define SCENEFACTORY

#include <string>
#include <vector>
#include <memory>
#include <exception>
#include <libconfig.h++>
#include <map>
#include <dirent.h>
#include <iostream>

#include "Camera.hpp"
#include "IPrimitive.hpp"
#include "ILight.hpp"
#include "IMaterial.hpp"
#include "ITransform.hpp"
#include "Point3D.hpp"
#include "Vector3D.hpp"
#include "DlLoader.hpp"

class SceneFactory {
public:
    SceneFactory() {
        fillPluginsFiles();
        loadPlugins();
    }

    ~SceneFactory() = default;

    class Exception : public std::exception {
        public:
            Exception(const std::string &msg) : _msg(msg) {}

            const char *what() const noexcept override {
                return _msg.c_str();
            }

        private:
            std::string _msg;
    };

    bool checkValidPrimitiveType(std::string type) {
        if (_pluginsPrimitives.find(type) != _pluginsPrimitives.end())
            return true;
        return false;
    }

    bool checkValidMaterialType(std::string type) {
        if (_pluginsMaterials.find(type) != _pluginsMaterials.end())
            return true;
        return false;
    }

    bool checkValidTransformType(std::string type) {
        if (_pluginsTransforms.find(type) != _pluginsTransforms.end())
            return true;
        return false;
    }

    bool checkValidLightType(std::string type) {
        if (_pluginsLights.find(type) != _pluginsLights.end())
            return true;
        return false;
    }

    /** @brief Create a primitive from a plugin, returning ownership via unique_ptr
     * @param type The type name of the primitive plugin
     * @param primitiveSetting The configuration setting for the primitive
     * @return A unique_ptr owning the created primitive
     */
    std::unique_ptr<RayTracer::IPrimitive> createPrimitive(std::string type, const libconfig::Setting &primitiveSetting) {
        auto &entry = _pluginsPrimitives[type];
        RayTracer::IPrimitive *primitive = entry.first(primitiveSetting);
        if (!primitive)
            throw Exception("failed to create primitive of type '" + type + "'");
        return std::unique_ptr<RayTracer::IPrimitive>(primitive);
    }

    /** @brief Create a material from a plugin, returning ownership via unique_ptr
     * @param type The type name of the material plugin
     * @param materialSetting The configuration setting for the material
     * @return A unique_ptr owning the created material
     */
    std::unique_ptr<RayTracer::IMaterial> createMaterial(std::string type, const libconfig::Setting &materialSetting) {
        auto &entry = _pluginsMaterials[type];
        RayTracer::IMaterial *material = entry.first(materialSetting);
        if (!material)
            throw Exception("failed to create material of type '" + type + "'");
        return std::unique_ptr<RayTracer::IMaterial>(material);
    }

    /** @brief Create a transform from a plugin, returning ownership via unique_ptr
     * @param type The type name of the transform plugin
     * @param transformSetting The configuration setting for the transform
     * @return A unique_ptr owning the created transform
     */
    std::unique_ptr<RayTracer::ITransform> createTransform(std::string type, const libconfig::Setting &transformSetting) {
        auto &entry = _pluginsTransforms[type];
        RayTracer::ITransform *transform = entry.first(transformSetting);
        if (!transform)
            throw Exception("failed to create transform of type '" + type + "'");
        return std::unique_ptr<RayTracer::ITransform>(transform);
    }

    /** @brief Create a light from a plugin, returning ownership via unique_ptr
     * @param type The type name of the light plugin
     * @param lightSetting The configuration setting for the light
     * @return A unique_ptr owning the created light
     */
    std::unique_ptr<RayTracer::ILight> createLight(std::string type, const libconfig::Setting &lightSetting) {
        auto &entry = _pluginsLights[type];
        RayTracer::ILight *light = entry.first(lightSetting);
        if (!light)
            throw Exception("failed to create light of type '" + type + "'");
        return std::unique_ptr<RayTracer::ILight>(light);
    }

    /** @brief Create a renderer from a plugin, returning ownership via unique_ptr
     * @param type The type name of the renderer plugin
     * @param rendererSetting The configuration setting for the renderer
     * @return A unique_ptr owning the created renderer
     */
    std::unique_ptr<RayTracer::IRendering> createRenderer(std::string type, const libconfig::Setting &rendererSetting) {
        auto &entry = _pluginsRendering[type];
        RayTracer::IRendering *renderer = entry.first(rendererSetting);
        if (!renderer)
            throw Exception("failed to create renderer of type '" + type + "'");
        return std::unique_ptr<RayTracer::IRendering>(renderer);
    }

private:
    std::vector<std::string> _pluginsFiles;
    std::map<std::string, std::pair<RayTracer::IPrimitive *(*)(const libconfig::Setting &setting), void (*)(RayTracer::IPrimitive *)>> _pluginsPrimitives;
    std::map<std::string, std::pair<RayTracer::ILight *(*)(const libconfig::Setting &setting), void (*)(RayTracer::ILight *)>> _pluginsLights;
    std::map<std::string, std::pair<RayTracer::IMaterial *(*)(const libconfig::Setting &setting), void (*)(RayTracer::IMaterial *)>> _pluginsMaterials;
    std::map<std::string, std::pair<RayTracer::ITransform *(*)(const libconfig::Setting &setting), void (*)(RayTracer::ITransform *)>> _pluginsTransforms;
    std::map<std::string, std::pair<RayTracer::IRendering *(*)(const libconfig::Setting &setting), void (*)(RayTracer::IRendering *)>> _pluginsRendering;

    void fillPluginsFiles() {
        DIR *dir = opendir("plugins");
        if (!dir) return;
        struct dirent *ent;
        while ((ent = readdir(dir)) != NULL) {
            std::string name = ent->d_name;
            if (name.find(".so") != std::string::npos) {
                _pluginsFiles.push_back("plugins/" + name);
            }
        }
        closedir(dir);
    }

    void loadPlugins() {
        for (const std::string &file : _pluginsFiles) {
            try {
                DlLoader<void *> loader(file);
                if (loader.hasFunction("createPrimitive")) {
                    std::string name = loader.getFunction<char *(*)()>("getName")();
                    _pluginsPrimitives[name] = {
                        loader.getFunction<RayTracer::IPrimitive *(*)(const libconfig::Setting &setting)>("createPrimitive"),
                        loader.getFunction<void (*)(RayTracer::IPrimitive *)>("destroyPrimitive")
                    };
                } else if (loader.hasFunction("createLight")) {
                    std::string name = loader.getFunction<char *(*)()>("getName")();
                    _pluginsLights[name] = {
                        loader.getFunction<RayTracer::ILight *(*)(const libconfig::Setting &setting)>("createLight"),
                        loader.getFunction<void (*)(RayTracer::ILight *)>("destroyLight")
                    };
                } else if (loader.hasFunction("createMaterial")) {
                    std::string name = loader.getFunction<char *(*)()>("getName")();
                    _pluginsMaterials[name] = {
                        loader.getFunction<RayTracer::IMaterial *(*)(const libconfig::Setting &setting)>("createMaterial"),
                        loader.getFunction<void (*)(RayTracer::IMaterial *)>("destroyMaterial")
                    };
                } else if (loader.hasFunction("createTransform")) {
                    std::string name = loader.getFunction<char *(*)()>("getName")();
                    _pluginsTransforms[name] = {
                        loader.getFunction<RayTracer::ITransform *(*)(const libconfig::Setting &setting)>("createTransform"),
                        loader.getFunction<void (*)(RayTracer::ITransform *)>("destroyTransform")
                    };
                } else if (loader.hasFunction("createRendering")) {
                    std::string name = loader.getFunction<char *(*)()>("getName")();
                    _pluginsRendering[name] = {
                        loader.getFunction<RayTracer::IRendering *(*)(const libconfig::Setting &setting)>("createRendering"),
                        loader.getFunction<void (*)(RayTracer::IRendering *)>("destroyRendering")
                    };
                } else {
                    std::cerr << "Warning: Plugin '" << file << "' does not have a valid entry point" << std::endl;
                }
            } catch (const std::exception &e) {
                std::cerr << "Failed to load plugin '" << file << "': " << e.what() << std::endl;
            }
        }
    }
};

#endif /* SCENEFACTORY */
