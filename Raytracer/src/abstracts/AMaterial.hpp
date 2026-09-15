/*
** EPITECH PROJECT, 2026
** AMaterial
** File description:
** AMaterial def
*/

#ifndef AMATERIAL
    #define AMATERIAL

    #include "IMaterial.hpp"
    #include <memory>

#define BASE_COLOR Math::Vector3D(255, 255, 255)

namespace RayTracer {
/** @class AMaterial
 * @brief An abstract class for materials
 * This class serves as a base for different types of materials in the ray tracer. It implements the IMaterial interface and provides a common implementation for managing a wrapper material. Derived classes can extend this base class to implement specific types of materials (e.g., diffuse, reflective, refractive) while reusing the common functionality provided by AMaterial.
 * @see IMaterial
 */
class AMaterial : public RayTracer::IMaterial {
    private:
        std::unique_ptr<RayTracer::IMaterial> _wrapper;

    protected:
        int _shininess;

    public:
        /** @brief Construct a material with no wrapper
         * This constructor initializes an AMaterial object with a null wrapper, indicating that it does not wrap any other material. It is useful for creating a base material that can be extended by derived classes without initially wrapping another material.
         */
        AMaterial() : _wrapper(nullptr), _shininess(128) {}
        AMaterial(int shininess) : _wrapper(nullptr), _shininess(shininess) {}
        virtual ~AMaterial() = default;

        AMaterial(const AMaterial &) = delete;
        AMaterial &operator=(const AMaterial &) = delete;
        AMaterial(AMaterial &&) = default;
        AMaterial &operator=(AMaterial &&) = default;
        
        /** @brief Set the wrapper material (takes ownership)
         * @param wrapper A unique_ptr to the material to set as the wrapper
         */
        void setWrapper(std::unique_ptr<RayTracer::IMaterial> wrapper) override { _wrapper = std::move(wrapper); }
        /** @brief Get the wrapper material (non-owning)
         * @return A raw pointer to the wrapper material, or nullptr if there is no wrapper
         */
        RayTracer::IMaterial *getWrapper() const override { return _wrapper.get(); }

        double getShininess() const override { return _shininess; }
};
}

#endif