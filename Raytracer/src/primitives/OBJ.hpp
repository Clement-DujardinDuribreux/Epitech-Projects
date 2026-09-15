/*
** EPITECH PROJECT, 2026
** OBJ
** File description:
** OBJ primitive
*/

#ifndef OBJ_PRIMITIVE
    #define OBJ_PRIMITIVE

    #include "APrimitive.hpp"
    #include "Triangle.hpp"

    #include <array>
    #include <algorithm>
    #include <cmath>
    #include <fstream>
    #include <sstream>
    #include <stdexcept>
    #include <string>
    #include <vector>

namespace RayTracer {
/** @class OBJ
 * @brief A primitive representing an OBJ model
 * This class represents a 3D model loaded from an OBJ file. It parses the vertices and faces from the file and provides methods for calculating ray intersections and normals for the model. The OBJ primitive uses a collection of triangles to represent the surface of the model, allowing for complex shapes to be rendered in the ray tracer.
 * The hits method calculates the intersection of a ray with the model by checking for intersections with each triangle face, while the getNormal method computes the normal vector at the point of intersection based on the normal of the intersected triangle. The class also includes functionality for loading the OBJ file, parsing its contents, and managing the vertex and face data.
 * @see APrimitive
 */
class OBJ : public RayTracer::APrimitive {
    private:
        struct Face {
            std::array<int, 3> indices;
            RayTracer::Triangle triangle;
            Math::Vector3D normal;
        };

        struct MeshNode {
            Math::Point3D min;
            Math::Point3D max;
            std::vector<Face*> faces;
            MeshNode *left = nullptr;
            MeshNode *right = nullptr;

            ~MeshNode() {
                delete left;
                delete right;
            }
        };

        std::vector<Math::Point3D> _vertices;
        std::vector<Face> _faces;
        MeshNode *_rootNode = nullptr;
        Math::Point3D _min;
        Math::Point3D _max;

        void clearTree() {
            delete _rootNode;
            _rootNode = nullptr;
        }

        MeshNode* buildTree(std::vector<Face*> facesList, int depth) {
            MeshNode *node = new MeshNode();
            if (facesList.empty())
                return node;
            
            double minX = _vertices[facesList[0]->indices[0]].getX();
            double minY = _vertices[facesList[0]->indices[0]].getY();
            double minZ = _vertices[facesList[0]->indices[0]].getZ();
            double maxX = minX;
            double maxY = minY;
            double maxZ = minZ;

            for (Face *face : facesList) {
                for (int i = 0; i < 3; i++) {
                    const Math::Point3D &v = _vertices[face->indices[i]];
                    minX = std::min(minX, v.getX());
                    minY = std::min(minY, v.getY());
                    minZ = std::min(minZ, v.getZ());
                    maxX = std::max(maxX, v.getX());
                    maxY = std::max(maxY, v.getY());
                    maxZ = std::max(maxZ, v.getZ());
                }
            }
            node->min = Math::Point3D(minX, minY, minZ);
            node->max = Math::Point3D(maxX, maxY, maxZ);

            if (facesList.size() <= 5 || depth > 15) {
                node->faces = facesList;
                return node;
            }

            double sizeX = maxX - minX;
            double sizeY = maxY - minY;
            double sizeZ = maxZ - minZ;
            int axis = 0;
            if (sizeY > sizeX && sizeY > sizeZ) axis = 1;
            else if (sizeZ > sizeX && sizeZ > sizeY) axis = 2;

            std::sort(facesList.begin(), facesList.end(), [this, axis](Face *a, Face *b) {
                double valA = (_vertices[a->indices[0]].getX() + _vertices[a->indices[1]].getX() + _vertices[a->indices[2]].getX());
                double valB = (_vertices[b->indices[0]].getX() + _vertices[b->indices[1]].getX() + _vertices[b->indices[2]].getX());
                if (axis == 1) {
                    valA = (_vertices[a->indices[0]].getY() + _vertices[a->indices[1]].getY() + _vertices[a->indices[2]].getY());
                    valB = (_vertices[b->indices[0]].getY() + _vertices[b->indices[1]].getY() + _vertices[b->indices[2]].getY());
                } else if (axis == 2) {
                    valA = (_vertices[a->indices[0]].getZ() + _vertices[a->indices[1]].getZ() + _vertices[a->indices[2]].getZ());
                    valB = (_vertices[b->indices[0]].getZ() + _vertices[b->indices[1]].getZ() + _vertices[b->indices[2]].getZ());
                }
                return valA < valB;
            });

            std::size_t half = facesList.size() / 2;
            std::vector<Face*> leftFaces(facesList.begin(), facesList.begin() + half);
            std::vector<Face*> rightFaces(facesList.begin() + half, facesList.end());

            node->left = buildTree(leftFaces, depth + 1);
            node->right = buildTree(rightFaces, depth + 1);

            return node;
        }

