#include "IMaterial.hpp"
#include "ColorMaterial.hpp"
#include "CommonParser.hpp"

#include <libconfig.h++>
#include <iostream>

extern "C" {

    /** @brief Get the name of the color material
     * @return A pointer to the null-terminated string representing the name
     */
    const char *getName(void) {
        return "color";
    }

    /** @brief Create a color material from the given configuration setting
     * @param setting The libconfig setting containing the color parameters
     * @return A pointer to the created IMaterial object, or nullptr if creation failed
     */
    RayTracer::IMaterial *createMaterial(const libconfig::Setting &setting) {
        int shininess = 128;
        if (!RayTracer::CommonParser::verifColor(setting, "color"))
            return nullptr;

        if (RayTracer::CommonParser::verifNumber(setting, "shininess"))
            shininess = RayTracer::CommonParser::getNumber(setting.lookup("shininess"));

        return new RayTracer::ColorMaterial(RayTracer::CommonParser::parseColor(setting.lookup("color")), shininess);
    }

    /** @brief Destroy a color material
     * @param material A pointer to the IMaterial object to be destroyed
     */
    void destroyMaterial(RayTracer::IMaterial *material) {
        delete material;
    }

}