#include "IPrimitive.hpp"
#include "Torus.hpp"
#include "CommonParser.hpp"

#include <libconfig.h++>
#include <iostream>

extern "C" {

    /** @brief Get the name of the torus primitive
     * @return A pointer to the null-terminated string representing the name
     */
    const char *getName(void) {
        return "torus";
    }

    /** @brief Create a torus primitive from a configuration setting
     * @param setting The configuration setting for the torus primitive
     * @return A pointer to the created torus primitive, or nullptr if creation failed
     * This method verifies the required configuration settings for the center point, major radius, minor radius, and axis of the torus. It creates a new Torus object with the specified parameters if the settings are valid.
     */
    RayTracer::IPrimitive *createPrimitive(const libconfig::Setting &setting) {
        if (!RayTracer::CommonParser::verifPoint(setting, "center") || !RayTracer::CommonParser::verifNumber(setting, "radius") || !RayTracer::CommonParser::verifNumber(setting, "height") || !RayTracer::CommonParser::verifVector(setting, "axis"))
            return nullptr;

        return new RayTracer::Torus(
            RayTracer::CommonParser::parsePoint(setting.lookup("center")),
            RayTracer::CommonParser::getNumber(setting.lookup("radius")),
            RayTracer::CommonParser::getNumber(setting.lookup("height")),
            RayTracer::CommonParser::parseVector(setting.lookup("axis"))
        );
    }

    /** @brief Destroy a torus primitive
     * @param primitive A pointer to the torus primitive to be destroyed
     */
    void destroyPrimitive(RayTracer::IPrimitive *primitive) {
        delete primitive;
    }
}