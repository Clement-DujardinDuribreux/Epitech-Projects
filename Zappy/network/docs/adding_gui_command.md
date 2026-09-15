# Ajouter une commande graphique serveur

Une commande graphique est une requête envoyée par le client GUI au serveur. Elle ne passe pas par la file d'actions AI classique: elle est exécutée immédiatement par `Server::executeGuiCommand`.

Exemples actuels: `msz`, `bct`, `mct`, `tna`, `ppo`, `plv`, `pin`, `sgt`, `sst`.

## Flux actuel

1. Le client GUI envoie une ligne, par exemple `ppo #3`.
2. `Server::processClientCommands` sépare le nom de commande et ses arguments.
3. `CommandFactory::createCommand` retourne un `GuiCommand`.
4. `GuiCommand::exec` appelle `Server::executeGuiCommand`.
5. `Server::executeGuiCommand` valide les paramètres et répond avec une ligne protocole.

Une commande graphique doit être déclarée à deux endroits: dans la factory et dans la liste `guiCommand` de `Server::processClientCommands`.

## Ajouter le nom dans la factory

Modifier `network/src/commands/CommandFactory.cpp`.

Ajouter le nom dans le bloc qui crée `GuiCommand`:

```cpp
if (command == "msz" || command == "bct" || command == "mct"
    || command == "tna" || command == "ppo" || command == "plv"
    || command == "pin" || command == "sgt" || command == "sst"
    || command == "newcmd")
    return std::make_unique<GuiCommand>(command);
```

## Autoriser la commande côté serveur

Modifier `Server::processClientCommands` dans `network/src/Server.cpp`.

Ajouter le nom dans le booléen `guiCommand`. Cette étape est importante: elle empêche une IA d'appeler une commande GUI, et empêche un client GUI d'appeler une commande AI.

```cpp
bool guiCommand = commandName == "msz" || commandName == "bct"
    || commandName == "mct" || commandName == "tna"
    || commandName == "ppo" || commandName == "plv"
    || commandName == "pin" || commandName == "sgt"
    || commandName == "sst" || commandName == "newcmd";
```

## Implémenter la réponse

Ajouter un bloc dans `Server::executeGuiCommand`.

Les règles de réponse actuelles:

- Paramètres invalides: `sbp\n`.
- Commande inconnue dans la factory: `suc\n`.
- Réponse correcte: une ligne protocole claire, terminée par `\n`.

Exemple:

```cpp
if (command == "newcmd") {
    if (input >> extra) {
        sendToClient(pollIndex, "sbp\n");
        return;
    }
    sendToClient(pollIndex, "newcmd value\n");
    return;
}
```

## Ajouter une requête côté GUI

Si la GUI doit appeler cette commande directement, ajouter une méthode publique dans `gui/src/client/Client.hpp`, puis son implémentation dans `gui/src/client/Client.cpp`.

Exemple:

```cpp
void requestNewCommand();
```

```cpp
void Client::requestNewCommand()
{
    sendMsg("newcmd\n");
}
```

## Ajouter un handler de réponse côté GUI

Si le serveur répond avec une nouvelle ligne protocole, ajouter un handler GUI comme décrit dans `gui/docs/adding_protocol_handler.md`.

## Tests à ajouter

Ajouter un cas dans `tests/network_functional.cpp`:

1. Démarrer le serveur.
2. Connecter un client `GRAPHIC`.
3. Envoyer la nouvelle commande.
4. Vérifier la réponse.
5. Vérifier `sbp` avec des paramètres invalides si la commande prend des arguments.

Lancer:

```sh
make -C network tests_run
```
