# Navigation et vision

La navigation repose sur deux informations serveur :

- `Look`, qui donne les objets visibles devant le joueur ;
- la direction `K` d'un Broadcast, utilisée pour rejoindre le capitaine.

L'IA ne maintient pas une carte globale. Elle prend des décisions locales à partir de la vision immédiate et de quelques mouvements simples.

## Structure de `Look`

`Look` renvoie une liste de tuiles devant le joueur. La tuile `0` est toujours la case actuelle. Ensuite, les tuiles sont listées rangée par rangée, de gauche à droite, dans l'orientation du joueur.

Nombre de tuiles visibles :

```text
(niveau + 1) * (niveau + 1)
```

Exemples :

| Niveau | Tuiles visibles |
|---:|---:|
| 1 | 4 |
| 2 | 9 |
| 3 | 16 |
| 7 | 64 |

Pour un niveau 2, les index sont organisés ainsi :

```text
        4 5 6 7 8
          1 2 3
            0
```

Cette représentation est relative au regard du joueur, pas au nord absolu de la carte.

## Parsing de `Look`

Une réponse ressemble à :

```text
[player, food linemate, , sibur]
```

Le parsing doit :

1. retirer les crochets extérieurs ;
2. séparer les tuiles avec la virgule ;
3. accepter les espaces optionnels après la virgule ;
4. séparer les objets d'une tuile avec l'espace ;
5. compter les occurrences de chaque objet.

Le résultat utilisé par l'IA est une liste de dictionnaires, par exemple :

```python
[
    {"player": 1},
    {"food": 1, "linemate": 1},
    {},
    {"sibur": 1},
]
```

## Conversion index vers coordonnées

`Navigation.tile_index_to_coords()` convertit un index de tuile en `(row, col)`.

- `row` est la distance vers l'avant ;
- `col` est le décalage latéral ;
- `col < 0` signifie à gauche ;
- `col > 0` signifie à droite.

La rangée `r` commence à l'index `r * r` et contient `2 * r + 1` tuiles. La colonne est centrée autour de `0`.

Exemples :

| Index | Coordonnées | Interprétation |
|---:|---|---|
| 0 | `(0, 0)` | case actuelle |
| 1 | `(1, -1)` | devant gauche |
| 2 | `(1, 0)` | devant |
| 3 | `(1, 1)` | devant droite |
| 4 | `(2, -2)` | deux rangées devant, très à gauche |
| 6 | `(2, 0)` | deux rangées devant |

## Mouvements vers une tuile visible

`moves_to_tile()` génère une séquence simple :

1. avancer `row` fois ;
2. si `col < 0`, tourner à gauche puis avancer `abs(col)` fois ;
3. si `col > 0`, tourner à droite puis avancer `col` fois.

Ce chemin n'est pas toujours optimal, mais il est stable et suffisant pour aller chercher une ressource visible.

Exemple :

| Index | Coordonnées | Mouvements |
|---:|---|---|
| 2 | `(1, 0)` | `Forward` |
| 1 | `(1, -1)` | `Forward`, `Left`, `Forward` |
| 8 | `(2, 2)` | `Forward`, `Forward`, `Right`, `Forward`, `Forward` |

## Navigation vers un Broadcast

Pour rejoindre le capitaine, le support utilise la direction sonore reçue dans `message K, ...`. Le code ne calcule pas un chemin complet. Il fait un petit pas dans la direction indiquée, puis attend les Broadcasts suivants pour corriger la trajectoire.

Table actuelle :

| K | Mouvements |
|---:|---|
| 0 | aucun mouvement, même case |
| 1 | `Forward` |
| 2 | `Left`, `Forward` |
| 3 | `Left`, `Forward` |
| 4 | `Left`, `Forward` |
| 5 | `Left`, `Left`, `Forward` |
| 6 | `Right`, `Forward` |
| 7 | `Right`, `Forward` |
| 8 | `Right`, `Forward` |

Cette approche est volontairement corrective : le capitaine rebroadcast souvent, donc le support n'a pas besoin de deviner toute la trajectoire d'un seul coup.

## Monde torique

La carte n'a pas de bord. Sortir par un côté revient de l'autre côté. Cela concerne aussi le son des Broadcasts : la direction reçue correspond au chemin le plus court dans ce monde bouclé.

Conséquence : deux joueurs peuvent être très proches même si leurs coordonnées absolues semblent éloignées sur une carte dessinée à plat.
