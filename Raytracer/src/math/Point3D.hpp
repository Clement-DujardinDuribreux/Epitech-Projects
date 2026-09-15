/*
** EPITECH PROJECT, 2026
** Point3D
** File description:
** Point3D def
*/

#ifndef POINT3D
    #define POINT3D

    #include "Vector3D.hpp"

namespace Math {
class Point3D {
    private:
        /* Attributs */
        double _x, _y, _z;

        /* Methods */

    protected:
        /* Attributs */

        /* Methods */

    public:
        /* Const / Destr */
        /** @brief Construct a point at the origin
         * This constructor initializes a Point3D object with default coordinates (0, 0, 0), representing the origin point in 3D space. It is useful for creating a point without specifying any initial coordinates.
         */
        Point3D() : _x(0), _y(0), _z(0) {}
        /** @brief Construct a point with specified coordinates
         * @param x The x coordinate of the point
         * @param y The y coordinate of the point
         * @param z The z coordinate of the point
         * This constructor initializes a Point3D object with the provided x, y, and z coordinates. It allows for creating a point at any location in 3D space by specifying its coordinates.
         */
        Point3D(double x, double y, double z) : _x(x), _y(y), _z(z) {}
        ~Point3D() = default;

        /* Getters */
        /** @brief Get the x coordinate of the point
         * @return The x coordinate of the point
         */
        double getX() const { return _x; }
        /** @brief Get the y coordinate of the point
         * @return The y coordinate of the point
         */
        double getY() const { return _y; }
        /** @brief Get the z coordinate of the point
         * @return The z coordinate of the point
         */
        double getZ() const { return _z; }

        /* Setters */

        /* Operator */

        /* Methods */

    double getDistance(const Point3D &other) const {
        double dx = _x - other.getX();
        double dy = _y - other.getY();
        double dz = _z - other.getZ();
        return std::sqrt(dx * dx + dy * dy + dz * dz);
    }

    /** @brief Add a vector to the point
     * @param obj The vector to add
     * @return A new point representing the result of the addition
     */
    Point3D operator+(const Vector3D &obj) { return Point3D(_x + obj.getX(), _y + obj.getY(), _z + obj.getZ()); }
    /** @brief Subtract a vector from the point
     * @param obj The vector to subtract
     * @return A new point representing the result of the subtraction
     */
    Point3D operator-(const Vector3D &obj) { return Point3D(_x - obj.getX(), _y - obj.getY(), _z - obj.getZ()); }
    /** @brief Check if two points are equal
     * @param obj The point to compare with
     * @return True if the points are equal, false otherwise
     */
    bool operator==(const Point3D &obj) { return _x != obj._x ? false : _y != obj._y ? false : _z != obj._z ? false : true; }
    /** @brief Check if two points are not equal
     * @param obj The point to compare with
     * @return True if the points are not equal, false otherwise
     */
    bool operator!=(const Point3D &obj) { return !(*this == obj); }
};
}

#endif