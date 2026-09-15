# Internal Works: How the Engine Renders ⚙️

This document explains the step-by-step process of how the Raytracer transforms a scene configuration into a final PPM image.

## 1. Scene Parsing 📄
The `ConfigParser` reads the `.cfg` file. It uses the `Factory` to dynamically load plugins from the `plugins/` directory. 
- It creates a `Camera` with the specified resolution and position.
- It instantiates all `Primitives`, `Materials`, `Lights`, and `Transforms`.
- Each primitive is immediately modified by its assigned transforms.

## 2. Rendering Orchestration: TileMap 🗺️
To optimize performance, the image is divided into a grid of **32x32 pixel tiles**.
- The `TileMap` class manages these tiles.
- It uses a `PoolThread` (Thread Pool) to render tiles in parallel across all available CPU cores.

## 3. The Ray Tracing Loop 🔄
For every pixel in a tile:
1. **Ray Generation:** The `Camera` generates a `Ray` starting from its position and passing through the corresponding point on the virtual screen (viewport).
2. **Intersection Testing:** The ray is tested against all `Primitives` in the scene. The engine identifies the closest intersection point.
3. **Material Shading:** If an object is hit, the engine calls the `getRendering()` method of the object's `Material`.

## 4. Shading & Lighting (BaseRendering) 💡
Most materials delegate the actual lighting calculation to the `IRendering` plugin (defaulting to `BaseRendering`). The process follows these steps:

### A. Ambient Light
Calculates the base level of light that reaches every surface regardless of orientation.

### B. Ambient Occlusion (AO)
- Sends multiple randomized "probe" rays from the hit point.
- If these rays hit other objects nearby, the point is darkened.
- This creates realistic soft shadows in corners and crevices.

### C. Diffuse Lighting (Lambert)
- Calculates light based on the angle between the surface normal and the light direction.
- Uses the formula: `DotProduct(Normal, LightDirection)`.

### D. Shadow Casting 🌑
- For every light source, a "shadow ray" is sent from the hit point towards the light.
- If another primitive blocks this ray, the light's contribution is ignored for that point, creating a sharp shadow.

### E. Specular Highlights (Phong)
- Calculates "shiny" spots on objects based on the reflection of light towards the camera.

## 5. Final Output 🖼️
Once all tiles are rendered, the `TileMap::display()` method iterates through the tiles in the correct order and prints the RGB values to the standard output in **PPM format**.

---

## 🛠️ Class Diagram Overview

- **`Camera`**: Handles viewport math and ray generation.
- **`TileMap` / `Tile`**: Handles multithreading and spatial division.
- **`PoolThread`**: A generic thread pool implementation.
- **`Pixel`**: Smallest unit of rendering, manages its own color calculation.
- **`Ray`**: Defines a start point and a direction vector.
