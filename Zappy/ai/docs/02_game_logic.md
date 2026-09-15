# Logique de jeu

La boucle principale se trouve dans `Agent.run()`. Elle avance un compteur interne `tick`, lit les événements serveur, rafraîchit parfois l'inventaire, broadcast l'état du joueur, puis appelle `decide()`.

## Boucle principale

Le déroulement normal est :

1. incrémenter `tick` ;
2. lire les événements asynchrones avec `drain_events()` ;
3. si l'agent est en incantation, ne rien envoyer et continuer à lire le serveur ;
4. rafraîchir l'inventaire toutes les `INVENTORY_INTERVAL` itérations ;
5. envoyer un Broadcast d'inventaire toutes les `INV_BROADCAST_INTERVAL` itérations ;
6. si le niveau est 8, rester vivant et annoncer `MAX` ;
7. sinon, choisir une action via `decide()`.

Le client décompte aussi une estimation de nourriture à chaque commande envoyée. Cela permet de réagir avant d'attendre un `Inventory`.

## Seuils de nourriture

Les seuils importants sont définis dans `ai/src/config/constants.py`.

| Constante | Valeur | Usage |
|---|---:|---|
| `FOOD_CRITICAL` | 10 | Seuil d'urgence. L'agent abandonne la session et cherche de la nourriture. |
| `SUPPORT_FOOD_TARGET` | 55 | Objectif de nourriture pour les supports. |
| `SUPPORT_INCANTATION_MIN` | 42 | Minimum pour accepter de rejoindre une incantation. |
| `CAPTAIN_FOOD_TARGET` | 90 | Objectif de nourriture du capitaine. |
| `CAPTAIN_COLLECT_FOOD_MIN` | 28 | Le capitaine mange avant de continuer la banque. |
| `CAPTAIN_INCANTATION_MIN` | 65 | Minimum avant de lancer la phase d'incantation. |
| `CAPTAIN_INCANTATION_ABORT` | 35 | Abandon de session si le capitaine tombe trop bas. |

Une unité de nourriture représente 126 unités de temps serveur. Comme les commandes coûtent du temps, l'IA retire une fraction de nourriture estimée à chaque action.

## Comportement du capitaine

Après une courte phase de découverte, le capitaine est choisi de manière
déterministe : l'agent connu avec le plus petit identifiant devient capitaine.

Si le capitaine est seul, il ne tente pas de monter immédiatement. Il prépare la suite : nourriture, collecte de pierres utiles, `Fork` quand aucun slot n'est libre, puis attente de nouvelles connexions manuelles. Avec deux joueurs ou plus, il conserve la même logique de préparation et attend que l'équipe atteigne 6 joueurs avant de lancer les incantations longues.

En mode libre, le capitaine suit cet ordre :

1. gérer la population si l'équipe manque de joueurs ;
2. manger si sa nourriture est trop basse ;
3. collecter les pierres manquantes pour la banque complète ;
4. remanger si nécessaire avant la session ;
5. attendre que les supports soient assez nombreux et assez nourris ;
6. démarrer une session `ladder_master`.

## Comportement des supports

Les supports sont volontairement simples. Ils ne collectent pas les pierres pour eux-mêmes : ils cherchent surtout de la nourriture, gardent leur inventaire à jour, et répondent aux sessions d'incantation du capitaine.

Cette simplicité est importante. Quand les supports ramassent aussi des pierres ou prennent trop d'initiatives, les ressources se dispersent et les incantations deviennent plus difficiles à synchroniser.

## Collecte de la banque

La banque correspond à toutes les pierres nécessaires depuis le niveau actuel du capitaine jusqu'au niveau 8. Le capitaine calcule ce qui manque avec `bank_missing()`.

Quand une pierre est visible, elle est scorée selon :

- le nombre de pierres encore manquantes ;
- la rareté de la ressource ;
- la distance estimée depuis la vision ;
- la quantité présente sur la tuile.

Les pierres rares comme `thystame` ou `phiras` obtiennent donc une priorité plus élevée. Si aucune pierre utile n'est visible, le capitaine explore.

## Population

L'équipe vise `TEAM_TARGET_SIZE = 6`, car les incantations des niveaux élevés demandent 6 joueurs.

Le capitaine utilise :

- `Connect_nbr` pour savoir si un slot est libre ;
- `Fork` pour créer un slot quand il n'y en a pas ;

Un `Fork` coûte cher en temps, donc il est évité si le capitaine n'a pas assez de nourriture. Après un `Fork`, l'agent ne lance pas de nouveau processus : il crée seulement l'oeuf et garde le slot disponible pour une connexion manuelle.

Tant que l'équipe n'a pas 6 joueurs connus, la population reste prioritaire sur le lancement des incantations finales. Les supports déjà connectés ne montent pas chacun de leur côté : ils restent disponibles, cherchent surtout de la nourriture et attendent que le groupe complet soit prêt.

## Exploration

L'exploration combine des lignes droites et des virages alternés. L'objectif n'est pas de cartographier précisément le monde, mais de couvrir régulièrement de nouvelles tuiles sans rester coincé dans une petite zone.

Quand une ressource utile ou de la nourriture est visible, l'agent utilise la navigation issue de `Look` pour rejoindre la tuile.
