/*
** EPITECH PROJECT, 2026
** Vector
** File description:
** Vector def
*/

#ifndef VECTOR3D
    #define VECTOR3D

    #include <cmath>

namespace Math {
class Vector3D {
    private:
        /* Attributs */
        double _x, _y, _z;

        /* Methods */

    protected:
        /* Attributs */

        /* Methods */

    public:
        /* Const / Destr */
        /** @brief Construct a vector at the origin
         * This constructor initializes a Vector3D object with default coordinates (0, 0, 0), representing the zero vector in 3D space. It is useful for creating a vector without specifying any initial coordinates.
         */
        Vector3D() : _x(0), _y(0), _z(0) {}
        /** @brief Construct a vector with specified coordinates
         * @param x The x coordinate of the vector
         * @param y The y coordinate of the vector
         * @param z The z coordinate of the vector
         * This constructor initializes a Vector3D object with the provided x, y, and z coordinates. It allows for creating a vector at any location in 3D space by specifying its coordinates.
         */
        Vector3D(double x, double y, double z) : _x(x), _y(y), _z(z) {}
        ~Vector3D() = default;

        /* Getters */
        /** @brief Get the x coordinate of the vector
         * @return The x coordinate of the vector
         */
        double getX() const { return _x; }
        /** @brief Get the y coordinate of the vector
         * @return The y coordinate of the vector
         */
        double getY() const { return _y; }
        /** @brief Get the z coordinate of the vector
         * @return The z coordinate of the vector
         */
        double getZ() const { return _z; }

        /** @brief Get the normalized version of the vector
         * @return A new vector representing the normalized version of the current vector
         */
        Vector3D getNormal() const { return *this / this->length(); }

        /** @brief Get the length of the vector
         * @return The length of the vector
         */
        double length() const {
            return sqrt(dot(*this));
        }

        /* Setters */

        void setX(double x) {
            _x = x;
        }

        void setY(double y) {
            _y = y;
        }

        void setZ(double z) {
            _z = z;
        }

        /* Operator */

        /* Methods */
        /** @brief Calculate the dot product of this vector and another vector
         * @param v The other vector
         * @return The dot product of the two vectors
         */
        double dot(const Math::Vector3D &v) const {
            return _x * v.getX() + _y * v.getY() + _z * v.getZ();
        }

        /** @brief Calculate the cross product of this vector and another vector
         * @param v The other vector
         * @return The cross product of the two vectors
         */
        Vector3D cross(const Math::Vector3D &v) const {
            return Vector3D(
                _y * v.getZ() - _z * v.getY(),
                _z * v.getX() - _x * v.getZ(),
                _x * v.getY() - _y * v.getX()
            );
        }

    /** @brief Add two vectors
     * @param obj The other vector
     * @return A new vector representing the sum of the two vectors
     */
    Vector3D operator+(const Vector3D &obj) const { return Vector3D(_x + obj.getX(), _y + obj.getY(), _z + obj.getZ()); }
    /** @brief Subtract two vectors
     * @param obj The other vector
     * @return A new vector representing the difference of the two vectors
     */
    Vector3D operator-(const Vector3D &obj) const { return Vector3D(_x - obj.getX(), _y - obj.getY(), _z - obj.getZ()); }
    /** @brief Multiply two vectors
     * @param obj The other vector
     * @return A new vector representing the product of the two vectors
     */
    Vector3D operator*(const Vector3D &obj) const { return Vector3D(_x * obj.getX(), _y * obj.getY(), _z * obj.getZ()); }
    /** @brief Divide two vectors
     * @param obj The other vector
     * @return A new vector representing the quotient of the two vectors
     */
    Vector3D operator/(const Vector3D &obj) const { return Vector3D(_x / obj.getX(), _y / obj.getY(), _z / obj.getZ()); }

    /** @brief Add a vector to this vector
     * @param obj The other vector
     * @return A reference to the modified vector
     */
    Vector3D &operator+=(const Vector3D &obj) { _x += obj.getX(); _y += obj.getY(); _z += obj.getZ(); return *this; }
    /** @brief Subtract a vector from this vector
     * @param obj The other vector
     * @return A reference to the modified vector
     */
    Vector3D &operator-=(const Vector3D &obj) { _x -= obj.getX(); _y -= obj.getY(); _z -= obj.getZ(); return *this; }
    /** @brief Multiply this vector by another vector
     * @param obj The other vector
     * @return A reference to the modified vector
     */
    Vector3D &operator*=(const Vector3D &obj) { _x *= obj.getX(); _y *= obj.getY(); _z *= obj.getZ(); return *this; }
    /** @brief Divide this vector by another vector
     * @param obj The other vector
     * @return A reference to the modified vector
     */
    Vector3D &operator/=(const Vector3D &obj) { _x /= obj.getX(); _y /= obj.getY(); _z /= obj.getZ(); return *this; }

    /** @brief Multiply this vector by a scalar
     * @param n The scalar value
     * @return A new vector representing the result of the multiplication
     */
    Vector3D operator*(double n) const { return Vector3D(_x * n, _y * n, _z * n); }
    /** @brief Divide this vector by a scalar
     * @param n The scalar value
     * @return A new vector representing the result of the division
     */
    Vector3D operator/(double n) const { return Vector3D(_x / n, _y / n, _z / n); }
    /** @brief Multiply this vector by a scalar and assign the result to this vector
     * @param n The scalar value
     * @return A reference to the modified vector
     */
    Vector3D &operator*=(double n) { _x *= n; _y *= n; _z *= n; return *this; }
    /** @brief Divide this vector by a scalar and assign the result to this vector
     * @param n The scalar value
     * @return A reference to the modified vector
     */
    Vector3D &operator/=(double n) { _x /= n; _y /= n; _z /= n; return *this; }  

    Vector3D rotateEuler(const Vector3D &eulerDeg) const {
        double rx = eulerDeg.getX() * M_PI / 180.0;
        double ry = eulerDeg.getY() * M_PI / 180.0;
        double rz = eulerDeg.getZ() * M_PI / 180.0;

        double cx = std::cos(rx), sx = std::sin(rx);
        double cy = std::cos(ry), sy = std::sin(ry);
        double cz = std::cos(rz), sz = std::sin(rz);

        double y1 = _y * cx - _z * sx;
        double z1 = _y * sx + _z * cx;
        double x1 = _x;

        double x2 = x1 * cy + z1 * sy;
        double z2 = -x1 * sy + z1 * cy;
        double y2 = y1;

        double x3 = x2 * cz - y2 * sz;
        double y3 = x2 * sz + y2 * cz;
        double z3 = z2;

        return Vector3D(x3, y3, z3);
    }

    static Vector3D rotateAroundPoint(const Vector3D &point, const Vector3D &center, const Vector3D &eulerDeg) {
        return (point - center).rotateEuler(eulerDeg) + center;
    }      
};
}

#endif