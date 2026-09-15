# Communication

L'IA communique avec deux canaux :

- les commandes directes envoyées au serveur Zappy ;
- les `Broadcast` reçus par tous les joueurs de la carte.

Les Broadcasts sont publics. L'IA filtre donc les messages par préfixe, nom d'équipe et format attendu.

## Handshake

Au lancement, le client suit le protocole standard :

1. le serveur envoie `WELCOME` ;
2. l'IA envoie le nom de l'équipe ;
3. le serveur renvoie le nombre de slots disponibles ;
4. le serveur renvoie les dimensions de la carte.

Ces informations initialisent `free_slots`, `map_width` et `map_height`.

## Commandes serveur utilisées

| Commande | Usage dans l'IA |
|---|---|
| `Forward`, `Right`, `Left` | Navigation et exploration. |
| `Look` | Vision des tuiles visibles, choix des ressources et vérification de la case d'incantation. |
| `Inventory` | Correction de l'estimation de nourriture et de pierres. |
| `Broadcast text` | Inventaire, appels d'incantation, confirmations et annulations. |
| `Connect_nbr` | Vérification des slots libres pour ajouter des joueurs. |
| `Fork` | Création d'un slot quand l'équipe doit grossir. |
| `Take object` | Récupération de nourriture ou de pierres. |
| `Set object` | Dépôt des pierres exactes avant incantation. |
| `Incantation` | Tentative d'élévation. |

`Eject` existe côté agent, mais la stratégie actuelle ne repose pas dessus comme mécanisme principal.

## Format des messages Broadcast

Les messages internes utilisent ce format avant chiffrement :

```text
ZPY|<team>|<agent_id>|<level>|<action>|<payload>
```

Le message est chiffré avant l'envoi via `encrypt_message()`, puis déchiffré à la réception avec `decrypt_message()`.

Exemple logique :

```text
ZPY|Blue|1234|4|INCANTATION|BANK-4-1234-980
```

## Actions Broadcast

| Action | Emetteur | Rôle |
|---|---|---|
| `INV` | Tous | Partage du niveau, de la nourriture, de la génération et des pierres. |
| `INCANTATION` | Capitaine | Appel des supports du même niveau vers une session. |
| `READY` | Support | Confirmation qu'il est sur la case du capitaine. |
| `CANCEL` | Capitaine | Annulation d'une session. |
| `GO` | Capitaine | Signal juste avant l'incantation. |
| `MAX` | Joueur niveau 8 | Annonce qu'un joueur a atteint le niveau final. |

## Filtrage à la réception

Un Broadcast est ignoré si :

- il ne peut pas être déchiffré ;
- il n'a pas exactement 6 champs ;
- le préfixe n'est pas `ZPY` ;
- le nom d'équipe ne correspond pas ;
- l'émetteur est l'agent lui-même ;
- le niveau ne correspond pas pour une session d'incantation ;
- l'émetteur n'est pas le capitaine attendu.

Ce filtrage est volontairement strict pour éviter qu'un message ennemi ou obsolète modifie le comportement.

## Direction du Broadcast

Le serveur donne une direction `K` entre 0 et 8 :

| K | Sens |
|---:|---|
| 0 | Même tuile |
| 1 | Devant |
| 2 | Devant gauche |
| 3 | Gauche |
| 4 | Derrière gauche |
| 5 | Derrière |
| 6 | Derrière droite |
| 7 | Droite |
| 8 | Devant droite |

La carte boucle sur elle-même. Le son peut donc venir d'un côté qui semble opposé si le chemin torique est plus court.

## Gestion asynchrone

Les réponses directes aux commandes et les événements comme `message`, `eject`, `dead` ou `Current level` peuvent arriver entre deux actions. Le client les remonte via `poll()` et `popEvents()`, puis l'agent les traite avec `handle_event()` ou `handle_response()`.

Pendant une incantation acceptée, l'agent ne doit plus envoyer de commandes. Il attend seulement `Current level: k` ou `ko`.
