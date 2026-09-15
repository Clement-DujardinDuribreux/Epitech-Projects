#include "ILight.hpp"
#include "SpotLight.hpp"
#include "CommonParser.hpp"

#include <libconfig.h++>
#include <iostream>

extern "C" {

    /** @brief Get the name of the spotlight
     * @return A pointer to the null-terminated string representing the name
     */
    const char *getName(void) {
        return "spotlight";
    }

    /** @brief Create a spotlight from the given configuration setting
     * @param setting The libconfig setting containing the spotlight parameters
     * @return A pointer to the created ILight object, or nullptr if creation failed
     */
    RayTracer::ILight *createLight(const libconfig::Setting &setting) {
        if (!RayTracer::CommonParser::verifPoint(setting, "position") || !RayTracer::CommonParser::verifVector(setting, "direction") || !RayTracer::CommonParser::verifNumber(setting, "angle"))
            return nullptr;

        return new RayTracer::SpotLight(
            RayTracer::CommonParser::parsePoint(setting.lookup("position")),
            RayTracer::CommonParser::parseVector(setting.lookup("direction")),
            RayTracer::CommonParser::getNumber(setting.lookup("angle"))
        );
    }

    /** @brief Destroy a spotlight
     * @param light A pointer to the ILight object to be destroyed
     */
    void destroyLight(RayTracer::ILight *light) {
        delete light;
    }

}