        void rebuildTree() {
            clearTree();
            std::vector<Face*> facePointers;
            for (Face &face : _faces)
                facePointers.push_back(&face);
            _rootNode = buildTree(facePointers, 0);
        }

        /** @brief Create a vector from two points
         * @param from The starting point
         * @param to The ending point
         * @return The vector from the starting point to the ending point
         */
        static Math::Vector3D createVector(Math::Point3D from, Math::Point3D to) {
            return Math::Vector3D(to.getX() - from.getX(), to.getY() - from.getY(), to.getZ() - from.getZ());
        }

        /** @brief Calculate the intersection of a ray with a triangle
         * @param ray The ray to test for intersection
         * @param triangle The triangle to test against
         * @return The point of intersection if the ray hits the triangle, or an empty point if there is no intersection
         */
        static double getDistance(Math::Point3D a, Math::Point3D b) {
            Math::Vector3D distance(a.getX() - b.getX(), a.getY() - b.getY(), a.getZ() - b.getZ());

            return distance.dot(distance);
        }

        /** @brief Parse an index from a face definition in the OBJ file
         * @param value The string value representing the index to be parsed
         * @return The parsed index as an integer, or -1 if the index is invalid
         */
        int parseIndex(const std::string &value) const {
            std::size_t length = 0;
            int i = 0;

            try {
                i = std::stoi(value, &length);
            } catch (const std::exception &) {
                return -1;
            }
            if (length == 0)
                return -1;
            if (i < 0)
                i = static_cast<int>(_vertices.size()) + i + 1;
            if (i <= 0 || i > static_cast<int>(_vertices.size()))
                return -1;
            return static_cast<int>(i - 1);
        }

        /** @brief Parse a vertex definition from the OBJ file
         * @param line The line of text containing the vertex definition to be parsed
         */
        void parseVertex(const std::string &line) {
            std::istringstream stream(line.substr(2));
            double x = 0;
            double y = 0;
            double z = 0;

            if (!(stream >> x >> y >> z))
                return;
            _vertices.push_back(Math::Point3D(x, y, z));
        }

        /** @brief Add a face to the OBJ model using the specified vertex indices
         * @param first The index of the first vertex of the face
         * @param second The index of the second vertex of the face
         * @param third The index of the third vertex of the face
         */
        void addFace(int first, int second, int third) {
            if (first < 0 || second < 0 || third < 0)
                return;
            RayTracer::Triangle triangle(_vertices[first], _vertices[second], _vertices[third]);
            Math::Vector3D normal = triangle.getNormal(Math::Point3D(), RayTracer::Ray());

            _faces.push_back({ {first, second, third}, std::move(triangle), normal });
        }

        /** @brief Parse a face definition from the OBJ file
         * @param line The line of text containing the face definition to be parsed
         */
        void parseFace(const std::string &line) {
            std::istringstream stream(line.substr(2));
            std::string value;
            std::vector<int> faceIndices;

            faceIndices.reserve(4);
            while (stream >> value)
                faceIndices.push_back(parseIndex(value));
            if (faceIndices.size() < 3)
                return;
            for (std::size_t i = 1; i + 1 < faceIndices.size(); i++)
                addFace(faceIndices[0], faceIndices[i], faceIndices[i + 1]);
        }

        /** @brief Update the bounding box of the OBJ model based on its vertices
         * This method calculates the minimum and maximum points of the bounding box that encompasses all the vertices of the OBJ model. 
         * It iterates through all the vertices to find the minimum and maximum x, y, and z coordinates, which are then used to define the corners of 
         * the bounding box. The resulting minimum and maximum points are stored in the _min and _max member variables, respectively.
         */
        void updateBox() {
            if (_vertices.empty()) {
                _min = Math::Point3D();
                _max = Math::Point3D();
                return;
            }

            double minX = _vertices[0].getX();
            double minY = _vertices[0].getY();
            double minZ = _vertices[0].getZ();
            double maxX = minX;
            double maxY = minY;
            double maxZ = minZ;

            for (const Math::Point3D &currentVertex : _vertices) {
                minX = std::min(minX, currentVertex.getX());
                minY = std::min(minY, currentVertex.getY());
                minZ = std::min(minZ, currentVertex.getZ());
                maxX = std::max(maxX, currentVertex.getX());
                maxY = std::max(maxY, currentVertex.getY());
                maxZ = std::max(maxZ, currentVertex.getZ());
            }
            _min = Math::Point3D(minX, minY, minZ);
            _max = Math::Point3D(maxX, maxY, maxZ);
        }

