# Space Miner

Jeu console en C (Windows) où le joueur aligne des gemmes, atteint des objectifs de score et progresse à travers plusieurs niveaux avec bonus/malus.

## Sommaire
- [Pitch](#pitch)
- [Stack technique](#stack-technique)
- [Installation et lancement](#installation-et-lancement)
- [Structure des fichiers](#structure-des-fichiers)
- [Points techniques intéressants](#points-techniques-intéressants)
- [Résultats](#résultats)
- [Améliorations futures](#améliorations-futures)

## Pitch
Space Miner est un projet de jeu en console qui combine logique de grille, gestion d’état de partie, progression de difficulté et ambiance audio.

## Stack technique
- **Langage** : C
- **Plateforme** : Windows (console)
- **Compilation** : GCC (MinGW)
- **Audio** : `winmm` (`mmsystem.h`, `#pragma comment(lib, "winmm.lib")`)

## Installation et lancement
### Prérequis
- Windows
- GCC (MinGW) installé et accessible dans le `PATH`

### Option 1 — Script `.bat`
Depuis ce dossier (`projects/space-miner`) :
1. Vérifier la présence du dossier `Sons` avec les fichiers audio attendus
2. Double-cliquer sur `LANCER_SPACE_MINER.bat`

### Option 2 — Ligne de commande
Toujours depuis `projects/space-miner` :

```bash
gcc main.c parametres.c menu_options.c affichage.c fonctionnement.c extensions.c -o SPACE_MINER.exe -lwinmm
SPACE_MINER.exe
```

## Structure des fichiers
```text
projects/space-miner/
├── README.md
├── LANCER_SPACE_MINER.bat
├── header.h
├── main.c
├── affichage.c
├── fonctionnement.c
├── parametres.c
├── menu_options.c
├── extensions.c
└── NOTICE-SPACE_MINER.md
```

## Points techniques intéressants
- Initialisation console UTF-8 pour l’affichage (`SetConsoleOutputCP`, `SetConsoleCP`)
- Boucle de jeu avec menu principal, sauvegarde/chargement et progression
- Gestion des bonus/malus et d’un niveau bonus
- Lecture audio dynamique avec playlist

## Résultats
- Jeu exécutable en local sur Windows
- Architecture C séparée par responsabilités (affichage, moteur, paramètres, options, extensions)
- Script de lancement automatisé pour simplifier l’exécution

## Améliorations futures
- Ajouter des tests unitaires sur la logique de grille
- Externaliser davantage de paramètres de jeu dans des fichiers de configuration lisibles
- Ajouter captures/GIF et instructions visuelles dans cette documentation
- Proposer une version multiplateforme (adaptation hors dépendances Windows)
