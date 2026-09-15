/*
** EPITECH PROJECT, 2026
** LowRendering
** File description:
** LowRendering def
*/

#ifndef LOWRENDERING
    #define LOWRENDERING

    #include "IRendering.hpp"
    #include "ILight.hpp"
    #include "IPrimitive.hpp"

namespace RayTracer {

class LowRendering : public RayTracer::IRendering {
    private:

    public:
        LowRendering() = default;
        ~LowRendering() override = default;

        Math::Vector3D Render(RayTracer::RenderingContext context, [[maybe_unused]] RayTracer::IPrimitive *currentObj, std::vector<RayTracer::IPrimitive *> objs, std::vector<RayTracer::ILight *> lights, [[maybe_unused]] RayTracer::Ray r, Math::Point3D hit, Math::Vector3D baseColor, [[maybe_unused]] int shininess) const override {
            Math::Vector3D color(0, 0, 0);

            if (lights.empty())
                return baseColor;

            // Lights
            for (const RayTracer::ILight *light : lights) {
                Math::Vector3D lightDir = light->getLightDir(hit);
                RayTracer::Ray shadowRay(hit + (lightDir.getNormal() * 0.001), lightDir);
                bool inShadow = false;

                for (RayTracer::IPrimitive *otherObj : objs) {
                    Math::Point3D shadowHit = otherObj->hits(shadowRay);
                    if (shadowHit != Math::Point3D()) {
                        Math::Vector3D shadowDist(shadowHit.getX() - shadowRay.getOrigin().getX(), shadowHit.getY() - shadowRay.getOrigin().getY(), shadowHit.getZ() - shadowRay.getOrigin().getZ());
                        if (shadowDist.length() < lightDir.length()) {
                            inShadow = true;
                            break;
                        }
                    }
                }
                
                if (inShadow) {
                    color += (baseColor * context.ambient) / lights.size();
                } else {
                    color += baseColor / lights.size();
                }
            }
            return color;
        }
};
}

#endif