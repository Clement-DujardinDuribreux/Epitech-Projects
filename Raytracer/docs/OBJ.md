# Guide: Loading 3D Models (.obj) 🗿

The Raytracer supports loading external 3D models using the Wavefront `.obj` file format. This is handled by the `objs` primitive plugin.

## 📦 What is an OBJ file?

An `.obj` file is a standard 3D model format that stores:
- `v`: Vertices (points in 3D space).
- `f`: Faces (triangles or polygons connecting the vertices).

Our implementation automatically triangulates non-triangular faces during loading.

## 🚀 How to use in a Scene

To load an OBJ model, use the `objs` group in your `primitives` section:

```libconfig
primitives: {
    objs = (
        {
            path = "assets/teapot.obj";
            center = { x = 0; y = 0; z = 100; };
            material = { 
                type = "color"; 
                color = { r = 200; g = 200; b = 200; }; 
            };
        }
    );
};
```

### Parameters
- `path`: (String) Absolute or relative path to the `.obj` file.
- `center`: (Point3D) Where the model's center will be positioned in the scene.
- `material`: (Standard Material) Any material plugin can be applied to the entire mesh.

---

## ⚡ Performance & Optimization

Loading large OBJ files (millions of triangles) can be slow. To maintain high performance, our engine uses **Bounding Box Acceleration**:

1. **Bounding Box Calculation**: During loading, the engine calculates an axis-aligned bounding box (AABB) that perfectly contains the entire model.
2. **Early Exit**: For every ray, the engine first tests if the ray hits the bounding box.
3. **Triangle Testing**: If (and only if) the box is hit, the engine proceeds to test intersection against every individual triangle in the mesh.

### Best Practices
- **Pre-triangulate**: Although the engine handles it, pre-triangulating your models in Blender reduces loading time.
- **Scale**: Ensure your model scale matches your scene's coordinate system.
- **Center**: The `center` parameter moves the mesh's geometric center to the target point.

---

## ⚠️ Known Limitations
- **No MTL Support**: Currently, `.mtl` files (external materials) are not parsed. The material must be defined in the `.cfg` file.
- **No Vertex Normals**: The engine calculates normals per-face. Smooth shading (Phong interpolation) using vertex normals is not yet supported.
- **No UV Mapping**: Texture mapping is currently not supported via the OBJ plugin.
