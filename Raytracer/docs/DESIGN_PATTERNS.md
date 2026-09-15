# Design Patterns 🏗️

Ce document décrit les design patterns utilisés dans le projet RayTracer, leur implémentation et leurs interactions.

---

## 1. Factory Pattern (Plugin Factory) 🏭

### Objectif
Permettre la création d'objets polymorphiques (`IPrimitive`, `IMaterial`, `ILight`, `ITransform`, `IRendering`) sans que le code appelant connaisse les classes concrètes. Les implémentations sont chargées dynamiquement depuis des plugins (`.so`).

### Architecture

```
┌──────────────┐          ┌──────────────────┐          ┌──────────────┐
│ ConfigParser │──────────▶│   SceneFactory    │──────────▶│   DlLoader   │
│              │  appelle  │                  │  charge   │              │
│              │  create*()│                  │  les .so  │  dlopen()    │
└──────────────┘          └──────────────────┘          └──────────────┘
                                   │
                                   │ retourne unique_ptr<T>
                                   ▼
                          ┌──────────────────┐
                          │  Plugin (.so)     │
                          │                  │
                          │  extern "C" {    │
                          │    getName()     │
                          │    createT()     │
                          │    destroyT()    │
                          │  }               │
                          └──────────────────┘
```

### Flux de création

1. **Chargement** : Au démarrage, `SceneFactory` scanne le répertoire `plugins/` et charge chaque `.so` via `DlLoader`
2. **Enregistrement** : Chaque plugin expose `getName()` qui retourne son identifiant (ex: `"spheres"`, `"color"`, `"point"`)
3. **Instanciation** : Quand `ConfigParser` appelle `createPrimitive("spheres", setting)`, la factory :
   - Appelle la fonction `createPrimitive()` du plugin
   - Le plugin fait `new Sphere(...)` et retourne un raw pointer
   - La factory le wrap dans un `std::unique_ptr<IPrimitive>` et le retourne

### Implémentation : `SceneFactory`

```cpp
class SceneFactory {
public:
    // Chaque méthode retourne un unique_ptr qui possède l'objet
    std::unique_ptr<RayTracer::IPrimitive> createPrimitive(std::string type, const libconfig::Setting &setting);
    std::unique_ptr<RayTracer::IMaterial>  createMaterial(std::string type, const libconfig::Setting &setting);
    std::unique_ptr<RayTracer::ILight>     createLight(std::string type, const libconfig::Setting &setting);
    std::unique_ptr<RayTracer::ITransform> createTransform(std::string type, const libconfig::Setting &setting);
    std::unique_ptr<RayTracer::IRendering> createRenderer(std::string type, const libconfig::Setting &setting);

private:
    // Stocke les pointeurs de fonctions créer/détruire de chaque plugin
    std::map<std::string, std::pair<
        RayTracer::IPrimitive *(*)(const libconfig::Setting &),
        void (*)(RayTracer::IPrimitive *)
    >> _pluginsPrimitives;
    // ... idem pour Light, Material, Transform, Rendering
};
```

### Côté Plugin

Chaque plugin exporte les fonctions standardisées via `extern "C"` :

```cpp
extern "C" {
    const char *getName(void) {
        return "spheres";           // identifiant dans le .cfg
    }

    RayTracer::IPrimitive *createPrimitive(const libconfig::Setting &setting) {
        // Parse la config et crée l'objet
        return new RayTracer::Sphere(center, radius);
    }

    void destroyPrimitive(RayTracer::IPrimitive *primitive) {
        delete primitive;
    }
}
```

### Côté appelant (ConfigParser)

```cpp
// Plus besoin de manipuler des raw pointers !
auto primitive = _factory.createPrimitive(type, setting);      // unique_ptr
primitive->setMaterial(_factory.createMaterial(matType, matSetting));
_objects.push_back(std::move(primitive));
```

### Avantages
- **Extensibilité** : Ajouter un nouveau type = créer un `.so`, sans recompiler le core
- **Découplage** : Le core ne connaît que les interfaces, pas les implémentations
- **Sécurité mémoire** : Ownership clair via `unique_ptr` dès la sortie de la factory

---

## 2. Decorator Pattern (Material Chain) 🎨

### Objectif
Permettre de combiner dynamiquement plusieurs comportements de matériaux sur un même objet (ex: un objet peut être coloré **ET** réfléchissant **ET** transparent) sans créer une classe pour chaque combinaison.

### Architecture

Chaque `IMaterial` peut envelopper un autre `IMaterial` via la relation `wrapper`. La chaîne se lit de l'extérieur vers l'intérieur :

