#include "IPrimitive.hpp"
#include "MobiusStrip.hpp"
#include "CommonParser.hpp"

#include <libconfig.h++>
#include <iostream>

extern "C" {

    /** @brief Get the name of the mobius strip primitive
     * @return A pointer to the null-terminated string representing the name
     */
    const char *getName(void) {
        return "mobiusstrip";
    }

    /** @brief Create a mobius strip primitive from a configuration setting
    * @param setting The configuration setting for the mobius strip primitive
    * @return A pointer to the created mobius strip primitive, or nullptr if creation failed
    */
    RayTracer::IPrimitive *createPrimitive(const libconfig::Setting &setting) {
        if (!RayTracer::CommonParser::verifPoint(setting, "center") || !RayTracer::CommonParser::verifNumber(setting, "radius") || !RayTracer::CommonParser::verifNumber(setting, "width") || !RayTracer::CommonParser::verifVector(setting, "axis"))
            return nullptr;

        return new RayTracer::MobiusStrip(
            RayTracer::CommonParser::parsePoint(setting.lookup("center")),
            RayTracer::CommonParser::getNumber(setting.lookup("radius")),
            RayTracer::CommonParser::getNumber(setting.lookup("width")),
            RayTracer::CommonParser::parseVector(setting.lookup("axis"))
        );
    }

    /** @brief Destroy a mobius strip primitive
     * @param primitive A pointer to the mobius strip primitive to be destroyed
     */
    void destroyPrimitive(RayTracer::IPrimitive *primitive) {
        delete primitive;
    }

}
