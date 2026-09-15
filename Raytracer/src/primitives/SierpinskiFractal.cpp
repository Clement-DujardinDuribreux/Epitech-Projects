/*
** EPITECH PROJECT, 2026
** SierpinskiFractal
** File description:
** SierpinskiFractal plugin
*/

#include "IPrimitive.hpp"
#include "SierpinskiFractal.hpp"
#include "CommonParser.hpp"

#include <libconfig.h++>
#include <cmath>
#include <iostream>

extern "C" {

    /** @brief Get the name of the Sierpinski fractal primitive
     * @return A pointer to the null-terminated string representing the name
     */
    const char *getName(void) {
        return "fractals";
    }

    /** @brief Create a Sierpinski fractal primitive from a configuration setting
     * @param setting The configuration setting for the Sierpinski fractal primitive
     * @return A pointer to the created Sierpinski fractal primitive, or nullptr if creation failed
     * This method verifies the required configuration settings for the vertices and depth of the fractal. It calculates the center point of the fractal based on the vertices and creates a new SierpinskiFractal object with the specified parameters.
     */
    RayTracer::IPrimitive *createPrimitive(const libconfig::Setting &setting) {
        if (!RayTracer::CommonParser::verifPoint(setting, "a") ||
            !RayTracer::CommonParser::verifPoint(setting, "b") ||
            !RayTracer::CommonParser::verifPoint(setting, "c") ||
            !RayTracer::CommonParser::verifPoint(setting, "d") ||
            !RayTracer::CommonParser::verifNumber(setting, "depth"))
            return nullptr;

        Math::Point3D a = RayTracer::CommonParser::parsePoint(setting.lookup("a"));
        Math::Point3D b = RayTracer::CommonParser::parsePoint(setting.lookup("b"));
        Math::Point3D c = RayTracer::CommonParser::parsePoint(setting.lookup("c"));
        Math::Point3D d = RayTracer::CommonParser::parsePoint(setting.lookup("d"));
        Math::Point3D e;
        int depth = static_cast<int>(RayTracer::CommonParser::getNumber(setting.lookup("depth")));

        if (setting.exists("e")) {
            if (!RayTracer::CommonParser::verifPoint(setting, "e"))
                return nullptr;
            e = RayTracer::CommonParser::parsePoint(setting.lookup("e"));
        } else {
            Math::Point3D base = c;
            double abX = b.getX() - a.getX();
            double abY = b.getY() - a.getY();
            double abZ = b.getZ() - a.getZ();
            double midX = (a.getX() + b.getX()) / 2.0;
            double midY = (a.getY() + b.getY()) / 2.0;
            double midZ = (a.getZ() + b.getZ()) / 2.0;
            double baseX = base.getX() - midX;
            double baseY = base.getY() - midY;
            double baseZ = base.getZ() - midZ;
            double abLengthSquared = abX * abX + abY * abY + abZ * abZ;
            double sideLength = std::sqrt(abLengthSquared);
            double projection = abLengthSquared == 0.0 ? 0.0 : (baseX * abX + baseY * abY + baseZ * abZ) / abLengthSquared;
            double dirX = baseX - abX * projection;
            double dirY = baseY - abY * projection;
            double dirZ = baseZ - abZ * projection;
            double dirLength = std::sqrt(dirX * dirX + dirY * dirY + dirZ * dirZ);

            if (dirLength < 0.000001) {
                dirX = -abY;
                dirY = abX;
                dirZ = 0.0;
                dirLength = std::sqrt(dirX * dirX + dirY * dirY + dirZ * dirZ);
            }
            if (dirLength < 0.000001)
                return nullptr;
            dirX = dirX / dirLength * sideLength;
            dirY = dirY / dirLength * sideLength;
            dirZ = dirZ / dirLength * sideLength;
            e = d;
            d = Math::Point3D(
                a.getX() + dirX,
                a.getY() + dirY,
                a.getZ() + dirZ
            );
            c = Math::Point3D(
                b.getX() + dirX,
                b.getY() + dirY,
                b.getZ() + dirZ
            );
        }

        return new RayTracer::SierpinskiFractal(a, b, c, d, e, depth);
    }

    /** @brief Destroy a Sierpinski fractal primitive
     * @param primitive A pointer to the Sierpinski fractal primitive to be destroyed
     */
    void destroyPrimitive(RayTracer::IPrimitive *primitive) {
        delete primitive;
    }

}
