#include "IPrimitive.hpp"
#include "Cylinder.hpp"
#include "CommonParser.hpp"

#include <libconfig.h++>
#include <iostream>

extern "C" {

    /** @brief Get the name of the cylinder primitive
     * @return A pointer to the null-terminated string representing the name
     */
    const char *getName(void) {
        return "cylinders";
    }

    /** @brief Create a cylinder primitive from a configuration setting
     * @param setting The configuration setting for the cylinder primitive
     * @return A pointer to the created cylinder primitive, or nullptr if creation failed
     */
    RayTracer::IPrimitive *createPrimitive(const libconfig::Setting &setting) {
        if (!RayTracer::CommonParser::verifPoint(setting, "center") || !RayTracer::CommonParser::verifNumber(setting, "radius") || !RayTracer::CommonParser::verifNumber(setting, "height") || !RayTracer::CommonParser::verifVector(setting, "axis"))
            return nullptr;

        return new RayTracer::Cylinder(
            RayTracer::CommonParser::parsePoint(setting.lookup("center")),
            RayTracer::CommonParser::getNumber(setting.lookup("radius")),
            RayTracer::CommonParser::getNumber(setting.lookup("height")),
            RayTracer::CommonParser::parseVector(setting.lookup("axis"))
        );
    }

    /** @brief Destroy a cylinder primitive
     * @param primitive A pointer to the cylinder primitive to be destroyed
     */
    void destroyPrimitive(RayTracer::IPrimitive *primitive) {
        delete primitive;
    }

}
