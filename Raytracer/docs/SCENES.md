# Guide: Scene Configuration (.cfg) 🎬

Scenes are defined using the `libconfig` format. A typical scene file consists of three main sections: `camera`, `primitives`, and `lights`.

## 📷 Camera
Configures the "eye" through which the scene is viewed.

```libconfig
camera: {
    resolution = { width = 1920; height = 1080; };
    position = { x = 0; y = 20; z = -100; };
    rotation = { x = 0; y = 0; z = 0; };
    fieldOfView = 72.0; # In degrees (must be between 0 and 180)
    renderer = "low-renderer"; # Optional: plugin name for custom renderer
};
```

| Attribut | Type | Requis | Description |
| :--- | :--- | :--- | :--- |
| `resolution` | `{width, height}` | Non | Résolution de l'image (défaut: 1920×1080) |
| `position` | `{x, y, z}` | Non | Position de la caméra (défaut: 0,0,0) |
| `rotation` | `{x, y, z}` | Non | Rotation de la caméra |
| `fieldOfView` | `double` | Non | Champ de vision en degrés (défaut: 90.0) |
| `renderer` | `string` | Non | Nom du plugin de rendu (défaut: `BaseRendering`) |

---

## 🧊 Primitives
A list of objects in the scene. Each group is named after the plugin's `getName()` return value.

```libconfig
primitives: {
    spheres = (
        { 
            center = { x = 0; y = 0; z = 0; };
            radius = 25; 
            material = { 
                color = { r = 255; g = 0; b = 0; }; 
                shininess = 128;
            };
            transforms = {
                translation = { translation = { x = 10; y = 0; z = 0; } };
                rotation = { rotation = { x = 0; y = 45; z = 0; } };
                scale = { scale = { x = 2; y = 2; z = 2; } };
            };
        }
    );
    planes = ( ... );
    cylinders = ( ... );
};
```

### Common Primitive Attributes
| Attribut | Type | Requis | Description |
| :--- | :--- | :--- | :--- |
| `center` | `{x, y, z}` | Oui | Position du centre de l'objet |
| `material` | `group` | Non | Matériau de surface (défaut: blanc) |
| `transforms` | `group` | Non | Transformations appliquées à l'objet |

### Primitives disponibles

| Plugin | `getName()` | Attributs spécifiques |
| :--- | :--- | :--- |
| Sphere | `spheres` | `radius` |
| Plane | `planes` | `length`, `width`, `normal = {x,y,z}` |
| Cylinder | `cylinders` | `radius`, `height` |
| Cone | `cones` | `angle`, `height` |
| Box | `boxes` | `size = {x,y,z}` |
| Triangle | `triangles` | `a`, `b`, `c` (3 sommets `{x,y,z}`) |
| Torus | `torus` | `majorRadius`, `minorRadius` |
| OBJ | `obj` | `path` (chemin du fichier `.obj`) |

---

## 🎨 Materials
Les matériaux définissent l'apparence de la surface. Ils se combinent via le **pattern Decorator** : chaque attribut de matériau est empilé en couche.

```libconfig
material = {
    color = { r = 255; g = 0; b = 0; };    # Base: couleur rouge
    shininess = 64;                          # Brillance spéculaire (défaut: 128)
    reflectivity = 0.8;                      # Ajout: reflet miroir 80%
    transparency = 0.5;                      # Ajout: transparence 50%
};
```

### Matériaux disponibles

| Attribut dans le `.cfg` | Plugin | Description |
| :--- | :--- | :--- |
| `color = {r, g, b}` | `ColorMaterial` | Couleur unie de base |
| `reflectivity = <double>` | `MirrorMaterial` | Réflexion miroir (0.0 à 1.0) |
| `transparency = <double>` | `TransparencyMaterial` | Transparence (0.0 = opaque, 1.0 = invisible) |

### Chaîne de décorateurs
Les matériaux se combinent automatiquement. L'ordre de la chaîne dépend de l'ordre des clés dans la section `material` du fichier `.cfg` :

```
transparency → reflectivity → color (base)
```

Chaque matériau appelle `getWrapper()->getRendering()` pour obtenir la couleur de la couche inférieure, puis applique son propre effet.

---

## 💡 Lights
Configures global and local illumination.

```libconfig
lights: {
    ambient = 0.4;      # Multiplicateur de lumière ambiante (0.0 à 1.0)
    diffuse = 0.6;      # Multiplicateur de lumière diffuse (0.0 à 1.0)

    # Ambient Occlusion
    AO = {
        samples = 64;       # Nombre de rayons de sondage
        radius = 10.0;      # Rayon de détection (unités de la scène)
        intensity = 0.8;    # Intensité de l'assombrissement (0.0 à 1.0)
    };

    point = (
        {
            origin = { x = 50; y = 80; z = -10; };
        }
    );
    
    directional = ( ... );
    spot = ( ... );
};
```

### Paramètres globaux

