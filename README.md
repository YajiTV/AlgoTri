# AlgoTri

AlgoTri est un visualiseur interactif d'algorithmes de tri réalisé en C. Il
permet d'observer, directement dans le terminal, les comparaisons et les
échanges effectués pendant un tri.

Le projet a été conçu dans un cadre pédagogique afin d'illustrer les
fondamentaux d'un langage compilé : fonctions, pointeurs, structures,
allocation dynamique, séparation en modules et gestion de la mémoire.

## Fonctionnalités

- visualisation animée sous forme de barres verticales ;
- menu interactif entièrement en français ;
- choix parmi dix algorithmes de tri ;
- tailles de tableau disponibles : 10, 20, 30 ou 40 valeurs ;
- vitesses disponibles : lente, normale ou rapide ;
- génération aléatoire d'une permutation sans doublon ;
- affichage du nombre de comparaisons et d'échanges ;
- couleurs distinctes pour les valeurs normales, comparées, échangées et
  triées ;
- pause, reprise et exécution pas à pas ;
- interruption propre d'un tri et retour au menu ;
- relance du même algorithme avec un nouveau tableau ;
- adaptation aux dimensions du terminal.

## Algorithmes disponibles

| Algorithme | Principe général |
| --- | --- |
| Tri à bulles | Échange les valeurs voisines mal ordonnées |
| Tri par sélection | Sélectionne successivement la plus petite valeur |
| Tri par insertion | Insère chaque valeur dans la partie déjà triée |
| Tri rapide | Partitionne le tableau autour d'un pivot |
| Tri fusion | Divise puis fusionne des sous-tableaux triés |
| Tri par tas | Utilise une structure de tas pour extraire les valeurs |
| Tri de Shell | Effectue des insertions avec des écarts décroissants |
| Tri cocktail | Parcourt alternativement le tableau dans les deux sens |
| Tri à peigne | Compare des valeurs séparées par un écart décroissant |
| Tri gnome | Déplace les valeurs par échanges successifs |

## Prérequis

- un environnement compatible POSIX ;
- un compilateur prenant en charge C17, comme GCC ou Clang ;
- Make ;
- `pkg-config` ;
- la bibliothèque `ncursesw`.

### Arch Linux

```bash
sudo pacman -S gcc make pkgconf ncurses
```

### Debian / Ubuntu

```bash
sudo apt-get update
sudo apt-get install build-essential pkg-config libncurses-dev
```

### Windows 10 / 11 avec MSYS2

Installer MSYS2 depuis PowerShell :

```powershell
winget install --id MSYS2.MSYS2 --exact
```

Ouvrir ensuite **MSYS2 UCRT64** depuis le menu Démarrer. Toutes les commandes
suivantes doivent être exécutées dans ce terminal.

Mettre à jour MSYS2 :

```bash
pacman -Syu
```

Si le terminal demande à être fermé, rouvrir **MSYS2 UCRT64**, puis terminer
la mise à jour :

```bash
pacman -Syu
```

Installer Git, Make, GCC, `pkg-config` et ncurses :

```bash
pacman -S --needed git make mingw-w64-ucrt-x86_64-gcc mingw-w64-ucrt-x86_64-pkgconf mingw-w64-ucrt-x86_64-ncurses
```

Cloner, compiler et lancer AlgoTri depuis le même terminal :

```bash
git clone https://github.com/YajiTV/AlgoTri.git
cd AlgoTri
make
make run
```

## Installation et lancement

Cloner le dépôt, entrer dans son dossier, puis compiler et lancer le programme :

```bash
git clone https://github.com/YajiTV/AlgoTri.git
cd AlgoTri
make
make run
```

L'exécutable généré se trouve dans `build/algotri`. Il peut également être
lancé directement :

```bash
./build/algotri
```

## Commandes

### Dans le menu

| Touche | Action |
| --- | --- |
| `↑` / `↓` | Sélectionner une option |
| `←` / `→` | Modifier la valeur sélectionnée |
| `Entrée` | Valider ou passer à l'option suivante |
| `Q` | Quitter le programme |

### Pendant un tri

| Touche | Action |
| --- | --- |
| `Espace` ou `P` | Mettre en pause ou reprendre |
| `N`, `→` ou `Entrée` | Avancer d'une étape pendant la pause |
| `Q` | Interrompre le tri et revenir au menu |
| `R` | Relancer le tri avec un nouveau tableau une fois terminé |
| `Entrée` | Revenir au menu une fois le tri terminé |

## Couleurs

| Couleur | Signification |
| --- | --- |
| Blanc | Valeur dans son état normal |
| Jaune | Valeur comparée |
| Rouge | Valeur impliquée dans un échange |
| Vert | Valeur triée lors de l'animation finale |
| Cyan | Titres et séparateurs |

## Commandes Make

| Commande | Description |
| --- | --- |
| `make` | Compiler le projet |
| `make run` | Compiler si nécessaire puis lancer AlgoTri |
| `make clean` | Supprimer les fichiers objets |
| `make fclean` | Supprimer les fichiers objets et l'exécutable |
| `make re` | Nettoyer puis recompiler entièrement le projet |

La compilation utilise les options strictes suivantes :

```text
-std=c17 -Wall -Wextra -Wpedantic -Werror
```

## Architecture

```text
AlgoTri/
├── include/
│   ├── algorithms/      # Interfaces des algorithmes
│   ├── core/            # Interfaces du contexte et des opérations
│   └── ui/              # Interfaces du menu et du rendu
├── src/
│   ├── algorithms/      # Implémentation des dix tris
│   ├── core/            # Tableau dynamique, statistiques et orchestration
│   ├── ui/              # Interface ncurses et contrôles clavier
│   └── main.c           # Point d'entrée
├── Makefile
└── README.md
```

Le moteur de tri et l'interface sont séparés. Les algorithmes manipulent un
`SortContext` commun et utilisent des opérations centralisées pour comparer,
échanger, mettre à jour les statistiques et demander un rendu. Cette
organisation évite de dupliquer la logique d'affichage dans chaque tri.

## Choix techniques

- **C17** pour mettre en pratique les notions fondamentales du langage C ;
- **ncursesw** pour construire une interface terminal colorée et interactive ;
- **allocation dynamique** pour gérer le tableau et son redimensionnement ;
- **Fisher-Yates** pour générer une permutation aléatoire sans doublon ;
- **fonctions de rappel** pour connecter le moteur de tri au rendu et aux
  contrôles sans dépendance directe des algorithmes envers ncurses ;
- **Make** pour automatiser la compilation et le nettoyage.

## Validation

Le projet a été validé avec GCC et Clang en compilation stricte. La gestion
mémoire et les principaux parcours interactifs ont également été vérifiés avec
Valgrind pendant le développement.

Le livrable final ne contient pas de suite de tests automatisés.
