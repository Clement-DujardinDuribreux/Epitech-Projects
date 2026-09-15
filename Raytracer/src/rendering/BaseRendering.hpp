/*
** EPITECH PROJECT, 2026
** BaseRendering
** File description:
** BaseRendering def
*/

#ifndef BASERENDERING
    #define BASERENDERING

    #include "IRendering.hpp"

namespace RayTracer {

/** @brief A base implementation of the IRendering interface 
 * This class provides a basic rendering algorithm that calculates the color of a point on a primitive based on ambient lighting, 
 * diffuse lighting from light sources, and ambient occlusion. It iterates through all light sources and primitives to determine
 * the contribution of each light to the final color, taking into account shadows and occlusion. The Render method can be overridden 
 * by derived classes to implement more complex rendering techniques, such as specular highlights, reflections, or refractions. 
 * This base implementation serves as a starting point for building more advanced renderers in the raytracer application.
 * @see IRendering
*/
class BaseRendering : public RayTracer::IRendering {
    private:

    public:
        BaseRendering() = default;
        ~BaseRendering() override = default;

        Math::Vector3D Render(RayTracer::RenderingContext context, RayTracer::IPrimitive *currentObj, std::vector<RayTracer::IPrimitive *> objs, std::vector<RayTracer::ILight *> lights, RayTracer::Ray r, Math::Point3D hit, Math::Vector3D baseColor, int shininess) const override {
            Math::Vector3D color = baseColor * context.ambient;
            Math::Vector3D v = currentObj->getNormal(hit, r);

            // Ambient Occlusion
            for (int i = 0; i < context.AOsamples; i++) {
                double theta = static_cast<double>(rand()) / RAND_MAX * 2.0 * M_PI;
                double phi = acos(1.0 - 2.0 * static_cast<double>(rand()) / RAND_MAX);
                double x = sin(phi) * cos(theta);
                double y = sin(phi) * sin(theta);
                double z = cos(phi);
                Math::Vector3D aoDir(x, y, z);
                if (aoDir.dot(v) < 0)
                    aoDir = aoDir * -1.0;
                RayTracer::Ray aoRay(hit + (v.getNormal() * 0.001), aoDir.getNormal());
                if (context.AOradius > 0) {
                    for (RayTracer::IPrimitive *obj : objs) {
                        Math::Point3D aoHit = obj->hits(aoRay);
                        if (aoHit != Math::Point3D() && obj->getMaterial()->getShadowIntensity(aoRay, aoHit, obj, objs) > 0 && aoRay.getOrigin().getDistance(aoHit) < context.AOradius) {
                            color = color * (1.0 - (context.AOintensity * obj->getMaterial()->getShadowIntensity(aoRay, aoHit, obj, objs)) / context.AOsamples);
                            break;
                        }
                    }
                }
            }

            // Lights
            for (const RayTracer::ILight *light : lights) {
                double shadowInt = 0.0;
                //Math::Vector3D shadowCol(1, 1, 1);
                Math::Vector3D lightDir = light->getLightDir(hit);
                Math::Vector3D viewVector = Math::Vector3D(r.getOrigin().getX() - hit.getX(), r.getOrigin().getY() - hit.getY(), r.getOrigin().getZ()- hit.getZ()).getNormal();
                RayTracer::Ray shadowRay(hit + (lightDir.getNormal() * 0.001), lightDir);
                for (RayTracer::IPrimitive *otherObj : objs) {
                    Math::Point3D shadowHit = otherObj->hits(shadowRay);
                    if (shadowHit != Math::Point3D() || light->getLightingValue(hit) <= 0) {
                        Math::Vector3D shadowDist(shadowHit.getX() - shadowRay.getOrigin().getX(), shadowHit.getY() - shadowRay.getOrigin().getY(), shadowHit.getZ() - shadowRay.getOrigin().getZ());
                        if (shadowDist.length() < lightDir.length() && otherObj->getMaterial()->getShadowIntensity(shadowRay, shadowHit, otherObj, objs) > 0) {
                            shadowInt = otherObj->getMaterial()->getShadowIntensity(shadowRay, shadowHit, otherObj, objs);
                            //shadowCol = otherObj->getMaterial()->getShadowColor();
                            //color = color * (shadowCol * shadowInt + Math::Vector3D(1, 1, 1) * (1.0 - shadowInt));
                            break;
                        }
                    }
                }
                double diffuse = v.dot(lightDir.getNormal()) * light->getLightingValue(hit);
                if (diffuse > 0) {
                    Math::Vector3D lightColor = (baseColor + light->getColor()) * (baseColor + light->getColor()).getNormal();
                    color = color + (lightColor * (diffuse * context.diffuseMultiplier * (1.0 - shadowInt) / lights.size()));

                    // Phong Specular
                    Math::Vector3D L = lightDir.getNormal();
                    Math::Vector3D N = v;
                    double dotNL = std::max(0.0, N.dot(L));
                    Math::Vector3D R = (N * 2.0 * dotNL) - L;
                    R = R.getNormal();
                    double specularAngle = std::max(0.0, R.dot(viewVector));
                    double specularFactor = std::pow(specularAngle, shininess);
                    Math::Vector3D specularContribution = light->getColor() * specularFactor * light->getLightingValue(hit);
                    color = color + (specularContribution * (1.0 - shadowInt));
                }
            }
            return color;
        }
};
}

#endif