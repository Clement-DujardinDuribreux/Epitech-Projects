# Ajouter une ressource ou un objet serveur

Le projet utilise actuellement sept ressources fixes: `food`, `linemate`, `deraumere`, `sibur`, `mendiane`, `phiras`, `thystame`.

Ajouter une ressource change le protocole, les inventaires, le spawn serveur, les commandes AI, la GUI et les tests. Ce n'est pas un changement local.

## Fichiers serveur à modifier

1. `network/include/game/Ressources.hpp`
   Ajouter la valeur dans l'enum.

2. `network/src/game/Inventory.cpp`
   Initialiser la nouvelle ressource dans le constructeur.

3. `Inventory::toString`
   Ajouter le nom et la quantité dans la réponse AI `Inventory`.

4. `Server::parseResource`
   Ajouter le nom protocole dans le tableau `names`.

5. `Server::formatTile`
   Étendre la boucle ou l'ordre de sérialisation si la ressource doit apparaître dans `bct`.

6. `Server::formatPlayerInventory`
   Étendre la réponse `pin` si l'inventaire GUI doit la connaître.

7. `Game::respawnResources`
   Ajouter la densité de spawn dans le tableau `densities`.

8. `LookCommand`
   Ajouter le nom dans `resourceNames` pour la réponse AI `Look`.

9. `TakeCommand` et `SetCommand`
   Ces commandes fonctionnent via `Server::parseResource`; elles acceptent la ressource dès que le parsing et les inventaires sont mis à jour.

## Fichiers GUI à modifier

1. `gui/src/Cell.hpp`
   La taille du vecteur `_resources` est actuellement `7`. Elle doit changer.

2. `gui/src/Player.hpp`
   L'inventaire joueur suit aussi l'ordre des ressources du protocole.

3. `gui/src/client/Client.cpp`
   `_handleTileContent` et `_handlePlayerInv` attendent un nombre fixe d'arguments.

4. `gui/src/AssetsManager.hpp`
   Ajouter la texture si la ressource a une icône.

5. `gui/src/Ath.hpp`
   Ajouter l'affichage dans les panneaux de ressources ou d'équipe.

6. `gui/src/Map.hpp`
   Adapter les agrégations si elles supposent sept ressources.

## Ordre protocole

L'ordre doit rester identique partout:

1. enum `Ressources`;
2. inventaire serveur;
3. `bct`;
4. `pin`;
5. `Look`;
6. vecteurs de ressources GUI;
7. rendu HUD.

Une incohérence d'ordre crée des bugs difficiles à voir: la GUI affichera une quantité valide mais sous la mauvaise icône.

## Ajouter un objet non collectable

Si l'objet n'est pas une ressource collectable par `Take` et `Set`, ne pas l'ajouter dans `Ressources`.

Créer plutôt un stockage dédié dans `Game`, par exemple un vecteur de structures comme `GameEgg`. Ensuite:

- ajouter les méthodes de création, recherche et suppression dans `Game`;
- ajouter des notifications `Server::notify...`;
- ajouter des handlers GUI;
- ajouter un rendu dans `Map`;
- ajouter des tests fonctionnels.

Les œufs sont le modèle existant à suivre: `GameEgg`, `Game::addEgg`, `Game::removeEgg`, `notifyEggPutGround`, `notifyEggConnection`, `notifyEggDeath`, puis `Egg` côté GUI.

## Tests à prévoir

- Test unitaire `Inventory`: ajout, retrait, format string.
- Test unitaire `Game`: spawn ou stockage du nouvel objet.
- Test fonctionnel GUI/protocole: réponse `bct`, `pin` ou nouvel événement.
- Test GUI si un nouveau rendu ou handler est ajouté.
