#include "LowRendering.hpp"
#include "IRendering.hpp"
#include "CommonParser.hpp"

#include <libconfig.h++>
#include <iostream>

extern "C" {

    const char *getName(void) {
        return "low-renderer";
    }

    RayTracer::IRendering *createRendering([[maybe_unused]] const libconfig::Setting &setting) {
        return new RayTracer::LowRendering();
    }

    void destroyRendering(RayTracer::IRendering *renderer) {
        delete renderer;
    }

}
