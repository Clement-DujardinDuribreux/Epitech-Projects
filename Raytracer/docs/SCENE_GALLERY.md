# Scene Gallery & Test Cases 🖼️

The `scenes/` directory contains various configuration files designed to showcase and test specific features of the Raytracer.

## 🌟 Featured Scenes

| File | Description | Key Feature |
| :--- | :--- | :--- |
| `test.cfg` | The standard benchmark scene. | Basic lighting & primitives |
| `mirror_test.cfg` | A scene with highly reflective surfaces. | Reflections (Mirror Material) |
| `golden_eclipse.cfg` | A complex, artistic scene. | Advanced lighting & Composition |
| `fractal.cfg` | Renders a 3D Sierpinski Fractal. | Recursive Primitives |
| `mobius.cfg` | Showcases the Mobius Strip primitive. | Non-orientable Surfaces |
| `obj_test.cfg` | Tests external 3D model loading. | OBJ Plugin |

---

## 🧪 Geometric Test Cases

These scenes are designed to verify the mathematical correctness of specific primitives:

- **Basic Shapes**: `sphere.cfg`, `box.cfg`, `cone.cfg`, `cylinder.cfg`, `torus.cfg`.
- **Polygons**: `triangle.cfg`, `pyramide.cfg`, `tetrahedron.cfg`.
- **Complex Geometry**: `mobius.cfg`, `fractal.cfg`.

---

## 💡 Lighting & Material Tests

- **`dircolorlight.cfg`**: Tests directional lights with specific RGB colors.
- **`yellow_black.cfg`**: High-contrast scene to test shadow sharpess and ambient light.
- **`mirror_test.cfg`**: Verifies that rays are correctly reflected according to the surface normal.

---

## 🏃 How to run a specific scene

To render any of these scenes, use the `create_ppm.sh` script:

```bash
./create_ppm.sh scenes/mirror_test.cfg
```

Then open `output.ppm` with an image viewer (like `eog`, `display`, or `feh`).
