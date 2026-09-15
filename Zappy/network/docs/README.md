# Documentation technique du network

Le module network contient le serveur Zappy. Il accepte les clients IA et les clients graphiques, valide le protocole texte, planifie les actions des joueurs, met à jour l'état de partie et émet les événements destinés à la GUI.

## Responsabilites

Le module est découpé en quatre zones principales:

- `Server` possède le socket d'écoute, la boucle `poll`, la liste des clients, la planification des commandes et les notifications graphiques.
- `Client` stocke une connexion TCP, ses buffers de lecture et d'écriture, l'équipe choisie, le joueur associé et les compteurs d'actions en attente.
- `commands` transforme les commandes du protocole en objets exécutables. Chaque commande expose son coût en cycles.
- `game` contient la carte torique, les joueurs, les inventaires, les ressources, les œufs et la timeline.

Le serveur est mono-thread. Les événements réseau sont gérés avec `poll(2)`. Les délais de jeu sont représentés par des actions datées dans `Timeline`.

## Compilation

Depuis la racine du dépôt:

```sh
make zappy_server
```

Le binaire produit est `./zappy_server`.

Les tests network sont inclus dans la cible:

```sh
make -C network tests_run
```

## Lancement

Le serveur attend toutes les options obligatoires:

```sh
./zappy_server -p 4242 -x 10 -y 10 -c 3 -f 100 -n team1 team2
```

Arguments:

- `-p`: port TCP du serveur.
- `-x`: largeur de la carte, entre `10` et `42`.
- `-y`: hauteur de la carte, entre `10` et `42`.
- `-c`: nombre initial de slots par équipe.
- `-f`: fréquence de jeu, entre `1` et `1000`.
- `-n`: liste des noms d'équipes.

Le serveur rejette les valeurs non numériques, les options inconnues, les noms
d'équipes dupliqués et le nom réservé `GRAPHIC`.

## Cycle de connexion

1. Un client TCP se connecte.
2. Le serveur envoie `WELCOME`.
3. Le client répond `GRAPHIC` ou un nom d'équipe.
4. Un client graphique reçoit l'état initial de la carte.
5. Un client IA reçoit le nombre de slots restants et la taille de carte.
6. Les commandes IA sont planifiées selon leur coût en cycles.
7. Les commandes GUI sont exécutées immédiatement.

Quand un joueur n'a plus de nourriture, le serveur envoie `dead` puis ferme la connexion après avoir vidé le buffer d'écriture.

## Planification des actions

Les commandes IA ne sont pas exécutées au moment exact où elles sont reçues. Le serveur calcule leur délai avec le coût en cycles et la fréquence courante, puis les insère dans `Timeline`.

Chaque client IA peut empiler jusqu'à dix actions. Si la limite est atteinte, le serveur répond `ko`.

Les événements internes comme la faim, le respawn de ressources, les notifications graphiques ou la fin d'incantation passent aussi par la timeline avec un poll index spécial.

## Protocole graphique

Les clients graphiques s'identifient avec `GRAPHIC`. Ils peuvent demander:

- `msz`: taille de carte.
- `bct X Y`: contenu d'une case.
- `mct`: contenu de toute la carte.
- `tna`: noms des équipes.
- `ppo #N`: position d'un joueur.
- `plv #N`: niveau d'un joueur.
- `pin #N`: inventaire d'un joueur.
- `sgt`: fréquence courante.
- `sst T`: modification de la fréquence.

Le serveur émet aussi des événements asynchrones: déplacement, collecte, dépôt, incantation, œufs, mort et fin de partie.

## Protocole IA

Commandes IA supportées:

- `Forward`
- `Right`
- `Left`
- `Look`
- `Inventory`
- `Broadcast TEXT`
- `Connect_nbr`
- `Fork`
- `Eject`
- `Take RESOURCE`
- `Set RESOURCE`
- `Incantation`

Les ressources utilisent les noms `food`, `linemate`, `deraumere`, `sibur`, `mendiane`, `phiras` et `thystame`.

## Erreurs protocole

Une commande IA inconnue retourne `ko`. Une commande GUI inconnue retourne `suc`. Des paramètres GUI invalides retournent `sbp`. Une équipe inconnue ou sans slot disponible est rejetée pendant la phase de connexion.

## Guides pratiques

- [Ajouter une commande IA serveur](adding_ai_command.md)
- [Ajouter une commande graphique serveur](adding_gui_command.md)
- [Ajouter une ressource ou un objet serveur](adding_resource_or_object.md)
