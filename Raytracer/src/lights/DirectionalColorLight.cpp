#include "ILight.hpp"
#include "DirectionalColorLight.hpp"
#include "CommonParser.hpp"

#include <libconfig.h++>
#include <iostream>

extern "C" {

    /** @brief Get the name of the directional color light
     * @return A pointer to the null-terminated string representing the name
     */
    const char *getName(void) {
        return "directionalcolor";
    }

    /** @brief Create a directional color light from the given configuration setting
     * @param setting The libconfig setting containing the directional color light parameters
     * @return A pointer to the created ILight object, or nullptr if creation failed
     */
    RayTracer::ILight *createLight(const libconfig::Setting &setting) {
        if (!RayTracer::CommonParser::verifPoint(setting, "origin") || !RayTracer::CommonParser::verifVector(setting, "direction") || !RayTracer::CommonParser::verifNumber(setting, "radius")
            || !RayTracer::CommonParser::verifColor(setting, "color"))
            return nullptr;

        return new RayTracer::DirectionalColorLight(
            RayTracer::CommonParser::parsePoint(setting.lookup("origin")),
            RayTracer::CommonParser::parseVector(setting.lookup("direction")),
            RayTracer::CommonParser::getNumber(setting.lookup("radius")),
            RayTracer::CommonParser::parseColor(setting.lookup("color"))
        );
    }

    /** @brief Destroy a directional color light
     * @param light A pointer to the ILight object to be destroyed
     */
    void destroyLight(RayTracer::ILight *light) {
        delete light;
    }

}