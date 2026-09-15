# Guide: Creating a New Primitive 🧊

Primitives are the physical objects in your 3D scene. To create one, you need to implement the `IPrimitive` interface.

## 🧱 The `IPrimitive` Interface

Your class must implement these key methods:

```cpp
virtual Math::Point3D hits(RayTracer::Ray &ray) const = 0;
virtual Math::Vector3D getNormal(Math::Point3D hit, RayTracer::Ray ray) const = 0;
```

- `hits`: Calculates if a ray intersects your object. Returns the `Point3D` of impact or an "empty" point (default constructor) if it misses.
- `getNormal`: Returns the surface normal at a given impact point. This is crucial for lighting calculations.

## 💡 Recommendation: Use `APrimitive`

Instead of implementing everything from scratch, inherit from `APrimitive`. It already handles:
- Center position (`_center`).
- Material storage (`_material`).
- Standard getters/setters.

## 🛠️ Example: A Simple Cube (Simplified)

### 1. Header (`Cube.hpp`)
```cpp
class Cube : public APrimitive {
    public:
        Cube(Math::Point3D center, double side) : APrimitive(center), _side(side) {}
        Math::Point3D hits(RayTracer::Ray &ray) const override;
        Math::Vector3D getNormal(Math::Point3D hit, RayTracer::Ray ray) const override;
    private:
        double _side;
};
```

### 2. Implementation (`Cube.cpp`)
Focus on the intersection math. For a cube, you'll likely use the AABB (Axis-Aligned Bounding Box) algorithm.

### 3. Plugin Export
In your `.cpp` file, add the factory functions:

```cpp
extern "C" {
    const char *getName() { return "cubes"; }

    IPrimitive *createPrimitive(const libconfig::Setting &setting) {
        // Use CommonParser to get center and side
        auto center = CommonParser::parsePoint(setting.lookup("center"));
        double side = CommonParser::getNumber(setting.lookup("side"));
        return new Cube(center, side);
    }

    void destroyPrimitive(IPrimitive *p) { delete p; }
}
```

## 📍 Integration in Scene
```libconfig
primitives: {
    cubes = (
        { 
            center = { x = 0; y = 0; z = 50; }; 
            side = 20.0;
            material = { type = "color"; color = { r = 255; g = 0; b = 0; }; };
        }
    );
};
```