| Attribut | Type | Requis | Description |
| :--- | :--- | :--- | :--- |
| `ambient` | `double` | Non | Multiplicateur de lumière ambiante (défaut: 0.0) |
| `diffuse` | `double` | Non | Multiplicateur de lumière diffuse (défaut: 0.0) |

### Lumières disponibles

| Plugin | `getName()` | Attributs | Description |
| :--- | :--- | :--- | :--- |
| PointLight | `point` | `origin = {x,y,z}` | Lumière ponctuelle blanche |
| ColorLight | `color_light` | `origin`, `color = {r,g,b}` | Lumière ponctuelle avec couleur |
| DirectionalLight | `directional` | `direction = {x,y,z}` | Lumière directionnelle (soleil) |
| DirectionalColorLight | `directional_color` | `direction`, `color` | Lumière directionnelle avec couleur |
| SpotLight | `spot` | `origin`, `direction`, `angle` | Spot lumineux (lampe torche) |

---

## 🌑 Ambient Occlusion (AO)

L'Ambient Occlusion simule l'assombrissement naturel dans les recoins et les zones confinées. Elle est configurée dans la section `lights` :

```libconfig
lights: {
    AO = {
        samples = 64;
        radius = 10.0;
        intensity = 0.8;
    };
};
```

### Paramètres

| Attribut | Type | Défaut | Description |
| :--- | :--- | :--- | :--- |
| `samples` | `int` | `0` (désactivé) | Nombre de rayons aléatoires envoyés depuis chaque point d'intersection. Plus le nombre est élevé, plus le résultat est lisse (mais plus lent). |
| `radius` | `double` | `0.0` | Distance maximale de détection d'occlusion (en unités de la scène). Seuls les objets dans ce rayon contribuent à l'assombrissement. |
| `intensity` | `double` | `0.0` | Force de l'assombrissement (0.0 = aucun effet, 1.0 = occlusion complète). |

### Fonctionnement

1. Pour chaque point d'intersection, `samples` rayons sont envoyés dans des directions aléatoires dans l'hémisphère autour de la normale
2. Si un rayon touche un objet à une distance inférieure à `radius`, le point est assombri
3. L'assombrissement est proportionnel à `intensity / samples` par rayon bloqué

### Recommandations

| Qualité | `samples` | `radius` | `intensity` | Temps |
| :--- | :--- | :--- | :--- | :--- |
| Rapide (preview) | 8-16 | 5.0 | 0.5 | ⚡ |
| Moyen | 32-64 | 10.0 | 0.8 | ⏱️ |
| Haute qualité | 128-256 | 15.0 | 0.8 | 🐢 |

> **Note** : Mettre `samples = 0` ou ne pas inclure la section `AO` désactive complètement l'Ambient Occlusion.

---

## 🔄 Transforms
Transformations applied to primitives after creation.

```libconfig
transforms = {
    translation = { translation = { x = 10; y = 0; z = 0; } };
    rotation = { rotation = { x = 0; y = 45; z = 0; } };
    scale = { scale = { x = 2; y = 1; z = 2; } };
};
```

| Plugin | `getName()` | Attribut | Description |
| :--- | :--- | :--- | :--- |
| TranslationTransform | `translation` | `translation = {x,y,z}` | Déplace l'objet |
| RotationTransform | `rotation` | `rotation = {x,y,z}` | Rotation en degrés |
| ScaleTransform | `scale` | `scale = {x,y,z}` | Changement d'échelle |

---

## 📄 Exemple complet

```libconfig
camera: {
    resolution = { width = 1920; height = 1080; };
    position = { x = 0; y = 2; z = 0; };
    fieldOfView = 72.0;
};

primitives: {
    spheres = (
        { 
            center = { x = -6; y = 5; z = 8; };
            radius = 2; 
            material = { 
                color = { r = 0; g = 255; b = 0; }; 
                reflectivity = 0.8;
            };
        },
        { 
            center = { x = 0; y = 1.5; z = 6; };
            radius = 1.5; 
            material = { 
                color = { r = 255; g = 0; b = 0; }; 
                transparency = 0.8; 
            };
        }
    );

    planes = (
        { 
            center = { x = 0; y = -2; z = 0; };
            length = 10000.0; width = 10000.0;
            normal = { x = 0; y = 1; z = 0; };
            material = { color = { r = 255; g = 255; b = 0; }; }; 
        }
    );
};

lights: {
    ambient = 0.4;
    diffuse = 0.6;

    AO = {
        radius = 10.0;
        samples = 64;
        intensity = 0.8;
    };

    point = (
        { origin = { x = -2; y = 10; z = 5; }; },
        { origin = { x = 4; y = 12; z = 1; }; }
    );
};
```

---

## 🚀 Pro Tip: Dynamic Loading
If you add a new plugin `plugins/CustomObj.so` with `getName()` returning `"custom"`, you can immediately start using it in your `.cfg`:

```libconfig
primitives: {
    custom = ( { ... } );
};
```
