# </> 42sh

## 📋​ Sommaire
1. [Description](#-description)
2. [Installation](#%EF%B8%8F-installation)
3. [Utilisation](#-utilisation)
4. [Fonctionnalités](#-fonctionnalités)
5. [Équipe](#-équipe)

## ✨​ Description

42sh est un projet d'Epitech qui consiste à recréer un shell (interpréteur de commandes) basé sur TCSH.
Ce shell implémente les fonctionnalités de base d'un shell Unix ainsi que des fonctionnalités avancées comme les inhibiteurs, les globbings, les backticks et bien plus.

**Langages utilisés :** C

## 🛠️​ Installation

Suivez ces étapes pour installer le projet sur votre machine locale :

1. Clone du repository
    ```bash
    git clone git@github.com:EpitechPGEPromo2029/B-PSU-200-LIL-2-1-42sh-gabriel.decloquement.git
    ```

2. Allez dans le répertoire du projet (une fois avoir clone le repository):
    ```bash
    cd B-PSU-200-LIL-2-1-42sh-gabriel.decloquement/
    ```
3. Compiler le projet :
    ```bash
    make
    ```
    ### Commandes Makefile :
    Tout recompiler :
    ```bash
    make re
    ```
    Supprimer les fichiers temporaires :
    ```bash
    make clean
    ```
    Commande clean + Suppression du binaire / executable :
    ```bash
    make fclean
    ```

## ​📈​ Utilisation

- Lancer le shell (après la commande make):
    ```bash
    ./42sh
    ```

## 🔧 Fonctionnalités

Notre shell implémente les fonctionnalités suivantes:

| Fonctionnalité | Description | Exemple |
|----------------|-------------|---------|
| Inhibiteurs    | Permet d'échapper des caractères spéciaux | `echo 'Hello World'` |
| Globbings      | Expansion de motifs | `ls *.c` |
| Parenthèses    | Groupement de commandes | `(ls && pwd)` |
| Variables d'environnement | Accès aux variables du système | `setenv Pierre 21`, `env`, `unsetenv Pierre` |
| Variables locales | Variables définies par l'utilisateur | `local var value` |
| Variables spéciales | Variables prédéfinies | `echo $term` |
| History        | Historique des commandes | `history`, `history 5` |
| Aliases        | Création de raccourcis de commandes | `alias ll "ls -la"` |
| Line edition   | Édition multi-ligne, auto-complétion | Utiliser les flèches et TAB |

> Note: Les fonctionnalités de contrôle de job (`&`, `fg`, `bg`), les backticks (`` `commande` ``) et le scripting ne sont pas implémenteés.

## 📍 Équipe

| Nom et Prénom | Rôle | Profil GitHub |
|---------------|------|--------------|
| Gabriel Decloquement | Développeur | [gdecloquement](https://github.com/gdecloquement) |
| Clément Dujardin--Duribreux | Développeur | [Clement-DujardinDuribreux](https://github.com/Clement-DujardinDuribreux) |
| Florent Dujardin--Duribreux | Développeur | [Pierrelec59](https://github.com/Pierrelec59) |
| Pierre Leclercq | Développeur | [FlorentDD-epitech](https://github.com/FlorentDD-epitech) |

Merci de votre lecture et votre petite star ! ; )