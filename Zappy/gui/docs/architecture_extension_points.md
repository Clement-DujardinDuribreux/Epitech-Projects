# Points d'extension GUI

Cette page résume les zones à modifier selon le type de changement.

## Ajouter une donnée reçue du serveur

Modifier:

1. `gui/src/client/Client.hpp`: nom de commande, entrée `_commandMap`, déclaration du handler.
2. `gui/src/client/Client.cpp`: parsing et mise à jour du modèle.
3. `gui/src/Map.hpp`, `Player.hpp`, `Cell.hpp` ou `Egg.hpp`: stockage de la donnée.
4. `gui/src/Map.hpp` ou `Ath.hpp`: rendu si la donnée est visible.
5. `tests/gui_unit.cpp`: scénario protocole et assertion sur l'état.

## Ajouter une action utilisateur GUI

Modifier:

1. `GUI::_handleEvent` pour l'événement clavier/souris.
2. `Client` si l'action envoie une commande au serveur.
3. `Map` ou `Ath` si l'action change l'affichage local.
4. Les tests si la logique est isolable sans fenêtre SFML ouverte.

La boucle principale garde cette séparation: `GUI` capte l'entrée utilisateur, `Client` parle au serveur, `Map` stocke l'état, `Ath` affiche les panneaux.

## Ajouter une requête périodique

Les méthodes `request...` de `Client` envoient des commandes GUI au serveur. `sendMsg` limite les envois avec `_SEND_INTERVAL_MS`.

Si une nouvelle requête doit être appelée régulièrement, vérifier que:

- elle ne spamme pas le serveur;
- elle ne duplique pas une information déjà envoyée par événement;
- elle ne dépend pas d'une map non initialisée;
- le handler de réponse existe.

## Ajouter un écran ou panneau HUD

Modifier `Ath` pour les panneaux d'interface.

Lire les données depuis `Map`, pas depuis `Client`, pour garder le rendu indépendant du réseau. Si le panneau dépend d'une donnée qui n'est pas encore stockée, ajouter cette donnée dans `Map` ou dans l'entité concernée.

## Ajouter une animation simple

Pour une animation liée à un joueur:

- stocker l'état dans `Player`;
- le déclencher dans le handler protocole;
- le lire dans `Map::drawPlayers` ou `Player::getSprite`.

Pour une animation indépendante:

- créer une petite structure avec position, temps de début et durée;
- stocker la liste dans `Map`;
- nettoyer les éléments terminés pendant le rendu ou lors d'une mise à jour dédiée.

Le code actuel n'a pas de système d'animation central. Garder les animations courtes et localisées évite de transformer `Map` en moteur de jeu.

## Ajouter un asset

Modifier uniquement `AssetsManager::loadGameAssets` pour charger l'asset. Les autres classes ne doivent récupérer les textures que par `AssetsManager::getTexture`.

Si une texture est utilisée dans plusieurs composants, garder une seule clé partagée.

## Vérifications avant merge

- Le serveur peut envoyer la nouvelle ligne sans casser une ancienne GUI.
- La GUI ignore une ligne invalide ou incomplète.
- Le rendu ne dépend pas d'une texture non chargée.
- Le test C++ couvre au moins le parsing ou la mutation de modèle.
- Doxygen génère sans erreur avec `sh docs/generate_doxygen.sh`.
