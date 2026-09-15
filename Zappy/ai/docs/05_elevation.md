# Élévation

L'élévation est la partie la plus risquée de l'IA : elle demande des pierres exactes, un nombre précis de joueurs du même niveau, et une synchronisation longue. La stratégie actuelle évite de répartir cette responsabilité entre tous les agents. Le capitaine centralise les pierres dans sa banque puis organise les incantations.

## Table des prérequis

| Niveau | Joueurs | linemate | deraumere | sibur | mendiane | phiras | thystame |
|---:|---:|---:|---:|---:|---:|---:|---:|
| 1 -> 2 | 1 | 1 | 0 | 0 | 0 | 0 | 0 |
| 2 -> 3 | 2 | 1 | 1 | 1 | 0 | 0 | 0 |
| 3 -> 4 | 2 | 2 | 0 | 1 | 0 | 2 | 0 |
| 4 -> 5 | 4 | 1 | 1 | 2 | 0 | 1 | 0 |
| 5 -> 6 | 4 | 1 | 2 | 1 | 3 | 0 | 0 |
| 6 -> 7 | 6 | 1 | 2 | 3 | 0 | 1 | 0 |
| 7 -> 8 | 6 | 2 | 2 | 2 | 2 | 2 | 1 |

## Banque de pierres

La banque est calculée par `full_ladder_need()`. Elle additionne toutes les pierres nécessaires depuis le niveau courant du capitaine jusqu'au niveau 8.

Depuis le niveau 1, il faut au total :

| Ressource | Quantité |
|---|---:|
| linemate | 9 |
| deraumere | 8 |
| sibur | 10 |
| mendiane | 5 |
| phiras | 6 |
| thystame | 1 |

Le capitaine garde ces ressources dans son inventaire. Les supports ne sont pas censés constituer leur propre stock de pierres, car cela rend les incantations dépendantes de trop de joueurs et augmente les risques de désynchronisation.

## Démarrage d'une session

Le capitaine démarre une session seulement si :

- sa banque est complète ;
- il a assez de nourriture ;
- l'équipe connue ou projetée atteint la taille cible ;
- assez de joueurs du même niveau ont une nourriture suffisante.

La session reçoit un identifiant unique :

```text
BANK-<level>-<captain_id>-<tick>
```

Les supports ne rejoignent que les sessions du capitaine attendu et de leur niveau actuel.

## Rassemblement

Pendant la session :

1. le capitaine broadcast `INCANTATION` régulièrement ;
2. les supports se déplacent vers la direction sonore reçue ;
3. quand un support reçoit la direction `0`, il envoie `READY` ;
4. le capitaine attend assez de joueurs sur sa case ;
5. le capitaine attend aussi assez de `READY`.

Cette double vérification évite de lancer une incantation parce que des joueurs sont brièvement visibles mais pas encore synchronisés côté logique.

## Préparation de la tuile

Avant d'envoyer `Incantation`, le capitaine prépare la tuile actuelle :

- il retire les pierres en trop avec `Take` ;
- il pose les pierres manquantes avec `Set` ;
- il vérifie que la tuile contient exactement les pierres du niveau courant ;
- il vérifie encore le nombre de joueurs.

Le serveur vérifie les prérequis au lancement et à la fin de l'incantation. Une pierre volée ou un joueur déplacé peut donc faire échouer l'élévation.

## Pendant l'incantation

Quand le serveur répond `Elevation underway`, l'agent passe en état `incanting`. Dans cet état, il n'envoie plus de commande et lit uniquement les événements serveur.

Résultats possibles :

| Réponse | Effet |
|---|---|
| `Current level: k` | Le niveau local est mis à jour, l'inventaire est rafraîchi, la session continue ou se termine. |
| `ko` | L'incantation échoue, l'agent quitte la session. |
| `dead` | Le programme s'arrête. |

Après un niveau réussi, si la session existe encore, elle est adaptée au nouveau niveau. Le capitaine peut donc tenter d'enchaîner les élévations tant que les joueurs restent groupés et nourris.

## Points fragiles

- Les supports peuvent manquer de nourriture pendant l'attente.
- Un support peut perdre le capitaine si les Broadcasts ne sont plus reçus.
- Une case peut contenir trop de pierres si un ancien essai a échoué.
- Les joueurs doivent être du même niveau, sinon le serveur refuse l'incantation.
- Les niveaux 6 et 7 demandent 6 joueurs, donc la population doit être prête avant de commencer.
