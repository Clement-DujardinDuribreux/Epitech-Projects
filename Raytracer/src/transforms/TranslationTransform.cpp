#include "ITransform.hpp"
#include "TranslationTransform.hpp"
#include "CommonParser.hpp"

#include <libconfig.h++>
#include <iostream>

extern "C" {
    /** @brief Get the name of the translation transform
     * @return A pointer to the null-terminated string representing the name
     */
    const char *getName(void) {
        return "translation";
    }

    /** @brief Create a translation transform from the given configuration setting
     * @param setting The libconfig setting containing the translation parameters
     * @return A pointer to the created ITransform object, or nullptr if creation failed
     */
    RayTracer::ITransform *createTransform(const libconfig::Setting &setting) {
        if (!RayTracer::CommonParser::verifVector(setting, "translation"))
            return nullptr;

        return new RayTracer::TranslationTransform(
            RayTracer::CommonParser::parseVector(setting.lookup("translation"))
        );
    }

    /** @brief Destroy a translation transform
     * @param transform A pointer to the ITransform object to be destroyed
     */
    void destroyTransform(RayTracer::ITransform *transform) {
        delete transform;
    }

}