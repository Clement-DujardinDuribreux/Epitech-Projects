#include "ITransform.hpp"
#include "RotationTransform.hpp"
#include "CommonParser.hpp"

#include <libconfig.h++>
#include <iostream>

extern "C" {

    const char *getName(void) {
        return "rotation";
    }

    RayTracer::ITransform *createTransform(const libconfig::Setting &setting) {
        if (!RayTracer::CommonParser::verifVector(setting, "rotation"))
            return nullptr;

        return new RayTracer::RotationTransform(
            RayTracer::CommonParser::parseVector(setting.lookup("rotation"))
        );
    }

    void destroyTransform(RayTracer::ITransform *transform) {
        delete transform;
    }

}