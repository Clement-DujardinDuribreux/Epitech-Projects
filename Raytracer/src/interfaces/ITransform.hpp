/*
** EPITECH PROJECT, 2026
** ITransform
** File description:
** ITransform def
*/

#ifndef ITRANSFORM
    #define ITRANSFORM

    #include "IPrimitive.hpp"

namespace RayTracer {
class ITransform {
    private:

    public:
        virtual ~ITransform() = default;

        /** @brief Apply the transformation to a primitive
         * @param primitive The primitive to transform
         * This method takes a pointer to a primitive and applies the transformation defined by the implementing class. The specific transformation behavior (e.g., translation, rotation, scaling) will depend on the implementation of this method in the derived classes. By calling this method, the properties of the primitive (such as its center point) can be modified according to the transformation logic.
         */
        virtual void transform(RayTracer::IPrimitive *primitive) = 0;
};
}

#endif