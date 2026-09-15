#include "IPrimitive.hpp"
#include "Sphere.hpp"
#include "CommonParser.hpp"

#include <libconfig.h++>
#include <iostream>

extern "C" {

    /** @brief Get the name of the sphere primitive
     * @return A pointer to the null-terminated string representing the name
     */
    const char *getName(void) {
        return "spheres";
    }

    /** @brief Create a sphere primitive from a configuration setting
     * @param setting The configuration setting for the sphere primitive
     * @return A pointer to the created sphere primitive, or nullptr if creation failed
     * This method verifies the required configuration settings for the center point and radius of the sphere. It creates a new Sphere object with the specified parameters if the settings are valid.
     */
    RayTracer::IPrimitive *createPrimitive(const libconfig::Setting &setting) {
        if (!RayTracer::CommonParser::verifPoint(setting, "center") || !RayTracer::CommonParser::verifNumber(setting, "radius"))
            return nullptr;

        return new RayTracer::Sphere(
            RayTracer::CommonParser::parsePoint(setting.lookup("center")),
            RayTracer::CommonParser::getNumber(setting.lookup("radius"))
        );
    }

    /** @brief Destroy a sphere primitive
     * @param primitive A pointer to the sphere primitive to be destroyed
     */
    void destroyPrimitive(RayTracer::IPrimitive *primitive) {
        delete primitive;
    }

}