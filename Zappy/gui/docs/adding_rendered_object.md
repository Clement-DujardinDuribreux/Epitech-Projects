# Ajouter un objet rendu dans la GUI

Un objet rendu est une entité visible sur la carte ou dans l'interface. Il peut venir du protocole serveur, d'un état local ou d'une agrégation déjà présente dans `Map`.

## Choisir le bon emplacement

- Objet posé sur une case: ajouter l'état dans `Map`, comme les `Egg`.
- État d'un joueur: ajouter la donnée dans `Player`.
- Ressource d'une case: modifier `Cell`.
- Élément d'interface fixe: modifier `Ath`.
- Image ou police: modifier `AssetsManager`.

## Ajouter un asset

Placer l'image dans `gui/assets/img` ou la police dans `gui/assets/fonts`.

Dans `gui/src/AssetsManager.hpp`, ajouter le chargement dans `loadGameAssets`.

Pour une image entière:

```cpp
loadTexture("crystal", "gui/assets/img/crystal.png");
```

Pour une partie d'une spritesheet:

```cpp
loadTextureWithRect("crystal", "gui/assets/img/items.png",
    sf::IntRect({0, 0}, {64, 64}));
```

La clé passée à `loadTexture` est celle utilisée ensuite avec `AssetsManager::getTexture`.

## Ajouter une entité de carte

Créer une classe si l'objet a un état propre. `gui/src/Egg.hpp` est le modèle le plus simple:

- id;
- position;
- constructeur;
- getters;
- `getSprite`.

Ajouter ensuite dans `Map`:

```cpp
std::vector<Crystal> _crystals;
std::vector<Crystal> &getCrystals();
void drawCrystals(sf::RenderWindow &window) const;
```

Appeler le rendu depuis `Map::draw`, dans l'ordre visuel voulu:

```cpp
drawChests(window);
drawCrystals(window);
drawEggs(window);
drawPlayers(window);
drawBroadcast(window);
```

Les positions doivent passer par `Map::getTileScreenPosition` pour rester alignées avec la grille et la caméra.

## Ajouter un état visuel à un joueur

Modifier `gui/src/Player.hpp`:

1. ajouter le champ;
2. ajouter un setter;
3. ajouter un getter si le rendu en a besoin;
4. adapter `getSprite` ou le rendu dans `Map::drawPlayers`.

Les états existants `expelling`, `forking`, `taking`, `dropping` et `incanting` montrent le pattern actuel.

## Ajouter une donnée dans l'ATH

Modifier `gui/src/Ath.hpp`.

Les fonctions de `Ath` reçoivent déjà `Map` ou des agrégations de ressources. Si la donnée est globale, ajouter une méthode de calcul dans `Map`. Si la donnée dépend d'une case, utiliser `Map::getCell` et `Map::getPlayersOn`.

Éviter de faire du parsing réseau dans `Ath`: le réseau met à jour `Map`, puis `Ath` lit `Map`.

## Ajouter une ressource affichée

Pour une nouvelle ressource, modifier en cohérence:

- `Cell`: taille du vecteur et rendu de la ressource;
- `Player`: taille de l'inventaire;
- `Client::_handleTileContent`;
- `Client::_handlePlayerInv`;
- `Map::getResources`;
- `Map::getTeamsResources`;
- `Ath`: icône, position et texte;
- `AssetsManager`: texture.

La GUI utilise l'ordre du protocole. Ne pas changer seulement l'affichage: il faut que le serveur envoie la même position dans `bct` et `pin`.

## Tests

Ajouter un test dans `tests/gui_unit.cpp`.

Exemples utiles:

- l'objet est créé après réception de l'événement protocole;
- l'objet est supprimé après réception de l'événement de suppression;
- l'agrégation de ressources inclut la nouvelle donnée;
- `Map::getCellPos` et `Map::getTileScreenPosition` restent cohérents;
- le handler ignore une commande invalide sans casser l'état existant.
