# RayTracer - Architecture

## Découpage

### Mathématique
| **Composant** | **Description** |
| :--- | :--- |
| `Point3D` | Représentation mathématique d'un point dans l'espace |
| `Vector3D` | Représentation mathématique d'un vecteur dans l'espace |

### Scène
| **Composant** | **Description** |
| :--- | :--- |
| `Primitive` | Objets courants de la scène (ex: Sphere, Plan, Cylindre, OBJ) |
| `Material` | Matériau qui compose un primitif (ex: Flat Color, Transparency, Mirror) |
| `Transform` | Règles qui agissent sur un primitif pour le transformer (ex: Scale, Rotate, Translate) |
| `Light` | Lumières de la scène (Point, Directional, Spot, Color) |

### RayTracer
| **Composant** | **Description** |
| :--- | :--- |
| `Camera` | Point de l'espace d'où sont envoyés les rayons |
| `Ray` | Rayon qui interagit avec les différents objets de la scène pour le rendu |
| `Rectangle3D` | Représentation de l'écran dans l'espace 3D |
| `Pixel` | Représentation d'un pixel de l'écran pour calculer son rendu |

### Parser & Factory
| **Composant** | **Description** |
| :--- | :--- |
| `ConfigParser` | Parse les fichiers `.cfg` et orchestre la création de la scène |
| `SceneFactory` | Charge les plugins (`.so`) et instancie les objets via `unique_ptr` |
| `DlLoader` | Charge dynamiquement les bibliothèques partagées (plugins) |
| `CommonParser` | Utilitaires de parsing pour les types courants (Point, Vector, Color) |

### Optimisation & Rendu
| **Composant** | **Description** |
| :--- | :--- |
| `TileMap` | Découpe l'image en tuiles 32×32 pour le rendu parallèle |
| `Tile` | Unité de rendu contenant une grille de `Pixel` |
| `PoolThread` | Pool de threads pour exécuter le rendu en parallèle |
| `BaseRendering` | Implémentation par défaut du pipeline de rendu (Lambert + Phong + AO) |
| `LowRendering` | Implémentation allégée du rendu |

---

## Interfaces

### IPrimitive
```cpp
class IPrimitive {
    public:
        virtual ~IPrimitive() = default;

        virtual Math::Point3D getCenter() const = 0;
        virtual RayTracer::IMaterial *getMaterial() const = 0;       // non-owning

        virtual void setMaterial(std::unique_ptr<RayTracer::IMaterial> material) = 0;  // takes ownership
        virtual void setCenter(Math::Point3D center) = 0;

        virtual Math::Point3D hits(RayTracer::Ray &ray) const = 0;
        virtual Math::Vector3D getNormal(Math::Point3D hit, RayTracer::Ray ray) const = 0;

        virtual void applyTranslation(Math::Vector3D translation) = 0;
        virtual void applyScaling(Math::Vector3D scaling) = 0;
        virtual void applyRotation(Math::Vector3D rotation) = 0;
};
```

### IMaterial
```cpp
class IMaterial {
    public:
        virtual ~IMaterial() = default;

        virtual RayTracer::IMaterial *getWrapper() const = 0;                           // non-owning
        virtual void setWrapper(std::unique_ptr<RayTracer::IMaterial> wrapper) = 0;     // takes ownership

        virtual Math::Vector3D getRendering(
            RayTracer::IPrimitive *currentObj,
            std::vector<RayTracer::IPrimitive *> objs,
            std::vector<RayTracer::ILight *> lights,
            RayTracer::Ray r, Math::Point3D hit,
            RayTracer::RenderingContext context
        ) const = 0;

        virtual Math::Vector3D getShadowColor() const = 0;
        virtual double getShadowIntensity(RayTracer::Ray ray, Math::Point3D hit,
            RayTracer::IPrimitive *currentObj,
            std::vector<RayTracer::IPrimitive *> objs) const = 0;
        virtual double getShininess() const = 0;
};
```

### ITransform
```cpp
class ITransform {
    public:
        virtual ~ITransform() = default;

        virtual void transform(RayTracer::IPrimitive *primitive) = 0;
};
```

### ILight
```cpp
class ILight {
    public:
        virtual ~ILight() = default;

        virtual Math::Point3D getOrigin() const = 0;
        virtual Math::Vector3D getColor() const = 0;

        virtual double getLightingValue(Math::Point3D p) const = 0;
        virtual Math::Vector3D getLightDir(Math::Point3D p) const = 0;
};
```

### IRendering
```cpp
class IRendering {
    public:
        virtual ~IRendering() = default;

        virtual Math::Vector3D Render(
            RayTracer::RenderingContext context,
            RayTracer::IPrimitive *currentObj,
            std::vector<RayTracer::IPrimitive *> objs,
            std::vector<RayTracer::ILight *> lights,
            RayTracer::Ray r, Math::Point3D hit,
            Math::Vector3D baseColor, int shininess
        ) const = 0;
};

struct RenderingContext {
    RayTracer::IRendering *renderer;    // non-owning
    double ambient;
    double diffuseMultiplier;
    int AOsamples;
    double AOradius;
    double AOintensity;
};
```

---

## Abstracts

### APrimitive
```cpp
class APrimitive : public RayTracer::IPrimitive {
    protected:
        Math::Point3D _center;
        std::unique_ptr<RayTracer::IMaterial> _material;    // owns the material

    public:
        APrimitive(Math::Point3D center);
        virtual ~APrimitive() = default;

        // Move-only (non-copyable) à cause du unique_ptr
        APrimitive(APrimitive &&) = default;
        APrimitive &operator=(APrimitive &&) = default;

        Math::Point3D getCenter() const override;
        RayTracer::IMaterial *getMaterial() const override;     // returns _material.get()
        void setMaterial(std::unique_ptr<RayTracer::IMaterial> material) override;
        void setCenter(Math::Point3D center) override;
};
```