        /** @brief Rebuild the triangle faces of the OBJ model after transformations
         * This method recalculates the triangle faces of the OBJ model based on the current vertex positions. 
         * It iterates through all the faces and updates their triangle definitions and normals to reflect any changes in the vertex positions, 
         * such as those caused by transformations like translation, rotation, or scaling. This ensures that the geometry of the model remains accurate for ray intersection tests and normal calculations.
         */
        void rebuildFaces() {
            for (Face &face : _faces) {
                face.triangle = RayTracer::Triangle(_vertices[face.indices[0]], _vertices[face.indices[1]], _vertices[face.indices[2]]);
                face.normal = face.triangle.getNormal(Math::Point3D(), RayTracer::Ray());
            }
            rebuildTree();
        }

        /** @brief Load an OBJ file and parse its contents to populate the model's vertices and faces
         * @param path The file path to the OBJ file to be loaded
         */
        void loadFile(const std::string &path) {
            std::ifstream file(path);
            std::string line;

            if (!file.is_open())
                return;
            while (std::getline(file, line)) {
                if (line.size() < 2)
                    continue;
                if (line[0] == 'v' && line[1] == ' ')
                    parseVertex(line);
                if (line[0] == 'f' && line[1] == ' ')
                    parseFace(line);
            }
            if (_vertices.empty() || _faces.empty())
                return;
            updateBox();
            _center = Math::Point3D((_min.getX() + _max.getX()) / 2.0, (_min.getY() + _max.getY()) / 2.0, (_min.getZ() + _max.getZ()) / 2.0);
        }

        /** @brief Move the OBJ model to a new center position
         * @param center The new center point to move the model to
         * This method calculates the translation vector needed to move the model from its current center to the specified new center. 
         * It then applies this translation to all the vertices of the model, effectively moving the entire model to the new position. 
         * After updating the vertex positions, it calls rebuildFaces() to update the triangle definitions and normals, and updateBox() to recalculate the bounding box based on the new vertex positions.
         */
        void moveTo(Math::Point3D center) {
            Math::Vector3D move(center.getX() - _center.getX(), center.getY() - _center.getY(), center.getZ() - _center.getZ());
            for (Math::Point3D &currentVertex : _vertices)
                currentVertex = currentVertex + move;
            _center = center;
            rebuildFaces();
            updateBox();
        }

        /** @brief Update the intersection range for a ray and a bounding box along a specific axis
         * @param origin The origin coordinate of the ray along the axis
         * @param direction The direction coordinate of the ray along the axis
         * @param boxMin The minimum coordinate of the bounding box along the axis
         * @param boxMax The maximum coordinate of the bounding box along the axis
         * @param minDistance A reference to the minimum distance variable to be updated with the new intersection range
         * @param maxDistance A reference to the maximum distance variable to be updated with the new intersection range
         * @param hasRange A reference to a boolean variable indicating whether an intersection range has already been established, which will be updated if this is the first range being calculated
         * @return true if the ray intersects with the bounding box along this axis, false otherwise
         */
        bool updateAxisRange(double origin, double direction, double boxMin, double boxMax, double &minDistance, double &maxDistance, bool &hasRange) const {
            if (std::abs(direction) < 0.000001) {
                if (origin < boxMin || origin > boxMax)
                    return false;
                return true;
            }

            double first = (boxMin - origin) / direction;
            double second = (boxMax - origin) / direction;

            if (first > second)
                std::swap(first, second);
            if (!hasRange) {
                minDistance = first;
                maxDistance = second;
                hasRange = true;
            } else {
                minDistance = std::max(minDistance, first);
                maxDistance = std::min(maxDistance, second);
            }
            if (minDistance > maxDistance)
                return false;
            return true;
        }

