#include "IPrimitive.hpp"
#include "Box.hpp"
#include "CommonParser.hpp"

#include <libconfig.h++>
#include <iostream>

extern "C" {

    /** @brief Get the name of the box primitive
     * @return A pointer to the null-terminated string representing the name
     */
    const char *getName(void) {
        return "box";
    }

    /** @brief Create a box primitive from a configuration setting
     * @param setting The configuration setting for the box primitive
     * @return A pointer to the created box primitive, or nullptr if creation failed
     */
    RayTracer::IPrimitive *createPrimitive(const libconfig::Setting &setting) {
        if (!RayTracer::CommonParser::verifPoint(setting, "center") || !RayTracer::CommonParser::verifNumber(setting, "length") || !RayTracer::CommonParser::verifNumber(setting, "width") || !RayTracer::CommonParser::verifNumber(setting, "height") || !RayTracer::CommonParser::verifVector(setting, "normal"))
            return nullptr;

        return new RayTracer::Box(
            RayTracer::CommonParser::parsePoint(setting.lookup("center")),
            RayTracer::CommonParser::getNumber(setting.lookup("length")),
            RayTracer::CommonParser::getNumber(setting.lookup("width")),
            RayTracer::CommonParser::getNumber(setting.lookup("height")),
            RayTracer::CommonParser::parseVector(setting.lookup("normal"))
        );
    }

    /** @brief Destroy a box primitive
     * @param primitive A pointer to the box primitive to be destroyed
     */
    void destroyPrimitive(RayTracer::IPrimitive *primitive) {
        delete primitive;
    }

}