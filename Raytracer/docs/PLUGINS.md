# Guide: Creating and Integrating Plugins 🔌

The Raytracer uses a dynamic plugin system based on `.so` files. This allows you to add new features (primitives, materials, lights, etc.) without recompiling the main core.

## 🛠️ Plugin Structure

Every plugin must be compiled as a shared library and export specific `extern "C"` functions so the `Factory` can load it.

### Required Functions for All Plugins

Regardless of the type, every plugin must implement:

```cpp
extern "C" {
    // Returns the name used in the .cfg file to identify this component
    const char *getName(void);
}
```

### Type-Specific Functions

Depending on what your plugin provides, you must implement one of the following sets of functions:

| Type | Creation Function | Destruction Function |
| :--- | :--- | :--- |
| **Primitive** | `IPrimitive *createPrimitive(const libconfig::Setting &setting)` | `void destroyPrimitive(IPrimitive *primitive)` |
| **Material** | `IMaterial *createMaterial(const libconfig::Setting &setting)` | `void destroyMaterial(IMaterial *material)` |
| **Light** | `ILight *createLight(const libconfig::Setting &setting)` | `void destroyLight(ILight *light)` |
| **Transform** | `ITransform *createTransform(const libconfig::Setting &setting)` | `void destroyTransform(ITransform *transform)` |
| **Rendering** | `IRendering *createRendering(const libconfig::Setting &setting)` | `void destroyRendering(IRendering *rendering)` |

---

## 📝 Step-by-Step: Creating a Plugin

### 1. Define your Class
Inherit from the appropriate interface (e.g., `IPrimitive`) or abstract class (e.g., `APrimitive`).

### 2. Implement the Logic
Implement the virtual methods required by the interface.

### 3. Implement the `extern "C"` Entry Points
Create a `.cpp` file that handles the `libconfig` parsing and object instantiation.

### 4. Update the Makefile
Add your new `.cpp` file to the `SRC_PLUGINS` list in the `Makefile`.

```makefile
SRC_PLUGINS = ... \
              src/your_folder/YourNewComponent.cpp
```

### 5. Compile
Run `make` or `make plugins`. The `.so` will be generated in the `plugins/` directory.

---

## ⚙️ Configuration Parsing

We use `libconfig++` for scene files. To simplify parsing, use the `RayTracer::CommonParser` utility:

- `verifPoint(setting, "key")`: Checks if a point exists.
- `parsePoint(setting.lookup("key"))`: Returns a `Math::Point3D`.
- `verifNumber(setting, "key")`: Checks if a number exists.
- `getNumber(setting.lookup("key"))`: Returns a `double`.
- `verifColor(setting, "key")`: Checks if a color exists.
- `parseColor(setting.lookup("key"))`: Returns a `Math::Vector3D` (RGB).

---

## 🚀 Integration

Once your plugin is in the `plugins/` folder, you can use it in any `.cfg` file by using the name returned by `getName()`.

Example if `getName()` returns `"my_custom_sphere"`:
```libconfig
primitives: {
    my_custom_sphere = (
        { center = { x = 0; y = 0; z = 0; }; radius = 10; }
    );
};
```