        /** @brief Check if a ray intersects with the bounding box of the OBJ model
         * @param ray The ray to test for intersection with the bounding box
         * @return true if the ray intersects with the bounding box, false otherwise
         * This method uses the slab method to determine if the ray intersects with the axis-aligned bounding box defined by the minimum and maximum points of the model. 
         * It checks for intersections along each axis (x, y, z) and updates the intersection range accordingly. If at any point it determines that there is no intersection along an axis, it returns false. 
         * If it successfully finds an intersection range along all three axes, it returns true, indicating that the ray intersects with the bounding box.
         */
        bool hitBox(RayTracer::Ray &ray) const {
            double minDistance = 0.0;
            double maxDistance = 0.0;
            bool hasRange = false;

            if (!updateAxisRange(ray.getOrigin().getX(), ray.getDirection().getX(), _min.getX(), _max.getX(), minDistance, maxDistance, hasRange))
                return false;
            if (!updateAxisRange(ray.getOrigin().getY(), ray.getDirection().getY(), _min.getY(), _max.getY(), minDistance, maxDistance, hasRange))
                return false;
            if (!updateAxisRange(ray.getOrigin().getZ(), ray.getDirection().getZ(), _min.getZ(), _max.getZ(), minDistance, maxDistance, hasRange))
                return false;
            return maxDistance > 0.000001;
        }

        /** @brief Check if a point is inside a face of the OBJ model
         * @param hit The point to check
         * @param face The face to check against
         * @return true if the point is inside the face, false otherwise
         * This method uses barycentric coordinates to determine if a point is inside a triangle. 
         * It calculates the barycentric coordinates (u, v) of the point relative to the triangle's vertices and checks if they are within the valid range [0, 1] such that u + v <= 1.
         */
        bool isInsideFace(Math::Point3D hit, const Face &face) const {
            Math::Point3D a = _vertices[face.indices[0]];
            Math::Point3D b = _vertices[face.indices[1]];
            Math::Point3D c = _vertices[face.indices[2]];
            Math::Vector3D ab = createVector(a, b);
            Math::Vector3D ac = createVector(a, c);
            Math::Vector3D ah = createVector(a, hit);
            double divider = ab.dot(ab) * ac.dot(ac) - ab.dot(ac) * ac.dot(ab);

            if (std::abs(divider) < 0.000001)
                return false;
            double u = (ac.dot(ac) * ah.dot(ab) - ab.dot(ac) * ah.dot(ac)) / divider;
            double v = (ab.dot(ab) * ah.dot(ac) - ab.dot(ac) * ah.dot(ab)) / divider;
            return u >= -0.0001 && v >= -0.0001 && u + v <= 1.0001;
        }

        bool hitNodeBox(RayTracer::Ray &ray, Math::Point3D minBox, Math::Point3D maxBox) const {
            double minDistance = 0.0;
            double maxDistance = 0.0;
            bool hasRange = false;

            if (!updateAxisRange(ray.getOrigin().getX(), ray.getDirection().getX(), minBox.getX(), maxBox.getX(), minDistance, maxDistance, hasRange))
                return false;
            if (!updateAxisRange(ray.getOrigin().getY(), ray.getDirection().getY(), minBox.getY(), maxBox.getY(), minDistance, maxDistance, hasRange))
                return false;
            if (!updateAxisRange(ray.getOrigin().getZ(), ray.getDirection().getZ(), minBox.getZ(), maxBox.getZ(), minDistance, maxDistance, hasRange))
                return false;
            return maxDistance > 0.000001;
        }

        Math::Point3D hitNode(MeshNode *node, RayTracer::Ray &ray, double &closeDistance, bool &isclose) const {
            if (!node || !hitNodeBox(ray, node->min, node->max))
                return Math::Point3D();

            Math::Point3D bestHit;

            if (node->left == nullptr && node->right == nullptr) {
                for (Face *face : node->faces) {
                    Math::Point3D hit = face->triangle.hits(ray);
                    if (hit != Math::Point3D()) {
                        double distance = getDistance(ray.getOrigin(), hit);
                        if (!isclose || distance < closeDistance) {
                            bestHit = hit;
                            closeDistance = distance;
                            isclose = true;
                        }
                    }
                }
                return bestHit;
            }

            Math::Point3D leftHit = hitNode(node->left, ray, closeDistance, isclose);
            Math::Point3D rightHit = hitNode(node->right, ray, closeDistance, isclose);

            if (rightHit != Math::Point3D() && leftHit != Math::Point3D()) {
                double distL = getDistance(ray.getOrigin(), leftHit);
                double distR = getDistance(ray.getOrigin(), rightHit);
                return (distL < distR) ? leftHit : rightHit;
            }
            if (leftHit != Math::Point3D()) return leftHit;
            return rightHit;
        }

