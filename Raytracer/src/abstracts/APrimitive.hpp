/*
** EPITECH PROJECT, 2026
** APrimitive
** File description:
** APrimitive def
*/

#ifndef APRIMITIVE
    #define APRIMITIVE

    #include "Point3D.hpp"
    #include "IMaterial.hpp"
    #include "ColorMaterial.hpp"
    #include "IPrimitive.hpp"
    #include <memory>

namespace RayTracer {
/** @class APrimitive
 * @brief An abstract class for primitives
 * This class serves as a base for different types of primitives in the ray tracer. It implements the IPrimitive interface and provides common attributes and methods for managing the center point and material of the primitive. Derived classes can extend this base class to implement specific types of primitives (e.g., spheres, planes, boxes) while reusing the common functionality provided by APrimitive.
 * @see IPrimitive
 */
class APrimitive : public RayTracer::IPrimitive {
    protected:
        Math::Point3D _center;
        std::unique_ptr<RayTracer::IMaterial> _material;

    public:
        /** @brief Construct a primitive with default center and no material
         * This constructor initializes an APrimitive object with its center at the default point (0, 0, 0) in 3D space and a null material. It is useful for creating a primitive without specifying initial parameters, allowing the center and material to be set later as needed.
         */
        APrimitive(Math::Point3D center) : _center(center), _material(nullptr) {}
        virtual ~APrimitive() = default;

        APrimitive(const APrimitive &) = delete;
        APrimitive &operator=(const APrimitive &) = delete;
        APrimitive(APrimitive &&) = default;
        APrimitive &operator=(APrimitive &&) = default;

        /** @brief Get the center point of the primitive
         * @return A point representing the center of the primitive in 3D space
         */
        Math::Point3D getCenter() const override { return _center; }
        
        /** @brief Get the material of the primitive (non-owning)
         * @return A raw pointer to the material of the primitive, or nullptr if there is no material
         */
        RayTracer::IMaterial *getMaterial() const override { return _material.get(); }

        /** @brief Set the material of the primitive (takes ownership)
         * @param material A unique_ptr to the material to set
         */
        void setMaterial(std::unique_ptr<RayTracer::IMaterial> material) override { _material = std::move(material); }
        /** @brief Set the center point of the primitive
         * @param center The new center point of the primitive
         */
        void setCenter(Math::Point3D center) override { _center = center; }
};
}

#endif