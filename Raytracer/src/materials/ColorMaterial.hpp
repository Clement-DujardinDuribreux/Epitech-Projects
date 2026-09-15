/*
** EPITECH PROJECT, 2026
** ColorMaterial
** File description:
** ColorMaterial def
*/

#ifndef COLORMATERIAL
    #define COLORMATERIAL

    #include "AMaterial.hpp"

namespace RayTracer {
/** @brief A material that represents a simple color
 * This class implements the AMaterial interface and represents a material that has a single color. It is used to define the base color of an object in the scene, which can then be modified by lighting and shading calculations during rendering.
 * The getRendering method returns the color of the material, while the getShadowColor and getShadowIntensity methods provide information for shadow calculations.
 * Example usage:
 * ColorMaterial redMaterial(Math::Vector3D(1.0, 0.0, 0.0)); // Create a red color material
 * This material can then be assigned to a primitive to give it a red color when rendered.
 * @see AMaterial
 */
class ColorMaterial : public RayTracer::AMaterial {
    private:
        Math::Vector3D _color;

    public:
        /** @brief Construct a color material with a default color
         * This constructor initializes a ColorMaterial object with a default color (0, 0, 0), which represents black. It is useful for creating a color material without specifying an initial color.
         */
        ColorMaterial() : _color(Math::Vector3D()) {}
        /** @brief Construct a color material with a specified color
         * @param color The color to be used for the material
         * This constructor initializes a ColorMaterial object with the provided color. It allows for creating a color material with any desired color by specifying the RGB values in the Math::Vector3D.
         */
        ColorMaterial(Math::Vector3D color, int shininess) : AMaterial(shininess), _color(color) {}
        ~ColorMaterial() override = default;

        /** @brief Get the rendering color of the material
         * @param currentObj The current primitive being rendered
         * @param objs The list of all primitives in the scene
         * @param lights The list of all lights in the scene
         * @param r The ray being traced
         * @param hit The point of intersection on the primitive
         * @param context The rendering context containing additional information for rendering
         * @return A vector representing the color of the material for rendering purposes
         */
        Math::Vector3D getRendering(
            [[maybe_unused]] RayTracer::IPrimitive *currentObj,
            [[maybe_unused]] std::vector<RayTracer::IPrimitive *> objs,
            [[maybe_unused]] std::vector<RayTracer::ILight *> lights,
            [[maybe_unused]] RayTracer::Ray r,
            [[maybe_unused]] Math::Point3D hit,
            [[maybe_unused]] RayTracer::RenderingContext context
        ) const override {
            return _color;
        }

        /** @brief Get the shadow color of the material
         * @return A vector representing the shadow color of the material
         */
        Math::Vector3D getShadowColor() const override {
            return Math::Vector3D(1, 1, 1);
        }

        /** @brief Get the shadow intensity of the material
         * @param ray The ray being traced for shadow calculation
         * @param hit The point of intersection on the primitive
         * @param currentObj The current primitive being rendered
         * @param objs The list of all primitives in the scene
         * @return A double value representing the intensity of the shadow (1 for no shadow, 0 for full shadow)
         */
        double getShadowIntensity([[maybe_unused]] RayTracer::Ray ray, [[maybe_unused]] Math::Point3D hit, [[maybe_unused]] RayTracer::IPrimitive *currentObj, [[maybe_unused]] std::vector<RayTracer::IPrimitive *> objs) const override {
            return 1;
        }
};
}

#endif