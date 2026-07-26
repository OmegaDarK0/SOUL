# SOUL Engine (v0.1.0-alpha)

*Lire dans d'autres langues : [English](README.md)*

## 1. Introduction et Philosophie du Moteur

Le moteur SOUL est une solution logicielle haute performance développée en C++20 suivant une approche "C with classes". Il a été conçu pour répondre aux exigences les plus strictes en matière de calcul temps réel et de gestion de ressources matérielles.

L'architecture repose intégralement sur le **Data-Oriented Design (DOD)** afin de maximiser la localité des données et l'efficacité du cache processeur. Nous visons un objectif de "Zéro-Allocation" durant la boucle d'exécution principale, garantissant une latence minimale et une stabilité des systèmes. Le moteur privilégie systématiquement la clarté des algorithmes et une organisation de la mémoire contiguë pour éviter les *cache misses*.

## 2. Architecture Technique

SOUL entretient une relation symbiotique avec le framework VOID (C17) : SOUL gère la logique haut niveau alors que VOID assure l'abstraction matérielle (Back-end) via des pointeurs opaques (*Opaque Pointers*).

* **ECS Pur :** Gestion des entités par signatures binaires avec stockage contigu au sein de structures `component_array` pour une itération rapide.
* **Job System :** Architecture multi-threadée strictement *lock-free* exploitant les primitives atomiques du framework VOID, comme `void_atomic_compare_exchange`, pour saturer les cœurs CPU détectés.
* **Gestion Mémoire :** Utilisation exclusive d'arénas via VOID. L'arène globale gère la persistance tandis que l'arène de frame (10% de la mémoire totale) est réinitialisée à chaque cycle sans coût via `void_frame_free`.

## 3. Système de Composants et ECS

Le moteur expose une suite de composants optimisés pour le stockage en "Stream of Arrays" (SoA). La registry centrale pilote la création des entités et l'attribution des composants dans la limite de `MAX_COMPONENTS` (32), assurant une empreinte mémoire prévisible.

| Composant | Description |
| :--- | :--- |
| **Transform** | Définit la position, la rotation et l'échelle en trois dimensions. |
| **Velocity** | Capture les vecteurs de direction et la vitesse scalaire. |
| **Body** | Critique pour la physique : stocke la masse inverse (`imass`) et le coefficient de rebond (`bounciness`). Une valeur `imass = 0` définit un objet de masse infinie (statique). |
| **Size** | Définit les dimensions AABB (Axis-Aligned Bounding Box). |
| **Sprite** | Contient les références de textures via l'Asset Manager. |
| **Viewport** | Gère les paramètres de projection et de tracking. |

## 4. Systèmes et Physique

### Physics
Le système de physique intègre les vecteurs de mouvement pour mettre à jour les transformations en fonction du `delta_time` (`dt`) calculé par le back-end VOID.

### Collision
La détection AABB utilise la `spatial_grid` pour limiter les tests aux voisinages immédiats. La résolution des collisions applique des impulsions basées sur l'`imass` des entités impliquées.

### Scene (Grille Spatiale)
La `spatial_grid` partitionne dynamiquement l'espace. Les entités sont triées par index de cellule via `std::sort` dans `scene.cpp` pour garantir des recherches de voisinage ultra-performantes.

### Camera
Le système de caméra gère le suivi dynamique des cibles et la conversion des coordonnées monde vers l'espace écran du viewport.

### Render
Le moteur de rendu implémente un système de **Z-sorting**. Toutes les commandes de rendu sont triées par profondeur (`std::sort` sur l'axe Z) avant l'envoi au back-end pour garantir l'ordre d'affichage.

## 5. Dépendances et Prérequis

* Le moteur dépend dynamiquement de la bibliothèque `libVOID` (v0.1.0-alpha).
* Les dépendances tierces incluent **SDL2** et **SDL2_image** pour la gestion du contexte graphique et des ressources.
* Un compilateur supportant le standard **C++20** (`g++` ou `clang`) est requis pour l'utilisation des fonctionnalités modernes du langage.
* Le système est compatible multiplateforme, supportant nativement Windows (via MinGW/MSYS2) et Linux (via GCC/Clang).

## 6. Compilation et Utilisation

La compilation est automatisée via un `Makefile` rigoureux.

Pour construire le binaire principal :
```bash
make
```

Pour générer la bibliothèque partagée :
```bash
make lib
```

**Arguments de lancement :**
* Le binaire SOUL accepte plusieurs arguments pour les tests front-end : `sandbox`, `kinetik` ou `survival`.
* Le binaire de test VOID accepte des paramètres de bas niveau pour tester les sous-systèmes : `memory`, `thread` ou `render`.

> **Note de Mémoire :** Pour garantir l'absence de fragmentation, toute allocation doit impérativement passer par `void_arena_alloc` ou les fonctions `arena_make` du moteur.

## 7. Structure du Projet

L'arborescence distingue clairement les interfaces des implémentations :
* `include/` : Contient les interfaces (`.hpp`).
* `src/` : Contient les implémentations (`.cpp`).
* `void.h` : Sert de pont d'interface (bridge) absolu entre le noyau en C et la logique en C++.
* `shell.cpp` : Constitue le point d'entrée applicatif, orchestrant l'initialisation des arénas et le lancement de la boucle de jeu.
* `ecs.cpp` et `job.cpp` : Encapsulent les couches de performance pure, isolées de la logique métier.

## 8. Licence

Ce projet est distribué sous licence **GPL 3**. Le code source et l'intégration avec le framework VOID sont protégés par le droit d'auteur. Toute redistribution doit maintenir les mentions de paternité originales et respecter les termes de la licence publique générale GNU.