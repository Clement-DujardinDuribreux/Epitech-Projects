#include "IPrimitive.hpp"
#include "Cone.hpp"
#include "CommonParser.hpp"

#include <libconfig.h++>
#include <iostream>

extern "C" {

    /** @brief Get the name of the cone primitive
     * @return A pointer to the null-terminated string representing the name
     */
    const char *getName(void) {
        return "cones";
    }

    /** @brief Create a cone primitive from a configuration setting
     * @param setting The configuration setting for the cone primitive
     * @return A pointer to the created cone primitive, or nullptr if creation failed
     */
    RayTracer::IPrimitive *createPrimitive(const libconfig::Setting &setting) {
        if (!RayTracer::CommonParser::verifPoint(setting, "center") || !RayTracer::CommonParser::verifNumber(setting, "radius") || !RayTracer::CommonParser::verifNumber(setting, "height") || !RayTracer::CommonParser::verifVector(setting, "axis"))
            return nullptr;

        return new RayTracer::Cone(
            RayTracer::CommonParser::parsePoint(setting.lookup("center")),
            RayTracer::CommonParser::getNumber(setting.lookup("radius")),
            RayTracer::CommonParser::getNumber(setting.lookup("height")),
            RayTracer::CommonParser::parseVector(setting.lookup("axis"))
        );
    }

    /** @brief Destroy a cone primitive
     * @param primitive A pointer to the cone primitive to be destroyed
     */
    void destroyPrimitive(RayTracer::IPrimitive *primitive) {
        delete primitive;
    }

}