### AMaterial
```cpp
class AMaterial : public RayTracer::IMaterial {
    private:
        std::unique_ptr<RayTracer::IMaterial> _wrapper;     // owns the wrapped material

    protected:
        int _shininess;

    public:
        AMaterial();
        AMaterial(int shininess);
        virtual ~AMaterial() = default;

        // Move-only (non-copyable) à cause du unique_ptr
        AMaterial(AMaterial &&) = default;
        AMaterial &operator=(AMaterial &&) = default;

        void setWrapper(std::unique_ptr<RayTracer::IMaterial> wrapper) override;
        RayTracer::IMaterial *getWrapper() const override;      // returns _wrapper.get()
        double getShininess() const override;
};
```

### ALight
```cpp
class ALight : public RayTracer::ILight {
    protected:
        Math::Point3D _origin;

    public:
        ALight();
        ALight(Math::Point3D origin);
        ~ALight() override = default;

        Math::Point3D getOrigin() const override;
};
```

---

## Implémentations concrètes

### Primitives
| **Classe** | **Hérite de** | **Description** |
| :--- | :--- | :--- |
| `Sphere` | `APrimitive` | Sphère définie par un centre et un rayon |
| `Plane` | `APrimitive` | Plan défini par un centre, des dimensions et une normale |
| `Cylinder` | `APrimitive` | Cylindre défini par un centre, un rayon et une hauteur |
| `Cone` | `APrimitive` | Cône défini par un sommet, un angle et une hauteur |
| `Box` | `APrimitive` | Boîte définie par un centre et des dimensions |
| `Triangle` | `APrimitive` | Triangle défini par trois sommets (Möller–Trumbore) |
| `Tetrahedron` | `APrimitive` | Tétraèdre composé de 4 faces triangulaires |
| `Pyramide` | `APrimitive` | Pyramide composée de 6 faces triangulaires |
| `Torus` | `APrimitive` | Tore défini par un grand et petit rayon |
| `MobiusStrip` | `APrimitive` | Ruban de Möbius paramétrique |
| `SierpinskiFractal` | `APrimitive` | Fractale de Sierpinski (pyramides récursives) |
| `OBJ` | `APrimitive` | Chargement de modèle 3D depuis un fichier `.obj` (avec BVH) |

### Matériaux
| **Classe** | **Hérite de** | **Description** |
| :--- | :--- | :--- |
| `ColorMaterial` | `AMaterial` | Matériau à couleur unie (Flat Color) |
| `MirrorMaterial` | `AMaterial` | Matériau réfléchissant (miroir) avec réflectivité configurable |
| `TransparencyMaterial` | `AMaterial` | Matériau transparent avec indice de transparence |

### Transforms
| **Classe** | **Hérite de** | **Description** |
| :--- | :--- | :--- |
| `TranslationTransform` | `ITransform` | Effectue une translation sur une primitive |
| `ScaleTransform` | `ITransform` | Effectue un changement d'échelle sur une primitive |
| `RotationTransform` | `ITransform` | Effectue une rotation sur une primitive |

### Lumières
| **Classe** | **Hérite de** | **Description** |
| :--- | :--- | :--- |
| `PointLight` | `ALight` | Lumière ponctuelle omnidirectionnelle |
| `DirectionalLight` | `ALight` | Lumière directionnelle (soleil) |
| `DirectionalColorLight` | `ALight` | Lumière directionnelle avec couleur |
| `SpotLight` | `ALight` | Lumière spot directionnel (lampe torche) |
| `ColorLight` | `ALight` | Lumière ponctuelle avec couleur |

### Renderers
| **Classe** | **Hérite de** | **Description** |
| :--- | :--- | :--- |
| `BaseRendering` | `IRendering` | Rendu complet : Ambient + Lambert + Phong + AO + Ombres |
| `LowRendering` | `IRendering` | Rendu simplifié (plus performant) |

---

## RayTracing (méthode de rendu)

- Détection par rayon (ray casting)
- Diffusion de la lumière selon la Loi de Lambert
- Réflexions spéculaires (modèle de Phong)
- Ombres portées (shadow rays)
- Ambient Occlusion (AO) par Monte Carlo
- Transparence & Réflexion via la chaîne de décorateurs de matériaux

---

## Guides détaillés

Pour approfondir la création de nouveaux composants et comprendre le fonctionnement du moteur, consultez les guides suivants :

- [Fonctionnement Interne](./INTERNAL_WORKS.md) ⚙️
- [Bases Mathématiques](./MATH.md) 📐
- [Design Patterns](./DESIGN_PATTERNS.md) 🏗️
- [Chargement de Modèles OBJ](./OBJ.md) 🗿
- [Création de Plugins](./PLUGINS.md) 🔌
- [Création de Primitives](./PRIMITIVES.md) 🧊
- [Configuration de Scènes](./SCENES.md) 🎬
- [Galerie de Scènes & Tests](./SCENE_GALLERY.md) 🖼️
- [Création de Lumières](./LIGHTS.md) 💡
- [Création de Matériaux](./MATERIALS.md) 🎨
- [Création de Transformations](./TRANSFORMS.md) 🔄
- [Optimisation](./OPTIMIZATION.md) ⚡
