#include "IPrimitive.hpp"
#include "OBJ.hpp"
#include "CommonParser.hpp"

#include <libconfig.h++>

extern "C" {

    /** @brief Get the name of the OBJ primitive
     * @return A pointer to the null-terminated string representing the name
     */
    const char *getName(void) {
        return "objs";
    }

    /** @brief Create an OBJ primitive from a configuration setting
     * @param setting The configuration setting for the OBJ primitive
     * @return A pointer to the created OBJ primitive, or nullptr if creation failed
     */
    RayTracer::IPrimitive *createPrimitive(const libconfig::Setting &setting) {
        if (!setting.exists("path") || !RayTracer::CommonParser::verifPoint(setting, "center"))
            return nullptr;

        std::string path = setting.lookup("path");
        return new RayTracer::OBJ(
            path,
            RayTracer::CommonParser::parsePoint(setting.lookup("center"))
        );
    }

    /** @brief Destroy an OBJ primitive
     * @param primitive A pointer to the OBJ primitive to be destroyed
     */
    void destroyPrimitive(RayTracer::IPrimitive *primitive) {
        delete primitive;
    }

}
