#include "IMaterial.hpp"
#include "TransparencyMaterial.hpp"
#include "CommonParser.hpp"

#include <libconfig.h++>
#include <iostream>

extern "C" {

    /** @brief Get the name of the transparency material
     * @return A pointer to the null-terminated string representing the name
     */
    const char *getName(void) {
        return "transparency";
    }

    /** @brief Create a transparency material from the given configuration setting
     * @param setting The libconfig setting containing the transparency parameters
     * @return A pointer to the created IMaterial object, or nullptr if creation failed
     */
    RayTracer::IMaterial *createMaterial(const libconfig::Setting &setting) {
        if (!RayTracer::CommonParser::verifNumber(setting, "transparency"))
            return nullptr;

        return new RayTracer::TransparencyMaterial(
            RayTracer::CommonParser::getNumber(setting.lookup("transparency"))
        );
    }

    /** @brief Destroy a transparency material
     * @param material A pointer to the IMaterial object to be destroyed
     */
    void destroyMaterial(RayTracer::IMaterial *material) {
        delete material;
    }

}