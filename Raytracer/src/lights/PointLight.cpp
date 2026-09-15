#include "ILight.hpp"
#include "PointLight.hpp"
#include "CommonParser.hpp"

#include <libconfig.h++>
#include <iostream>

extern "C" {

    /** @brief Get the name of the point light
     * @return A pointer to the null-terminated string representing the name
     */
    const char *getName(void) {
        return "point";
    }

    /** @brief Create a point light from the given configuration setting
     * @param setting The libconfig setting containing the point light parameters
     * @return A pointer to the created ILight object, or nullptr if creation failed
     */
    RayTracer::ILight *createLight(const libconfig::Setting &setting) {
        if (!RayTracer::CommonParser::verifPoint(setting, "origin"))
            return nullptr;

        return new RayTracer::PointLight(
            RayTracer::CommonParser::parsePoint(setting.lookup("origin"))
        );
    }

    /** @brief Destroy a point light
     * @param light A pointer to the ILight object to be destroyed
     */
    void destroyLight(RayTracer::ILight *light) {
        delete light;
    }

}