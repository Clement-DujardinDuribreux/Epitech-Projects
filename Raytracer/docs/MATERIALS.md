# Guide: Creating a New Material 🎨

Materials define how light interacts with a surface (color, reflection, refraction, etc.).

## 🧱 The `IMaterial` Interface

Your class must implement:

```cpp
virtual Math::Vector3D getRendering(
    RayTracer::IPrimitive *currentObj,
    std::vector<RayTracer::IPrimitive *> objs,
    std::vector<RayTracer::ILight *> lights,
    RayTracer::Ray r,
    Math::Point3D hit,
    RayTracer::RenderingContext context
) const = 0;
```

- `getRendering`: This is the core method. It calculates the final color of a pixel. 
- It usually delegates some work to the `context.renderer->Render(...)` which handles standard lighting (Lambert, etc.).

## 💡 Recommendation: Use `AMaterial`

Inherit from `AMaterial` to handle the `shininess` property automatically.

## 🛠️ Example: A Neon Material

A neon material might ignore shadows and just return its own bright color.

### 1. Header (`NeonMaterial.hpp`)
```cpp
class NeonMaterial : public AMaterial {
    public:
        NeonMaterial(Math::Vector3D color) : AMaterial(0), _color(color) {}
        
        Math::Vector3D getRendering(...) const override {
            // Returns fixed color, ignoring lighting/shadows
            return _color;
        }
    private:
        Math::Vector3D _color;
};
```

### 2. Plugin Export
```cpp
extern "C" {
    const char *getName() { return "neon"; }

    IMaterial *createMaterial(const libconfig::Setting &setting) {
        auto color = CommonParser::parseColor(setting.lookup("color"));
        return new NeonMaterial(color);
    }

    void destroyMaterial(IMaterial *m) { delete m; }
}
```

## 📍 Integration in Scene
```libconfig
primitives: {
    spheres = (
        { 
            center = { x = 0; y = 0; z = 0; }; 
            radius = 10;
            material = { type = "neon"; color = { r = 0; g = 255; b = 0; }; };
        }
    );
};
```
