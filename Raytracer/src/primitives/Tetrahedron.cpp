#include "IPrimitive.hpp"
#include "Tetrahedron.hpp"
#include "CommonParser.hpp"

#include <libconfig.h++>
#include <iostream>

extern "C" {

    /** @brief Get the name of the tetrahedron primitive
     * @return A pointer to the null-terminated string representing the name
     */
    const char *getName(void) {
        return "tetrahedra";
    }

    /** @brief Create a tetrahedron primitive from a configuration setting
     * @param setting The configuration setting for the tetrahedron primitive
     * @return A pointer to the created tetrahedron primitive, or nullptr if creation failed
     * This method verifies the required configuration settings for the vertices of the tetrahedron. It calculates the center point of the tetrahedron based on the vertices and creates a new Tetrahedron object with the specified parameters if the settings are valid.
     */
    RayTracer::IPrimitive *createPrimitive(const libconfig::Setting &setting) {
        if (!RayTracer::CommonParser::verifPoint(setting, "a") || !RayTracer::CommonParser::verifPoint(setting, "b") || !RayTracer::CommonParser::verifPoint(setting, "c") || !RayTracer::CommonParser::verifPoint(setting, "d"))
            return nullptr;

        return new RayTracer::Tetrahedron(
            RayTracer::CommonParser::parsePoint(setting.lookup("a")),
            RayTracer::CommonParser::parsePoint(setting.lookup("b")),
            RayTracer::CommonParser::parsePoint(setting.lookup("c")),
            RayTracer::CommonParser::parsePoint(setting.lookup("d"))
        );
    }

    /** @brief Destroy a tetrahedron primitive
     * @param primitive A pointer to the tetrahedron primitive to be destroyed
     */
    void destroyPrimitive(RayTracer::IPrimitive *primitive) {
        delete primitive;
    }

}
