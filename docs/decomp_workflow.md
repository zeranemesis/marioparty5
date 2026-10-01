# Décompiler un module : méthode et contrat

Les modules de Mario Party 5 sont décompilés en parallèle, **un module par
worktree et par agent**. Cette page est la référence commune : ce qu'on a le
droit de toucher, comment on travaille, et ce qu'on rend. L'avancement mesuré
est dans [avancement.md](avancement.md).

## La règle qui gouverne le reste

**`ninja` finit par `135 files OK` à chaque commit.** C'est la seule preuve
qu'une unité marquée `Matching` est réellement identique : une unité `Matching`
est liée dans le DOL ou le REL, et le moindre octet différent fait échouer la
vérification SHA-1. Un commit qui casse ce contrôle n'est jamais poussé.

## Les trois statuts d'une unité (`configure.py`)

| statut | sens | liée dans le build matching | entre dans le port |
|---|---|---|---|
| `Matching` | toutes les fonctions et les données identiques | oui | oui |
| `Equivalent` | code **complet** et fidèle, au moins une fonction pas identique | non | oui |
| `NonMatching` | incomplète ou pas encore écrite | non | non |

`Equivalent` n'est pas une facilité : le port exécute ce code. Une unité
`Equivalent` n'a ni fonction vide, ni stub, ni logique « approximative ». Chaque
fonction non identique porte au-dessus d'elle un commentaire
`// NONMATCHING: <ce qui diffère, en une ligne>`.

## Préparer un worktree

Fait par l'orchestrateur, depuis le dépôt principal :

```bash
python tools/new_worktree.py resultDll
```

Résultat : `C:\Users\BEACALCADM\mp5-wt\resultDll` sur la branche
`decomp/resultDll`, disque lié en dur dans `orig/`, outils partagés avec le
dépôt principal, premier `ninja` terminé par `135 files OK`.

## Périmètre d'un agent

Autorisé :

