#include "IPrimitive.hpp"
#include "Pyramide.hpp"
#include "CommonParser.hpp"

#include <libconfig.h++>
#include <iostream>

extern "C" {

    /** @brief Get the name of the pyramide primitive
     * @return A pointer to the null-terminated string representing the name
     */
    const char *getName(void) {
        return "pyramide";
    }

    /** @brief Create a pyramide primitive from a configuration setting
     * @param setting The configuration setting for the pyramide primitive
     * @return A pointer to the created pyramide primitive, or nullptr if creation failed
     */
    RayTracer::IPrimitive *createPrimitive(const libconfig::Setting &setting) {
        if (!RayTracer::CommonParser::verifPoint(setting, "a") || !RayTracer::CommonParser::verifPoint(setting, "b") || !RayTracer::CommonParser::verifPoint(setting, "c") || !RayTracer::CommonParser::verifPoint(setting, "d") || !RayTracer::CommonParser::verifPoint(setting, "e") )
            return nullptr;

        return new RayTracer::Pyramide(
            RayTracer::CommonParser::parsePoint(setting.lookup("a")),
            RayTracer::CommonParser::parsePoint(setting.lookup("b")),
            RayTracer::CommonParser::parsePoint(setting.lookup("c")),
            RayTracer::CommonParser::parsePoint(setting.lookup("d")),
            RayTracer::CommonParser::parsePoint(setting.lookup("e"))
        );
    }

    /** @brief Destroy a pyramide primitive
     * @param primitive A pointer to the pyramide primitive to be destroyed
     */
    void destroyPrimitive(RayTracer::IPrimitive *primitive) {
        delete primitive;
    }

}
