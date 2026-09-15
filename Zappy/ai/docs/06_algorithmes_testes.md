# Algorithmes testés

Ce fichier résume les principales stratégies testées pour l'IA et les raisons qui ont mené au système final de banque.

## 1. Machine à états distribuée

La première approche reposait sur une machine à états classique : chaque agent pouvait explorer, survivre, appeler les autres, suivre un appel, puis tenter une incantation.

L'idée était simple : chaque joueur devenait autonome et pouvait prendre l'initiative si son inventaire permettait une élévation.

Ce qui a posé problème :

- beaucoup trop de Broadcasts étaient envoyés ;
- plusieurs agents pouvaient lancer des appels concurrents ;
- les agents changeaient souvent d'objectif ;
- la nourriture descendait vite pendant les phases d'attente ;
- les joueurs mouraient avant d'avoir stabilisé une montée de niveaux.

Conclusion : l'approche était trop bruyante. La communication coûtait cher et l'équipe passait trop de temps à se synchroniser.

## 2. Chacun pour soi avec rassemblement

La deuxième approche laissait chaque agent collecter ses propres ressources, avec un rassemblement seulement au moment de l'incantation.

L'objectif était de réduire les Broadcasts : les joueurs farmaient de leur côté, puis répondaient aux appels quand ils étaient du bon niveau.

Ce qui a posé problème :

- les joueurs ne montaient pas tous au même rythme ;
- certains arrivaient en retard aux rassemblements ;
- les ressources étaient réparties dans plusieurs inventaires ;
- une partie de l'équipe restait bloquée à un niveau inférieur ;
- les derniers niveaux demandaient trop de joueurs synchronisés ;
- l'équipe finissait souvent bloquée autour des niveaux 4 ou 5.

Conclusion : cette version progressait mieux au début, mais la désynchronisation devenait bloquante sur les niveaux élevés.

## 3. Système final de banque

La stratégie finale centralise les décisions importantes.

Principe :

- un capitaine est élu ;
- le capitaine collecte toutes les pierres nécessaires ;
- les autres joueurs cherchent surtout de la nourriture ;
- le capitaine agrandit l'équipe jusqu'à 6 joueurs ;
- une fois la banque prête, tout le monde rejoint un seul endroit ;
- le capitaine pose les pierres exactes et spam les montées de niveau tant que le groupe tient.

Avantages :

- beaucoup moins de décisions contradictoires ;
- moins de pierres perdues dans des inventaires séparés ;
- supports plus simples, donc plus robustes ;
- les Broadcasts servent surtout à annoncer l'état et à guider vers une session ;
- les incantations s'enchaînent mieux quand le groupe est enfin réuni.

Limites :

- le capitaine est critique : s'il meurt, la banque est perdue ;
- la collecte initiale peut être longue ;
- il faut beaucoup de nourriture avant de lancer la chaîne d'incantations ;
- les supports doivent rester disponibles sans se disperser.

## Comparatif

| Strategie | Points forts | Probleme principal | Resultat |
|---|---|---|---|
| Machine à états distribuée | Très autonome | Trop de Broadcasts, mort rapide | Abandonnée |
| Chacun pour soi + rassemblement | Bon début de partie | Désynchronisation et niveaux inégaux | Bloquée niveau 4/5 |
| Banque centralisée | Coordination plus stable | Dépendance forte au capitaine | Stratégie retenue |

## Leçon retenue

Dans Zappy, une IA totalement distribuée semble séduisante, mais les contraintes du jeu rendent la synchronisation coûteuse. La stratégie retenue sacrifie un peu de parallélisme pour gagner en stabilité : un joueur pense pour l'équipe, les autres survivent et se rendent disponibles au bon moment.
