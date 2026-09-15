# Ajouter un handler de protocole GUI

Cette page explique comment afficher côté GUI une nouvelle ligne envoyée par le serveur.

## Flux actuel

Le client GUI lit le socket dans `Client::update`, stocke les octets dans `_buffer`, puis `Client::_extractCommands` découpe les lignes terminées par `\n`.

Chaque ligne est envoyée à `Client::_parseCommand`, qui:

1. découpe la ligne en tokens séparés par espaces;
2. lit `args[0]` comme nom de commande;
3. cherche le handler dans `_commandMap`;
4. appelle la méthode `_handle...`.

Tout handler doit donc être déclaré dans `gui/src/client/Client.hpp`, enregistré dans `_commandMap`, puis implémenté dans `gui/src/client/Client.cpp`.

## Ajouter le nom de commande

Dans `gui/src/client/Client.hpp`, ajouter un `#define` près des autres commandes.

```cpp
#define CMD_PLAYER_DANCE "pda"
```

Utiliser le nom exact envoyé par le serveur.

## Ajouter l'entrée dans la map

Toujours dans `Client.hpp`, ajouter l'entrée `_commandMap`.

```cpp
{ CMD_PLAYER_DANCE, [this](const std::vector<std::string> &a){ _handlePlayerDance(a); } },
```

## Déclarer le handler

Ajouter la méthode privée dans `Client.hpp`.

```cpp
void _handlePlayerDance(const std::vector<std::string> &args);
```

## Implémenter le handler

Dans `gui/src/client/Client.cpp`, valider le nombre d'arguments avant de modifier la map.

```cpp
void Client::_handlePlayerDance(const std::vector<std::string> &args)
{
    if (args.size() != 2)
        return;
    int id = parseInt(args[1]);
    for (auto &player : _map.getPlayers()) {
        if (player.getId() == id) {
            player.setDancing(true);
            break;
        }
    }
}
```

Les handlers actuels ignorent les commandes invalides au lieu de lever une erreur. Garder ce comportement évite de casser l'affichage quand une trame est partielle ou inattendue.

## Modifier le modèle graphique

Si l'événement ajoute un état temporaire à un joueur:

1. ajouter un booléen ou une donnée dans `gui/src/Player.hpp`;
2. ajouter getter et setter;
3. utiliser cet état dans `Player::getSprite` ou `Map::drawPlayers`.

Si l'événement ajoute une entité indépendante:

1. créer une classe proche de `Egg`;
2. ajouter un `std::vector` dans `Map`;
3. ajouter getter;
4. ajouter une méthode de rendu dans `Map`;
5. appeler cette méthode depuis `Map::draw`.

## Ajouter une requête GUI vers le serveur

Si la GUI doit aussi demander cette information, ajouter une méthode publique dans `Client`.

Dans `Client.hpp`:

```cpp
void requestPlayerDance(int id);
```

Dans `Client.cpp`:

```cpp
void Client::requestPlayerDance(int id)
{
    sendMsg("pda " + std::to_string(id) + "\n");
}
```

## Tests

Ajouter ou compléter `tests/gui_unit.cpp`.

Pour tester un handler, utiliser le scénario client existant: un serveur TCP local envoie une ligne protocole, puis le test vérifie l'état de `Client::getMap`.

Points à vérifier:

- mauvais nombre d'arguments ignoré;
- id avec préfixe `#` accepté si le serveur l'envoie;
- état `Map`, `Player`, `Egg` ou HUD correctement modifié;
- aucune exception si la commande arrive avant certaines données optionnelles.
