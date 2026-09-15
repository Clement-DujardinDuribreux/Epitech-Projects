# Optimization Guide ⚡

The Raytracer is designed to be efficient, but ray tracing is computationally expensive. Here is how to tune the engine for maximum performance.

## 🧵 Multithreading

The engine automatically uses a **Thread Pool** to distribute the workload across all available CPU threads. 
- **Tiles**: The image is divided into 32x32 pixel tiles. Each thread picks a tile, renders it, and moves to the next.
- **Scalability**: Adding more CPU cores will result in almost linear speedup.

## 📉 Tuning for Speed

If rendering is too slow, adjust these parameters in your `.cfg` file:

### 1. Resolution
The most direct impact. Reducing 1080p to 720p reduces the number of rays by ~50%.
```libconfig
resolution = { width = 1280; height = 720; };
```

### 2. Ambient Occlusion (AO)
AO is very expensive because it shoots multiple extra rays for *every* hit point.
- **Drafting**: Set `AOsamples = 0;` to disable it.
- **Quality**: Use `AOsamples = 16;` or `32;` for final renders.
- **Radius**: A smaller `AOradius` can sometimes speed up intersection tests by hitting fewer objects.

### 3. Scene Complexity
- **Primitives**: More objects mean more intersection tests. 
- **OBJ Models**: Use models with lower triangle counts. The engine uses **Bounding Boxes**, so many small models are often faster than one massive model with millions of triangles.

## 🛠️ Code-Level Optimizations

If you are developing plugins, keep these tips in mind:

### Intersection Math
- Use the **Quadratic Formula** efficiently. Calculate the discriminant once and exit early if it's negative.
- Avoid `sqrt()` whenever possible (e.g., compare squared distances instead of real distances).

### Memory Management
- The `Factory` and `DlLoader` are used once during startup. The main rendering loop avoids allocations and uses stack-allocated `Ray` and `Vector3D` objects to prevent heap overhead.

### Compiler Flags
Ensure you compile with optimization flags enabled:
```bash
# In your Makefile
CXXFLAGS = -O3 -march=native -ffast-math
```
- `-O3`: Maximum optimization.
- `-march=native`: Optimizes the code specifically for your CPU.
- `-ffast-math`: Speeds up floating-point calculations (at the cost of some precision).