```
Primitive
  └── material: MirrorMaterial (reflectivity = 0.8)
        └── wrapper: TransparencyMaterial (transparency = 0.5)
              └── wrapper: ColorMaterial (color = {255, 0, 0})
                    └── wrapper: nullptr (fin de chaîne)
```

### Flux d'exécution de `getRendering()`

```
MirrorMaterial::getRendering()
│
├── 1. Appelle getWrapper()->getRendering()
│   │
│   └── TransparencyMaterial::getRendering()
│       │
│       ├── 1. Appelle getWrapper()->getRendering()
│       │   │
│       │   └── ColorMaterial::getRendering()
│       │       │
│       │       └── Retourne la couleur de base (ex: rouge)
│       │
│       └── 2. Blend couleur + objets derrière × transparence
│
└── 2. Blend résultat + couleur réfléchie × réflectivité
```

### Implémentation : `AMaterial`

```cpp
class AMaterial : public RayTracer::IMaterial {
    private:
        std::unique_ptr<RayTracer::IMaterial> _wrapper;     // possède le matériau enveloppé

    public:
        void setWrapper(std::unique_ptr<RayTracer::IMaterial> wrapper) override {
            _wrapper = std::move(wrapper);      // transfert d'ownership
        }

        RayTracer::IMaterial *getWrapper() const override {
            return _wrapper.get();              // accès non-owning
        }
};
```

### Construction de la chaîne (ConfigParser)

La chaîne est construite **de l'intérieur vers l'extérieur** :

```cpp
std::unique_ptr<RayTracer::IMaterial> ConfigParser::parseMaterials(const libconfig::Setting &setting) {
    // 1. Commencer par le matériau de base (couleur)
    std::unique_ptr<RayTracer::IMaterial> current = std::make_unique<ColorMaterial>(BASE_COLOR, 128);

    // 2. Envelopper avec chaque matériau supplémentaire
    for (int i = 0; i < setting.getLength(); i++) {
        auto material = _factory.createMaterial(type, setting);
        material->setWrapper(std::move(current));   // le nouveau enveloppe l'ancien
        current = std::move(material);              // le nouveau devient le courant
    }
    return current;     // retourne le matériau le plus externe
}
```

### Exemple concret

Configuration :
```libconfig
material = {
    color = { r = 255; g = 0; b = 0; };
    reflectivity = 0.8;
    transparency = 0.5;
};
```

Chaîne résultante :
```
TransparencyMaterial(0.5)
  └── MirrorMaterial(0.8)
        └── ColorMaterial({255,0,0})
```

### Pattern appliqué aux ombres

Le pattern Decorator s'applique aussi aux calculs d'ombres. Chaque matériau dans la chaîne peut modifier l'intensité et la couleur de l'ombre :

| Matériau | `getShadowIntensity()` | `getShadowColor()` |
| :--- | :--- | :--- |
| `ColorMaterial` | `1.0` (opaque) | Couleur sombre |
| `MirrorMaterial` | Délègue au wrapper | Délègue au wrapper |
| `TransparencyMaterial` | `1.0 - transparency` (réduit l'ombre) | Blend couleur + blanc |

### Avantages
- **Composition libre** : N'importe quelle combinaison de matériaux sans explosion de classes
- **Responsabilité unique** : Chaque matériau gère un seul aspect (couleur, reflet, transparence)
- **Extensibilité** : Ajouter un nouveau comportement = créer un nouveau matériau-décorateur
- **Ownership sûr** : La chaîne est possédée via `unique_ptr`, détruite automatiquement

---

## 3. Composite Pattern (Primitives composites) 🧱

### Objectif
Permettre à des primitives complexes d'être composées de sous-primitives plus simples.

### Exemples

| **Composite** | **Composant** | **Description** |
| :--- | :--- | :--- |
| `Tetrahedron` | `Triangle` × 4 | Tétraèdre = 4 faces triangulaires |
| `Pyramide` | `Triangle` × 6 | Pyramide = 6 faces triangulaires |
| `SierpinskiFractal` | `Pyramide` × N | Fractale récursive de pyramides |
| `OBJ` | `Triangle` × N | Modèle 3D chargé depuis un fichier `.obj` |

Chaque composite délègue `hits()` et `getNormal()` à ses sous-primitives et retourne le résultat le plus proche.

---

## Résumé des patterns

| **Pattern** | **Où** | **Pourquoi** |
| :--- | :--- | :--- |
| **Factory** | `SceneFactory` + plugins | Instanciation dynamique d'objets polymorphiques |
| **Decorator** | `IMaterial` / `AMaterial` | Combinaison libre de comportements de matériaux |
| **Composite** | `Tetrahedron`, `Pyramide`, `OBJ` | Primitives complexes = composition de simples |
