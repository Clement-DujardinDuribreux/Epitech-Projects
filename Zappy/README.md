# ZAPPY

> Projet de fin d'année — 2ème année | Epitech Lille

---

## Table des matières

1. [Présentation du projet](#presentation-du-projet)
2. [Architecture](#architecture)
3. [Compilation](#compilation)
4. [Utilisation](#utilisation)
5. [Règles du jeu](#regles-du-jeu)
6. [Ressources et densités](#ressources-et-densites)
7. [Le rituel d'élévation](#le-rituel-delevation)
8. [Vision](#vision)
9. [Commandes disponibles](#commandes-disponibles)
10. [Protocole de communication](#protocole-de-communication)
11. [Interface graphique](#interface-graphique)

---

## Présentation du projet {#presentation-du-projet}

**Zappy** est un jeu réseau multijoueur dans lequel plusieurs équipes s'affrontent sur une carte composée de tuiles contenant des ressources.

Le but est simple : **la première équipe dont au moins 6 joueurs atteignent le niveau d'élévation maximum remporte la partie.**

Le monde du jeu s'appelle **Trantor** — une planète plate (pas de reliefs, pas de montagnes, pas de vallées), dont la carte se boucle sur elle-même : sortir par la droite revient à entrer par la gauche, et ainsi de suite dans toutes les directions.

---

## Architecture

Le projet se compose de **trois binaires** indépendants :

| Binaire | Langage | Rôle |
|---|---|---|
| `zappy_server` | C++ | Serveur de jeu — gère le monde, les ressources, les joueurs |
| `zappy_gui` | C++ | Client graphique — visualisation en temps réel de la partie |
| `zappy_ai` | PYTHON | Client IA — pilote automatiquement un joueur |

Le serveur tourne en **processus unique, thread unique** et utilise `poll` pour le multiplexage des sockets (pas d'attente active).

---

## Compilation

```bash
# Compiler les trois binaires
make

# Ou individuellement
make zappy_server
make zappy_gui
make zappy_ai
```

---

## Utilisation

### Serveur

```bash
./zappy_server -p <port> -x <width> -y <height> -n <team1> <team2> ... -c <clientsNb> -f <freq>
```

| Option | Description |
|---|---|
| `-p port` | Numéro de port |
| `-x width` | Largeur du monde, entre 10 et 42 |
| `-y height` | Hauteur du monde, entre 10 et 42 |
| `-n name1 name2 ...` | Noms des équipes |
| `-c clientsNb` | Nombre de clients autorisés par équipe |
| `-f freq` | Inverse de l'unité de temps, entre 1 et 1000 |

### GUI

```bash
./zappy_gui -p <port> -h <machine>
```

| Option | Description |
|---|---|
| `-p port` | Numéro de port |
| `-h machine` | Nom d'hôte du serveur |

### IA

```bash
./zappy_ai -p <port> -n <team_name> -h <machine>
```

| Option | Description |
|---|---|
| `-p port` | Numéro de port |
| `-n name` | Nom de l'équipe |
| `-h machine` | Nom de la machine (localhost par défaut) |

---

## Règles du jeu {#regles-du-jeu}

### Les habitants (Trantorians)

Les Trantorians sont des êtres sans corps, flous, qui occupent la totalité de la tuile sur laquelle ils se trouvent. Ils sont **pacifistes** — ni violents, ni agressifs — et errent à la recherche de nourriture et de pierres.

- Chaque unité de nourriture permet de survivre **126 unités de temps**.
- En début de partie, chaque joueur dispose de **10 unités de vie** (soit 1260 unités de temps, ou `1260 / f` secondes).
- Les joueurs ne peuvent **pas communiquer en dehors du jeu** : tout échange doit passer par le serveur.

### Les équipes

Au démarrage, chaque équipe dispose de `n` slots représentés par des œufs sur la carte. Un client qui se connecte fait éclore un œuf au hasard parmi les disponibles. Le joueur nouvellement créé démarre dans une direction aléatoire.

Un joueur peut **se reproduire** avec la commande `Fork`, qui pond un œuf et ajoute un slot disponible pour l'équipe.

### Condition de victoire

La partie est remportée par la **première équipe dont au moins 6 joueurs atteignent le niveau 8** (élévation maximale).

---

## Ressources et densités {#ressources-et-densites}

Le serveur génère des ressources au démarrage puis **toutes les 20 unités de temps**, réparties uniformément sur la carte selon la formule :

```
quantité = largeur × hauteur × densité
```

| Ressource | Densité | Exemple (carte 10×10) |
|---|---|---|
| food | 0.5 | 50 |
| linemate | 0.3 | 30 |
| deraumere | 0.15 | 15 |
| sibur | 0.1 | 10 |
| mendiane | 0.1 | 10 |
| phiras | 0.08 | 8 |
| thystame | 0.05 | 5 |

> Au minimum, il y a toujours **au moins une unité de chaque ressource** présente sur la carte.

---

## Le rituel d'élévation {#le-rituel-delevation}

Pour monter de niveau, un joueur doit lancer une incantation (`Incantation`) en réunissant sur la même tuile un certain nombre de joueurs du **même niveau** et des **pierres spécifiques**.

Les joueurs participants n'ont pas besoin d'être dans la même équipe.

Pendant le rituel, tous les participants sont **figés** — aucune action n'est possible. Si les conditions ne sont plus remplies à la fin de l'action, l'élévation **échoue**. En cas de succès, les pierres sont retirées de la tuile.

### Table d'élévation

| Élévation | Joueurs requis | linemate | deraumere | sibur | mendiane | phiras | thystame |
|---|---|---|---|---|---|---|---|
| 1 → 2 | 1 | 1 | 0 | 0 | 0 | 0 | 0 |
| 2 → 3 | 2 | 1 | 1 | 1 | 0 | 0 | 0 |
| 3 → 4 | 2 | 2 | 0 | 1 | 0 | 2 | 0 |
| 4 → 5 | 4 | 1 | 1 | 2 | 0 | 1 | 0 |
| 5 → 6 | 4 | 1 | 2 | 1 | 3 | 0 | 0 |
| 6 → 7 | 6 | 1 | 2 | 3 | 0 | 1 | 0 |
| 7 → 8 | 6 | 2 | 2 | 2 | 2 | 2 | 1 |

---

## Vision

Le champ de vision d'un joueur est limité et **augmente à chaque élévation** : +1 tuile devant, +1 de chaque côté de la nouvelle ligne.

La commande `Look` retourne la liste des objets visibles, tuile par tuile :

```
[player, objet_tuile1, ..., objet_tuileP, ...]
```

Le numérotage des tuiles part de la tuile devant le joueur (1), puis s'étend en arc trigonométrique autour de lui.

### Transmission sonore

Le broadcast est entendu par tous les joueurs, sans identification de l'émetteur. Les joueurs perçoivent uniquement la **direction** d'où provient le son (numéro de tuile) et le message. Sur un monde sphérique, c'est toujours le chemin le plus court qui est emprunté. Si l'émetteur et le récepteur sont sur la même tuile, la direction indiquée est `0`.

---

## Commandes disponibles

| Action | Commande | Durée | Réponse |
|---|---|---|---|
| Avancer d'une tuile | `Forward` | 7/f | `ok` |
| Tourner à droite (90°) | `Right` | 7/f | `ok` |
| Tourner à gauche (90°) | `Left` | 7/f | `ok` |
| Regarder autour | `Look` | 7/f | `[tuile1, tuile2, ...]` |
| Inventaire | `Inventory` | 1/f | `[linemate n, sibur n, ...]` |
| Diffuser un message | `Broadcast text` | 7/f | `ok` |
| Slots libres de l'équipe | `Connect_nbr` | — | valeur |
| Se reproduire | `Fork` | 42/f | `ok` |
| Éjecter les joueurs | `Eject` | 7/f | `ok/ko` |
| Ramasser un objet | `Take object` | 7/f | `ok/ko` |
| Poser un objet | `Set object` | 7/f | `ok/ko` |
| Lancer une incantation | `Incantation` | 300/f | `Elevation underway` / `ko` |

> Toutes les commandes se terminent par `\n`. Une commande invalide retourne `ko`.
> Le client peut envoyer jusqu'à **10 commandes en file** sans attendre de réponse. Au-delà, le serveur les ignore.

### Unité de temps

La durée d'une action se calcule comme suit : `durée_action / f`, où `f` est la fréquence passée en argument au serveur (`f = 100` par défaut).

---

## Protocole de communication

### Connexion du client IA / GUI

```
← WELCOME\n
→ TEAM-NAME\n          (ou "GRAPHIC" pour le GUI)
← CLIENT-NUM\n
← X Y\n
```

- `X` et `Y` : dimensions du monde
- `CLIENT-NUM` : nombre de slots disponibles pour l'équipe

### Broadcast reçu

```
message K, texte\n
```

### Éjection reçue

```
eject: K\n
```

où `K` est la direction d'où vient la poussée.

---

## Interface graphique

La GUI se connecte au serveur en envoyant `GRAPHIC` comme nom d'équipe. Elle est **autonome** une fois lancée.

Elle doit implémenter au minimum une **visualisation 2D** avec icônes du monde en temps réel. Une interface 3D est appréciée mais la fonctionnalité prime sur l'esthétique.

Le serveur adopte une approche optimisée : seules les tuiles modifiées sont poussées vers la GUI (pas de mise à jour globale à chaque changement).

---

**Auteurs :**
- Gabriel Decloquement
- Pierre Leclercq
- Florent Dujardin--Duribreux
- Clément Dujardin--Duribreux
