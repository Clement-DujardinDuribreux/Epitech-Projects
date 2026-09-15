# Guide: Creating a New Light 💡

Lights illuminate your scene and interact with materials. To create a new light type, you need to implement the `ILight` interface.

## 🧱 The `ILight` Interface

Your class must implement these key methods:

```cpp
virtual Math::Point3D getOrigin() const = 0;
virtual Math::Vector3D getColor() const = 0;
virtual double getLightingValue(Math::Point3D p) const = 0;
virtual Math::Vector3D getLightDir(Math::Point3D p) const = 0;
```

- `getOrigin`: The position of the light source.
- `getColor`: The RGB color of the light (usually normalized or 0-255).
- `getLightingValue`: The intensity of the light at point `p`. This can account for distance attenuation.
- `getLightDir`: The vector pointing from the surface point `p` towards the light source.

## 💡 Recommendation: Use `ALight`

Inherit from `ALight` to automatically handle the storage and retrieval of the `_origin`.

## 🛠️ Example: A Simple Point Light

### 1. Header (`MyLight.hpp`)
```cpp
class MyLight : public ALight {
    public:
        MyLight(Math::Point3D origin, Math::Vector3D color) : ALight(origin), _color(color) {}
        
        Math::Vector3D getLightDir(Math::Point3D p) const override {
            return Math::Vector3D(_origin.getX() - p.getX(), ...).getNormal();
        }
        
        double getLightingValue(Math::Point3D p) const override {
            // Example: No attenuation
            return 1.0; 
        }

        Math::Vector3D getColor() const override { return _color; }
    private:
        Math::Vector3D _color;
};
```

### 2. Plugin Export
```cpp
extern "C" {
    const char *getName() { return "my_point_light"; }

    ILight *createLight(const libconfig::Setting &setting) {
        auto origin = CommonParser::parsePoint(setting.lookup("origin"));
        auto color = CommonParser::parseColor(setting.lookup("color"));
        return new MyLight(origin, color);
    }

    void destroyLight(ILight *l) { delete l; }
}
```

## 📍 Integration in Scene
```libconfig
lights: {
    my_point_light = (
        {
            origin = { x = 100; y = 100; z = 0; };
            color = { r = 255; g = 255; b = 255; };
        }
    );
};
```
