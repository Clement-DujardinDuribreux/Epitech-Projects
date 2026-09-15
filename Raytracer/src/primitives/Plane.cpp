#include "IPrimitive.hpp"
#include "Plane.hpp"
#include "CommonParser.hpp"

#include <libconfig.h++>
#include <iostream>

extern "C" {

    /** @brief Get the name of the plane primitive
     * @return A pointer to the null-terminated string representing the name
     */
    const char *getName(void) {
        return "planes";
    }

    /** @brief Create a plane primitive from a configuration setting
     * @param setting The configuration setting for the plane primitive
     * @return A pointer to the created plane primitive, or nullptr if creation failed
     */
    RayTracer::IPrimitive *createPrimitive(const libconfig::Setting &setting) {
        if (!RayTracer::CommonParser::verifPoint(setting, "center") || !RayTracer::CommonParser::verifVector(setting, "normal") || !RayTracer::CommonParser::verifNumber(setting, "length") || !RayTracer::CommonParser::verifNumber(setting, "width"))
            return nullptr;

        return new RayTracer::Plane(
            RayTracer::CommonParser::parsePoint(setting.lookup("center")),
            RayTracer::CommonParser::getNumber(setting.lookup("length")),
            RayTracer::CommonParser::getNumber(setting.lookup("width")),
            RayTracer::CommonParser::parseVector(setting.lookup("normal"))
        );
    }

    /** @brief Destroy a plane primitive
     * @param primitive A pointer to the plane primitive to be destroyed
     */
    void destroyPrimitive(RayTracer::IPrimitive *primitive) {
        delete primitive;
    }

}