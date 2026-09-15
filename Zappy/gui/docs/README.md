# Documentation technique de la GUI

La GUI est le client graphique du projet Zappy. Elle se connecte au serveur avec le rôle `GRAPHIC`, lit les messages du protocole graphique et maintient une copie locale de l'état de partie pour l'afficher avec SFML.

## Responsabilites

La boucle principale est portée par `GUI`. Elle récupère les données réseau via `Client`, met à jour la `Map`, traite les événements SFML, déplace la `Camera`, puis dessine la carte et l'ATH.

`Client` isole la communication TCP. Il ouvre la connexion, démarre un thread de lecture, accumule les données reçues et découpe les commandes terminées par `\n`. Chaque commande est routée vers un handler qui met à jour la carte, les joueurs, les oeufs, le temps de jeu ou l'état de fin.

`Map`, `Cell`, `Player` et `Egg` forment le modèle graphique. Ces classes stockent uniquement ce qui est nécessaire à l'affichage: dimensions, ressources, équipes, positions, niveaux, inventaires, messages et entités présentes sur la carte.

`Ath`, `AssetsManager` et `Utils` regroupent le rendu d'interface. `AssetsManager` charge les textures et polices une seule fois, puis les expose par nom aux composants de rendu.

## Flux reseau

Au démarrage, la GUI attend `WELCOME`, répond `GRAPHIC`, puis demande la taille de carte avec `msz`. Les mises à jour courantes passent par les commandes du protocole graphique:

- `msz`: dimensions de la carte.
- `bct`: contenu d'une case.
- `pnw`, `ppo`, `plv`, `pin`, `pdi`: cycle de vie et état des joueurs.
- `enw`, `ebo`, `edi`: cycle de vie des oeufs.
- `pbc`, `pex`, `pdr`, `pgt`, `pfk`: événements visuels de joueur.
- `sgt`, `sst`: unité de temps.
- `seg`: fin de partie.

Les commandes invalides ou incomplètes sont ignorées pour garder l'affichage stable même si une trame partielle arrive.

## Rendu

La carte est projetée en vue inclinée avec `Map::getTileScreenPosition`. La caméra applique les déplacements clavier, le zoom et un clamp dans la fenêtre. Les ressources sont affichées sous forme de coffres sur la carte et de compteurs dans l'ATH.

Un clic sur une case active le panneau de détail de cellule. Ce panneau affiche les ressources de la case et les joueurs présents au moment du clic.

## Tests

Les tests GUI sont écrits en C++ dans `tests/gui_unit.cpp`. Ils couvrent:

- le parsing des arguments CLI;
- les opérations de base sur les cellules;
- les agrégations de ressources et de niveaux par équipe;
- la conversion carte vers écran et écran vers case;
- un scénario fonctionnel avec un serveur TCP local qui envoie des commandes graphiques au `Client`.

La cible `make tests_run` compile et exécute les tests C++ du projet sans lancer les tests Python.

## Guides pratiques

- [Ajouter un handler de protocole](adding_protocol_handler.md)
- [Ajouter un objet rendu](adding_rendered_object.md)
- [Points d'extension GUI](architecture_extension_points.md)
