# Porter le moteur : les règles

Le dépôt contient **deux builds** du même code :

- le build **matching**, compilé par MWCC pour GameCube, qui doit rester octet
  pour octet identique au disque (`ninja` → `135 files OK`) ;
- le build **natif**, compilé par MSVC, Clang ou GCC pour un hôte 64 bits
  little-endian (`python tools/build_port.py`).

Tout ce qui suit sert à écrire du code qui satisfait les deux.

## La règle de base

**Le build matching ne change jamais.** Concrètement :

- un changement qui ne modifie pas le code généré par MWCC peut être écrit
  directement (voir « changements neutres » ci-dessous) ;
- tout le reste vit dans `#ifdef TARGET_PC` … `#else` … `#endif`, la branche
  `#else` gardant le code d'origine **mot pour mot** ;
- après chaque lot de modifications : `ninja` doit afficher `135 files OK`.

`TARGET_PC` est défini uniquement par le build natif ; `__MWERKS__` uniquement
par MWCC.

## Changements neutres

Ces réécritures produisent exactement le même binaire GameCube, parce que
`uintptr_t` et `u32` sont le **même type** pour MWCC (`unsigned long`) :

| avant | après |
|---|---|
| `(u32)pointeur` dans un calcul d'adresse | `(uintptr_t)pointeur` |
| `(void *)(entier + 0x80000000)` | `(void *)(uintptr_t)(entier + 0x80000000)` |
| `(TYPE *)(((u32)p) + n)` | `(TYPE *)(((uintptr_t)p) + n)` |

Quand un pointeur est **rangé** dans un champ de structure ou passé à une API
qui garde 32 bits, ce n'est plus neutre : voir les deux motifs suivants.

## Les motifs déjà traités

| motif | solution | exemple |
|---|---|---|
| Pointeur rangé dans `u32 work[4]` d'un objet | champ élargi en `uintptr_t` sous `TARGET_PC` | `include/game/object.h` |
| Pointeur de chaîne rangé dans un numéro de message | table de poignées : `MESSNUM_PTR()` rend un identifiant > 0x80000000, `MESSNUM_TO_PTR()` le résout | `src/port/messnum.c` |
| Adresse mémoire principale passée à l'ARAM | `uintptr_t` (Aurora prend déjà ce type) | `src/game/armem.c` |
| `asm { mflr ... }` pour l'adresse de retour | `0` sur PC, l'adresse ne sert qu'aux messages d'erreur | `src/game/malloc.c` |
| `__declspec(weak)` | encadré par `#ifdef __MWERKS__` | `src/game/hsfmotion.c` |
| Tableau de sommets indexé | `GXSETARRAY(attr, data, taille, pas, le)` : Aurora a besoin de la taille et de l'endianness | `include/game/gxarray.h` |
| Donnée binaire incluse depuis le DOL (`.inc`) | lue sur le disque du joueur au démarrage | `src/port/dolassets.cpp` |
| Module REL | bibliothèque partagée chargée par son nom | `src/port/rel_loader.c` |
| Processus (`gcsetjmp`/`gclongjmp`) | coroutines libco | `src/game/process.c` |
| Sous-système absent d'Aurora (THP) | remplacé par un stub listé dans `PC_REPLACED_UNITS` | `src/port/thp_stub.c` |

## Les données du disque sont gros-boutistes

Tout ce que le jeu lit du disque — modèles HSF, animations ANM, messages,
sauvegardes, tables de plateau — est écrit en **big-endian**, avec des
**offsets 32 bits** que le jeu transforme en pointeurs sur place. Sur un hôte
64 bits little-endian, ni les octets ni la taille des pointeurs ne
correspondent.

La règle : **une structure lue depuis un fichier n'est jamais utilisée
directement**. On décrit le format du fichier par une structure séparée (champs
`u32` pour les offsets, valeurs à retourner) et on la convertit vers la
structure native du jeu :

- `include/port/endian.h` : `PORT_BE16`, `PORT_BE32`, `PORT_BEF32` ;
- une structure `…File` par structure de fichier, jamais partagée avec le jeu ;
- la conversion alloue la structure native et remplit ses pointeurs.

Le drapeau `le` de `GXSETARRAY` dit à Aurora si le tableau a déjà été converti
(`TRUE`) ou s'il est resté tel quel (`FALSE`).

## Statuts et fichiers du port

- `tools/gen_files_cmake.py` construit `files.cmake` depuis `configure.py` :
  une unité entre dans le port quand elle est `Matching` ou `Equivalent`, un
  module REL quand **toutes** ses unités le sont.
- `PC_REPLACED_UNITS` dans ce script liste les unités que le port remplace par
  du code à lui (`game/jmp.c`, THP…).
- Le code propre au port vit dans `src/port/` et `include/port/`.

## Vérifier

```bash
ninja                              # 135 files OK : la décompilation est intacte
python tools/build_port.py         # build natif (Windows par défaut)
```

Un avertissement de troncature de pointeur est une **erreur** dans le build
natif (`/we4311 /we4312`, `-Werror=pointer-to-int-cast`) : c'est le filet qui
attrape les pointeurs rangés dans des entiers 32 bits.
