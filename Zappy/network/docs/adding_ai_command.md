# Ajouter une commande AI serveur

Cette page décrit le chemin à suivre pour ajouter une commande envoyée par une IA, par exemple `Dance`, dans la structure actuelle du serveur.

## Où passe une commande

Une ligne reçue par le serveur suit ce chemin:

1. `Client::readFromClient` ajoute les octets dans le buffer du client.
2. `Server::processClientCommands` découpe le buffer ligne par ligne.
3. `CommandFactory::createCommand` transforme le nom de commande en objet.
4. `Timeline::addAction` planifie l'exécution avec le coût en cycles.
5. `Server::executeTimeline` appelle `ICommand::exec`.
6. La commande modifie `Game`, `Player`, `Inventory` ou répond au client avec `Server::sendToClient`.

Une commande AI doit donc être représentée par une classe dans `network/include/commands`.

## Créer la classe

Créer un header dans `network/include/commands`, par exemple `DanceCommand.hpp`.

La classe doit hériter de `ACommand`. Le nombre passé au constructeur de `ACommand` est le coût en cycles.

```cpp
#ifndef DANCE_COMMAND
    #define DANCE_COMMAND

    #include "commands/ACommand.hpp"
    #include "Server.hpp"

class DanceCommand : public ACommand {
    public:
        DanceCommand() : ACommand(7) {}

        void exec(std::string args, Server &server, int pollIndex,
            Game &game) override {
            (void)args;
            (void)game;
            server.sendToClient(pollIndex, "ok\n");
        }
};

#endif
```

Règles à respecter:

- Répondre avec `ok\n`, `ko\n` ou une réponse protocole valide.
- Utiliser `pollIndex` pour retrouver le client source dans `server.getPoll().getFds()`.
- Modifier le monde par `Game`, `Player` ou `Inventory`, pas par une copie.
- Appeler les notifications graphiques si l'action doit être visible dans la GUI.
- Garder la classe courte; si la logique grossit, déplacer les calculs dans une fonction privée.

## Enregistrer la commande

Modifier `network/src/commands/CommandFactory.cpp`:

1. Ajouter l'include du nouveau header.
2. Ajouter une condition dans `CommandFactory::createCommand`.

```cpp
#include "commands/DanceCommand.hpp"

if (command == "Dance")
    return std::make_unique<DanceCommand>();
```

Sans cette entrée, le serveur répondra `ko` à l'IA.

## Ajouter un événement graphique si nécessaire

Si la commande change un état visible, ajouter une notification dans `Server` ou réutiliser une notification existante.

Exemples existants:

- `notifyPlayerPosition` après `Forward`, `Left`, `Right`.
- `notifyResourceCollect` après `Take`.
- `notifyResourceDrop` après `Set`.
- `notifyPlayerBroadcast` après `Broadcast`.
- `notifyPlayerFork` et `notifyEggPutGround` après `Fork`.

Une notification GUI doit envoyer une ligne conforme au protocole graphique avec `queueGraphicsEvent`. Le passage par la timeline évite de modifier les buffers pendant l'exécution d'une autre action.

## Adapter la GUI

Si le nouveau message graphique n'existe pas encore côté GUI, ajouter le handler dans `gui/src/client/Client.hpp` et `gui/src/client/Client.cpp`.

Le serveur peut fonctionner sans ce handler, mais l'événement ne sera pas visible.

## Ajouter les tests

Ajouter au minimum:

- un test unitaire dans `tests/network_unit.cpp` si la commande modifie une règle isolée;
- un test fonctionnel dans `tests/network_functional.cpp` si le protocole TCP change.

Pour une commande planifiée, le test fonctionnel doit démarrer `zappy_server`, connecter une IA, envoyer la commande et lire la réponse attendue.

Lancer:

```sh
make -C network tests_run
```

## Checklist

- Header de commande ajouté dans `network/include/commands`.
- Commande enregistrée dans `CommandFactory.cpp`.
- Coût en cycles correct dans le constructeur.
- Réponse AI valide.
- Notification GUI ajoutée si l'action est visible.
- Handler GUI ajouté si un nouveau message graphique est émis.
- Test unitaire ou fonctionnel ajouté.
- Documentation Doxygen ajoutée sur la nouvelle classe.