- `src/REL/<module>/` et `include/REL/<module>.h` (créer, modifier) ;
- `config/GP5E01_00/rels/<module>/symbols.txt` et `splits.txt` du module
  (tailles, types, découpage des données quand c'est nécessaire au match) ;
- les lignes `Object(...)` du module dans `configure.py` ;
- pour une unité du DOL (msm, `game/code_801A7E90.c`) : son fichier source, son
  en-tête, et les lignes de `config/GP5E01_00/symbols.txt` qui la concernent.

Toléré, et **listé dans le rapport** : ajouter des déclarations manquantes dans
un en-tête partagé (`include/game/*.h`, `include/dolphin/*.h`…), sans rien
renommer ni supprimer.

Interdit : modifier une unité d'un autre module, changer les options de
compilation, toucher à `orig/`, `build/`, `tools/`, `docs/` ou à la CI.

## La recette

### 1. Lire ce qu'il y a à faire

```bash
python tools/unit_status.py resultDll
```

Liste les unités du module et chaque fonction qui n'est pas encore identique
(`missing` = pas encore écrite). L'assembleur produit par dtk est dans
`build/GP5E01_00/<module>/asm/REL/<module>/<unité>.s` (pour le DOL :
`build/GP5E01_00/asm/<chemin>.s`), les symboles dans
`config/GP5E01_00/rels/<module>/symbols.txt`.

### 2. Un premier jet

```bash
python tools/m2c_draft.py resultDll/REL/resultDll/result
```

Écrit `build/m2c/resultDll/REL/resultDll/result.m2c.c`, avec les types du jeu
en contexte. C'est un **brouillon** : noms `temp_r3`, types devinés, casts
inutiles. On ne le copie jamais tel quel ; on le réécrit dans le style du
dépôt.

### 3. Écrire la source

- `src/REL/<module>/<unité>.c`, dans le style des modules déjà faits :
  `src/REL/m507dll/m507.c`, `src/REL/m526Dll/m526.c`,
  `src/REL/mdpartyDll/*.c` (includes `game/...`, `REL/<module>.h`,
  `datanum/...`, données `lbl_1_data_*` définies dans l'ordre du binaire).
- Les mini-jeux de Mario Party 4 sont une bonne référence de structure :
  `C:\Users\BEACALCADM\Marioparty4\src\REL\` (lecture seule ; ignorer les blocs
  `TARGET_PC`, propres au port MP4).
- Après la **création** d'un fichier source, une fois :

```bash
python tools/reconfigure.py
```

### 4. La boucle de match

```bash
python tools/fndiff.py resultDll/REL/resultDll/result fn_1_1234
```

Compare la fonction compilée depuis `src/` à l'original, instruction par
instruction, avec la ligne C de chaque instruction :

- `!` : vraie différence (registre, ordre, instruction en trop ou en moins) ;
- `~` : seul le nom d'un littéral ou d'un label diffère (`lbl_1_rodata_10`
  contre `@269`), généralement sans conséquence une fois les données en place.

La mesure de référence reste `python tools/unit_status.py <module>` (issue du
rapport objdiff), et la preuve finale reste `ninja`.

Pièges fréquents avec MWCC `-O0,p` (options des REL) :

- une variable locale nommée garde sa valeur dans un registre non volatil
  (`mr r29, r3`) là où un appel imbriqué ne le fait pas ;
- l'ordre de déclaration des variables change l'attribution des registres ;
- les littéraux flottants sont rangés dans `.rodata` dans l'ordre de leur
  première apparition dans le fichier ;
- `s16`/`u16`/`s32` et signé/non signé changent `extsh`, `rlwinm`, `cmplwi` ;
- un `if (x) { ... } else { ... }` et un ternaire ne produisent pas les mêmes
  branches.

### 5. Passer une unité en `Matching`

Quand `unit_status.py` montre toutes les fonctions à 100 % : remplacer
`Object(NonMatching, ...)` par `Object(Matching, ...)`, puis `ninja`.
`135 files OK` : c'est identique. Sinon, ce sont les données (ordre, tailles,
valeurs, alignement) : corriger ou revenir à `NonMatching`.

### 6. Quand une fonction résiste

Après plusieurs approches **réellement différentes** (types, variables
temporaires, forme des boucles, ordre des déclarations, inline), garder la
version la plus proche qui reste fidèle, ajouter le commentaire
`// NONMATCHING:`, et passer l'unité en `Equivalent` une fois **toutes** ses
fonctions écrites. `unit_status.py` compile toutes les sources : une unité
`Equivalent` qui ne compile pas est un échec.

### 7. Commits

- Sur la branche du worktree, un commit par unité terminée ou par étape
  significative : le travail ne doit jamais exister seulement en mémoire.
- Messages en anglais, phrases descriptives comme dans le reste du dépôt :
  `Match resultDll/result`, `Decompile m524Dll: 76 functions, 71 identical`.
- Terminer chaque message par
  `Co-Authored-By: Claude Opus 5 <noreply@anthropic.com>`.
- Avant chaque commit : `ninja` → `135 files OK`, et `git status` ne montre
  rien sous `orig/`, `build/`, ni fichier généré.

## Le rapport rendu à l'orchestrateur

1. Unités : statut final de chacune (`Matching` / `Equivalent` / `NonMatching`).
2. Fonctions identiques / total, depuis `unit_status.py`.
3. Fonctions non identiques : nom, fuzzy %, ce qui diffère.
4. Modifications hors du module (en-têtes partagés, symbols du DOL).
5. Notes pour le portage : structures lues depuis un fichier du disque,
   pointeurs rangés dans des `u32`/`s32`, tableaux de taille fixe calculée pour
   32 bits, arithmétique sur des adresses, tailles de tas.
6. Commits créés (hash court et titre), et `135 files OK` confirmé sur le
   dernier.

## Intégration (orchestrateur)

1. Relire le diff de la branche.
2. `git rebase main`, puis `ninja` → `135 files OK`.
3. Avance rapide de `main`, push.
4. `python tools/avancement.py`, commit de `docs/avancement.md`.
5. `git worktree remove` et suppression de la branche.

## Piège : les sources décompilées non suivies sont fragiles

Un nettoyage d'arbre de travail (`tools/new_worktree.py`, `git stash`, puis
`git clean`) supprime **tous les fichiers non suivis par git**. Les sources
qu'on vient d'écrire sont précisément dans ce cas : tant qu'elles ne sont pas
commitées, elles n'existent que sur le disque.

Concrètement, une session a perdu dans l'ordre `OSThread.c`, `OSInterrupt.c`,
`OSContext.c`, `OSError.c`, `OSMemory.c`, `OSTime.c`, `OSStopwatch.c`,
`OSCache.c`, `__start.c`, ainsi que les outils de scratch non suivis
(`probediff.py`, `mwccprobe.py`, `_splice.py`, `_batch.py`, `_bytes.py`).

Conséquences pratiques :

- **Écrire le fichier, puis `ls -la` pour confirmer qu'il est sur disque** avant
  de continuer. Un `write_file` qui semble réussir peut être suivi d'un nettoyage
  par un autre agent ou un autre thread.
- **Commiter dès qu'une unité compile**, même si elle n'est pas encore à 100 %.
  Le désassemblage de référence (`build/GP5E01_00/asm/dolphin/os/*.s`) est suivi
  par git et survit : on peut toujours reconstruire, c'est long mais possible.
- **Reconstruire depuis le `.s`, pas depuis la mémoire.** Le désassemblage
  contient tout : l'ordre des fonctions, le placement du pool de chaînes, les
  décalages de structure, les constantes.
- Ne pas compter sur `orig/` comme sauvegarde : ce répertoire est lui aussi non
  suivi.
- Les outils de scratch sont aussi des sources à protéger. Les reconstruire à
  partir de ce document est possible, mais coûte une session.
