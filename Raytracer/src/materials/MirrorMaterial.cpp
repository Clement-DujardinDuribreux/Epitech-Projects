#include "IMaterial.hpp"
#include "MirrorMaterial.hpp"
#include "CommonParser.hpp"

#include <libconfig.h++>
#include <iostream>
#include <cmath>

extern "C" {

    /** @brief Get the name of the mirror material
     * @return A pointer to the null-terminated string representing the name
     */
    const char *getName(void) {
        return "reflectivity";
    }

    /** @brief Create a mirror material from the given configuration setting
     * @param setting The libconfig setting containing the reflectivity parameters
     * @return A pointer to the created IMaterial object, or nullptr if creation failed
     */
    RayTracer::IMaterial *createMaterial(const libconfig::Setting &setting) {
        double reflectivity = 1.0;
        
        if (RayTracer::CommonParser::verifNumber(setting, "reflectivity")) {
            reflectivity = RayTracer::CommonParser::getNumber(setting.lookup("reflectivity"));
        }
        
        return new RayTracer::MirrorMaterial(reflectivity);
    }

    /** @brief Destroy a mirror material
     * @param material A pointer to the IMaterial object to be destroyed
     */
    void destroyMaterial(RayTracer::IMaterial *material) {
        delete material;
    }

}
