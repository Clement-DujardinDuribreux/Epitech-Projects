#include "IPrimitive.hpp"
#include "Triangle.hpp"
#include "CommonParser.hpp"

#include <libconfig.h++>
#include <iostream>

extern "C" {

    /** @brief Get the name of the triangle primitive
     * @return A pointer to the null-terminated string representing the name
     */
    const char *getName(void) {
        return "triangles";
    }

    /** @brief Create a triangle primitive from a configuration setting
     * @param setting The configuration setting for the triangle primitive
     * @return A pointer to the created triangle primitive, or nullptr if creation failed
     * This method verifies the required configuration settings for the vertices of the triangle. It calculates the center point of the triangle based on the vertices and creates a new Triangle object with the specified parameters if the settings are valid.
     */
    RayTracer::IPrimitive *createPrimitive(const libconfig::Setting &setting) {
        if (!RayTracer::CommonParser::verifPoint(setting, "a") || !RayTracer::CommonParser::verifPoint(setting, "b") || !RayTracer::CommonParser::verifPoint(setting, "c"))
            return nullptr;

        return new RayTracer::Triangle(
            RayTracer::CommonParser::parsePoint(setting.lookup("a")),
            RayTracer::CommonParser::parsePoint(setting.lookup("b")),
            RayTracer::CommonParser::parsePoint(setting.lookup("c"))
        );
    }

    /** @brief Destroy a triangle primitive
     * @param primitive A pointer to the triangle primitive to be destroyed
     */
    void destroyPrimitive(RayTracer::IPrimitive *primitive) {
        delete primitive;
    }

}
