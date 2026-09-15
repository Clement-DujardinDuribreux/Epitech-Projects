#include "ITransform.hpp"
#include "ScaleTransform.hpp"
#include "CommonParser.hpp"

#include <libconfig.h++>
#include <iostream>

extern "C" {

    const char *getName(void) {
        return "scaling";
    }

    RayTracer::ITransform *createTransform(const libconfig::Setting &setting) {
        if (!RayTracer::CommonParser::verifVector(setting, "scaling"))
            return nullptr;

        return new RayTracer::ScaleTransform(
            RayTracer::CommonParser::parseVector(setting.lookup("scaling"))
        );
    }

    void destroyTransform(RayTracer::ITransform *transform) {
        delete transform;
    }

}