        bool isHitInBox(Math::Point3D hit, Math::Point3D minBox, Math::Point3D maxBox) const {
            return hit.getX() >= minBox.getX() - 0.001 && hit.getX() <= maxBox.getX() + 0.001 &&
                   hit.getY() >= minBox.getY() - 0.001 && hit.getY() <= maxBox.getY() + 0.001 &&
                   hit.getZ() >= minBox.getZ() - 0.001 && hit.getZ() <= maxBox.getZ() + 0.001;
        }

        Math::Vector3D getNormalNode(MeshNode *node, Math::Point3D hit, RayTracer::Ray ray) const {
            if (!node || !isHitInBox(hit, node->min, node->max))
                return Math::Vector3D();

            if (node->left == nullptr && node->right == nullptr) {
                for (Face *face : node->faces) {
                    Math::Point3D Newhit = face->triangle.hits(ray);
                    if (Newhit != Math::Point3D() && hit.getDistance(Newhit) < 0.001)
                        return face->triangle.getNormal(hit, ray);
                }
                return Math::Vector3D();
            }

            Math::Vector3D leftNormal = getNormalNode(node->left, hit, ray);
            if (leftNormal.length() > 0) return leftNormal;

            return getNormalNode(node->right, hit, ray);
        }

    public:
        OBJ() : RayTracer::APrimitive(Math::Point3D()), _min(Math::Point3D()), _max(Math::Point3D()) {}
        /** @brief Construct an OBJ primitive by loading a model from an OBJ file and positioning it at a specified center point
         * @param path The file path to the OBJ file to be loaded
         * @param center The center point to position the loaded model at
         * This constructor initializes the OBJ primitive by loading the model data from the specified OBJ file and then moving the model to the given center position. 
         * It calls the loadFile method to parse the OBJ file and populate the vertices and faces, and then calls moveTo to position the model at the desired location in the scene.
         */
        OBJ(const std::string &path, Math::Point3D center) : RayTracer::APrimitive(Math::Point3D()), _min(Math::Point3D()), _max(Math::Point3D()) {
            loadFile(path);
            moveTo(center);
        }
        ~OBJ() override {
            clearTree();
        }

        /** @brief Calculate the intersection of a ray with the OBJ model
         * @param ray The ray to test for intersection
         * @return The point of intersection if the ray hits the model, or an empty point if there is no intersection
         * This method first checks if the ray intersects with the bounding box of the model using hitBox(). If it does, it iterates through all the faces of the model and checks for intersections with each triangle. 
         * It keeps track of the closest intersection point found and returns it at the end. If no intersections are found, it returns an empty point.
         */
        Math::Point3D hits(RayTracer::Ray &ray) const override {
            if (_faces.empty() || !_rootNode)
                return Math::Point3D();
            double closeDistance = 0.0;
            bool isclose = false;
            return hitNode(_rootNode, ray, closeDistance, isclose);
        }

        /** @brief Get the normal vector at a point on the OBJ model
         * @param hit The point on the model for which to calculate the normal
         * @param ray The ray that intersected with the model
         * @return The normal vector at the intersection point, or an empty vector if no normal can be calculated
         * This method iterates through all the faces of the model and checks if the intersection point is within each face. If so, it returns the face's normal vector.
        */
        Math::Vector3D getNormal(Math::Point3D hit, RayTracer::Ray ray) const override {
            if (_faces.empty() || !_rootNode)
                return Math::Vector3D();
            return getNormalNode(_rootNode, hit, ray);
        }

        void applyTranslation(Math::Vector3D translation) override {
            moveTo(_center + translation);
        }

        void applyScaling(Math::Vector3D scaling) override {
            for (Math::Point3D &v : _vertices) {
                v = Math::Point3D(
                    _center.getX() + (v.getX() - _center.getX()) * scaling.getX(),
                    _center.getY() + (v.getY() - _center.getY()) * scaling.getY(),
                    _center.getZ() + (v.getZ() - _center.getZ()) * scaling.getZ()
                );
            }
        }

        void applyRotation(Math::Vector3D rotation) override {
            Math::Vector3D centerVec(_center.getX(), _center.getY(), _center.getZ());
            for (Math::Point3D &v : _vertices) {
                Math::Vector3D vVec(v.getX(), v.getY(), v.getZ());
                Math::Vector3D rotated = Math::Vector3D::rotateAroundPoint(vVec, centerVec, rotation);
                v = Math::Point3D(rotated.getX(), rotated.getY(), rotated.getZ());
            }
            rebuildFaces();
            updateBox();
        }
};
}

#endif
