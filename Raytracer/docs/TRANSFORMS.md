# Guide: Creating a New Transform 🔄

Transforms allow you to modify a primitive's properties (position, scale, rotation, etc.) during the scene parsing phase.

## 🧱 The `ITransform` Interface

Your class must implement:

```cpp
virtual void transform(RayTracer::IPrimitive *primitive) = 0;
```

- `transform`: This method receives a pointer to a primitive and modifies it. 
- For example, a `Translation` transform gets the primitive's center and adds a vector to it.

## 🛠️ Example: A Scaling Transform

Imagine you want a transform that doubles the size of a sphere.

### 1. Header (`ScaleTransform.hpp`)
```cpp
class ScaleTransform : public ITransform {
    public:
        ScaleTransform(double factor) : _factor(factor) {}
        
        void transform(IPrimitive *primitive) override {
            // Check if it's a sphere (simplified)
            // Or add a setScale to IPrimitive if you want to support it generally
            // primitive->setScale(primitive->getScale() * _factor);
        }
    private:
        double _factor;
};
```

### 2. Plugin Export
```cpp
extern "C" {
    const char *getName() { return "scale"; }

    ITransform *createTransform(const libconfig::Setting &setting) {
        double factor = CommonParser::getNumber(setting.lookup("factor"));
        return new ScaleTransform(factor);
    }

    void destroyTransform(ITransform *t) { delete t; }
}
```

## 📍 Integration in Scene
```libconfig
primitives: {
    spheres = (
        { 
            center = { x = 0; y = 0; z = 0; }; 
            radius = 10;
            transforms = {
                scale = { factor = 2.0; };
            };
        }
    );
};
```
