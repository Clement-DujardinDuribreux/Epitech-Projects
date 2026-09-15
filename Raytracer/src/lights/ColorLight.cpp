#include "ILight.hpp"
#include "ColorLight.hpp"
#include "CommonParser.hpp"

#include <libconfig.h++>
#include <iostream>

extern "C" {

    /** @brief Get the name of the color light
     * @return A pointer to the null-terminated string representing the name
     */
    const char *getName(void) {
        return "point-color";
    }

    /** @brief Create a color light from the given configuration setting
     * @param setting The libconfig setting containing the color light parameters
     * @return A pointer to the created ILight object, or nullptr if creation failed
     */
    RayTracer::ILight *createLight(const libconfig::Setting &setting) {
        if (!RayTracer::CommonParser::verifPoint(setting, "origin") || !RayTracer::CommonParser::verifColor(setting, "color"))
            return nullptr;

        return new RayTracer::ColorLight(
            RayTracer::CommonParser::parsePoint(setting.lookup("origin")),
            RayTracer::CommonParser::parseColor(setting.lookup("color"))
        );
    }

    /** @brief Destroy a color light
     * @param light A pointer to the ILight object to be destroyed
     */
    void destroyLight(RayTracer::ILight *light) {
        delete light;
    }

}