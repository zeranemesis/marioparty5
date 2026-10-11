# Vague 1+2 — 2026-10-04, flotte d'agents en parallèle

Branche `decomp/m524Dll`. Local uniquement, rien n'est poussé. Après **chaque**
intégration : `ninja` sans `FAILED:`, `dtk shasum -q -c` = **135 files OK**.

## Résultat mesuré

| | début de session | maintenant |
|---|---|---|
| code identique | 28.34 % | **28.5 3 %** |
| fonctions identiques | 6 551 / 21 157 | **6 726+ / 21 157** |
| unités liées | 346 / 881 | 346 / 881 (inchangé, normal) |
| unités fermées devenues mesurables | — | **45** |

Le gain de la session vient presque entièrement d'un geste : **ouvrir des unités
qui n'avaient aucune source**. Une unité sans source ne crédite ni code ni data ;
cinq de ses plus petites fonctions deviennent matchaables en une session. Units
les plus productives du moment : `m548dll/stage` 5/14 en 924 octets,
`m504dll/player` 5/13 en 1 144 octets, `m565dll/stage` 5/18 en 816 octets,
`m552dll/com` 4/9 en 708 octets, `m562dll/koopaboss` 5/21 en 460 octets.

Le plus gros gain unique est venu d'étendre une unité existante plutôt que d'en
créer une : **`m538Dll/m538` est passée de 4/89 à 15/89 fonctions (148 → 2 168
octets)**. Ça demande de lire une source de 154 Ko et de redéfinir des structs,
donc c'est plus risqué — mais le plafond est bien plus haut. Cette unité est
`Equivalent`, donc non liée : aucun hash de module n'a bougé.

## Ce qui a été créé pour faire tourner la flotte

| outil | rôle |
|---|---|
| `docs/AGENT_BRIEF.md` | le brief que chaque agent lit d'abord : la boucle, le piège objdiff, les faits de codegen vérifiés, les murs d'outillage, le budget de contexte, le format de rapport. |
| `tools/dispatch_list.py` | classe les unités `src=n` par `small_sum`, nomme les plus petites fonctions, **filtre les unités absentes de `configure.py`** (31 unités fantômes éliminées, dont `m572dll/player` qui n'a jamais existé). |
| `tools/wt_new.ps1` | provisionne un worktree en ~2 s : `git worktree add`, hardlinks `orig/GP5E01_00`, copie `build/GP5E01_00` (229 Mo), copie `extern/musyx` (sinon `report.json` est impossible), copie `build/tools`, date des sources forcée, `configure.py`. |
| `tools/wt_cfg.ps1` | recopie le `configure.py` **du main** dans un worktree et relance `configure.py`. |
| `tools/install_unit.ps1` | copie **un seul** fichier source d'un agent vers le main, `LastWriteTime` forcé. |

## Protocole d'intégration — dans cet ordre, sans exception

1. `install_unit.ps1` — le fichier source **seul**. Jamais le `configure.py`
   de l'agent.
2. `python configure.py ...` une fois si le fichier est nouveau pour le main.
3. `ninja -j 4` doit finir sans `FAILED:` et sans `error:`.
4. `dtk shasum -q -c config/GP5E01_00/build.sha1` doit afficher `135 files OK`.
5. `python tools/unit_status.py <module> --no-build` **dans le main** : seule
   mesure qui fait foi.

Un agent = un module = un worktree. Deux agents sur le même module se disputent
`.ninja_log`. Un module = un fichier = une unité de dispatch.

## Le fait le plus important de la session

**Le pourcentage d'un agent n'est pas le crédit.** `fndiff.py` pilote objdiff-cli
**2.7.1** ; le crédit vient de `report.json` (objdiff **3.3.1**). 3.3.1 crédite
**plus**, systématiquement.

| ce que l'agent rapporte | ce que objdiff 3.3.1 crédite |
|---|---|
| 91.67 / 85.71 / 80 % | les trois fonctions à 100 % |
| 91.43 % | 100 % |
| 82.35 / 92.86 / 93.10 % | les cinq fonctions à 100 % |
| 48 / 52 / 66 % (24 lignes `~`) | les trois fonctions à 100 % |
| 75 %, 88.89 %, 92.50 %, 94.12 % | 100 % |

Cause mesurée : 2.7.1 compte une instruction non appariée par `bl` vers un
symbole défini dans l'objet original mais pas encore dans le `.o`, et il facture
les offsets de littéraux `.rodata` qui ne peuvent pas s'aligner avant que le
pool de littéraux de l'unité soit complet.

**Le verdict par fonction est donc `0 differing (!)`.** Les lignes `~`
(label-only) ne coûtent rien. Un pourcentage bas sans ligne `!` est un artefact
de relocation, et réécrire le corps dans ce cas gaspille le budget.

Corollaires : un agent qui rapporte pessimiste n'est pas un agent raté ; un agent
qui rapporte 100 % sans l'avoir mesuré dans le main n'est pas crédible non plus.
Le main checkout est l'arbitre.

## Faits de codegen vérifiés (tous mesurés cette session)

- **Un symbole que l'objet original DÉFINIT doit être défini dans la source**,
  pas `extern`. Un `lbl_1_bss_XXXX` en `B` ou `D` dans `powerpc-eabi-nm` est la
  propriété de cette unité de traduction. Coût mesuré sur `mgtourDll/score` :
  trois fonctions coincées à 40-85 % avec un flux d'instructions identique,
  100 % après avoir défini le struct. Longueur par l'écart entre symboles
  successifs dans `symbols.txt`.
- **Boucle décompte : `i = N; while (i--) { }`.** Quand la latch cible est
  `cmpwi` **avant** `subi`. `for (i=N;i>0;i--)`, `for (i=N;i!=0;i--)`,
  `for (i=N;i!=0;--i)` et `while (i!=0){ body; i--; }` émettent tous `subi` en
  premier.
- **MWCC alloue les paramètres APRÈS tous les locals**, en ordre de déclaration
  inverse, et donne une maison sur la pile à tout local initialisé depuis une
  variable de paramètre nue. D'où la paire `stw`/`lwz` et un cadre plus grand.
  Corollaire mesuré : **supprimer une copie locale d'un paramètre** a fait passer
  une fonction de 9 lignes de différence à 0.
- **Un local qu'on ne fait que passer à un appel reste en callee-saved, sans
  maison sur pile** (`lwz r31` + `mr r3,r31`), alors qu'un local mort en prend
  une. Le local qu'il faut est souvent celui qui ne semble pas nécessaire.
- **Un local mort doit être initialisé et jamais relu** pour reproduire un `stw`
  mort ; ajouter `(void)x` supprime le store.
- Affectation de struct (`dst = *src;`) reproduit un bloc de copie ;
  `memcpy(dst, src, 0x48)` émet un `bl memcpy` et casse le match.
- Un paramètre comparé à une constante peut vouloir `volatile s16` : un local
  `s16 v = id;` prend r31 et ajoute `extsh` plus un prologue r31 absents de
  l'original.
- Multiplication flottante : destination = opérande **gauche**
  (`0.1f * (x / 10000.0f)`).
- `(f32)(u32expr)` produit bien le `fsubs` (EC..) que l'original émet contre une
  constante `double 2^52` en f1 ; `fsub` est FC...
- Une valeur de retour constante après un appel doit vivre dans un local, sinon
  MWCC émet `li r3,1` au lieu de la copie callee-saved.
- `x = ~((u32)1 << n) & y` donne `andc` ; `y &= ~(1 << n)` donne `nor` + `and`.
- Un `switch` sur des cas 0..4 à corps vide produit le `cmpwi` mort + une branche
  **inconditionnelle** vers l'épilogue ; deux formes `if/return` donnent `beq`.
- Un `switch` dont le pivot est le cas **médian** matche avec un `case 0:`
  explicite (MWCC enracine à 2 au lieu de 3). Le pivot **minimal** est
  inaccessible : voir `docs/capped.md` C3.
- Une fonction qui rend `self` a besoin du `mr r3,r31` final même si la valeur
  est déjà dans r3.
- Les casts nuls typés ne sont pas toujours justes : `(void(*)(void))0` et `NULL`
  donnent `li r0,0` + `cmplw` là où l'original utilise `cmplwi`. Les deux
  formes méritent d'être essayées, dans les deux sens.
- La largeur de retour d'un **extern** se déduit du test qui suit l'appel : un
  `extsh` juste après signifie `s16`, même si la valeur est stockée dans un
  local `s32` ; un `stw` mort sans `mr r3` signifie `void`.
- Un `.bss` incomplet ne fait pas perdre le crédit des fonctions : objdiff
  crédite `matched_code` fonction par fonction.

## Pièges d'intégration rencontrés

1. **Une source peut compiler dans son worktree et échouer dans le main.**
   `src/REL/m530Dll/kinomi.c` faisait `#include "REL/m530Dll.h"` ; l'en-tête
   existe sur `decomp/m530Dll` mais **pas** sur la branche principale, donc
   `mwcceppc` a aborté et l'unité mesurait 0/31. Règle donnée aux agents : ne pas
   inclure un en-tête REL de module, déclarer localement. Deux agents ont
   spontanément refusé d'inclure un en-tête qui existait pourtant dans le main,
   en déclarant localement — c'est le bon réflexe, il supprime une classe entière
   d'échecs.
2. **Les noms de fonctions dérivés d'adresses entrent en collision entre
   modules.** `fn_1_12DC` existe dans `m536Dll/m536.o` **et** dans
   `m537Dll/m537.o`, à des adresses différentes. Voir les mêmes noms comme « les
   cinq plus petites » de deux modules n'est donc pas un bug de dispatch. Seul
   compte : le symbole existe en `T` dans l'objet de CE module.
3. **`dispatch_list.py` peut n'afficher aucun candidat pour un module qui a
   encore des unités ouvertes** : il exige au moins 6 fonctions et au moins 3
   fonctions entre 20 et 900 octets. Un agent a contourné ça en listant à la main
   et a trouvé une unité de 196 octets que l'outil cachait.
4. **`m572dll` n'est pas une cible utile** : son unité unique `m572` est une
   méga-fonction (le texte saute de 0x710 à 0x85E4).

## Répartition des rôles dans un module

Un module par agent, un fichier par agent. Quand deux agents doivent travailler
dans le même module (fichiers différents), le prompt doit dire explicitement de
ne jamais faire `git add .` — seulement le chemin exact — sinon le commit embarque
le `configure.py` modifié par l'autre agent. C'est arrivé une fois et a été rattrapé.

## Configuration du dépôt pour cette campagne

- `configure.py` du main est **modifié et non commité** ; c'est la référence, et
  `tools/wt_cfg.ps1` le recopie dans tous les worktrees. Un worktree créé depuis
  une branche ancienne porte le `configure.py` de cette branche, qui peut
 沥 disagree sur quelles unités existent.
- Fichiers déjà modifiés dans le main, à ne jamais écraser depuis un worktree :
  `configure.py`, `files.cmake`, `tools/*`, `extern/musyx`, `src/REL/m518dll`,
  `src/REL/m529dll`, `src/REL/m530Dll`, `src/REL/m538Dll` (via son en-tête),
  `src/REL/m540Dll`, `src/REL/m563dll`, `src/REL/m570Dll/player.c`,
  `src/REL/m571dll`, `src/dolphin/os/OSMemory.c`.
- Une seule modification d'en-tête partagé a été intégrée, celle de
  `include/REL/m538Dll.h` (+5/-3), **après** avoir vérifié que ce header n'est
  inclus que par `m538.c` et que `m538.c` est `Equivalent`, donc non lié. Sans
  ces deux vérifications, la modification aurait pu invalider un hash de module.
- `tools/smallfns.py` est **cassé** : `CWD` codé en dur vers une autre machine.
  Utiliser `tools/dispatch_list.py`.

## Où aller ensuite

- 130 unités `src=n` restent ouvertes après `dispatch_list.py`. À ce rythme
  (≈45 unités, ≈6 500 octets, ≈200 fonctions par session) il faut encore une
  trentaine de sessions pour les ouvrir toutes.
- Le plafond par unité est bas (5 fonctions, 150-1 100 octets). La voie la plus
  rapide vers 100 % est donc **de terminer les unités déjà ouvertes** : elles ont
  des sources, donc des structs et des `.bss` déjà corrects, et les fonctions
  restantes sont souvent de la même famille que celles qui matchent.
- Les données restent le goulot : 23,5 % au départ, contre 28,5 % pour le code.
  Créditer `.data` et `.bss` demande de compléter les sources, pas seulement
  d'ajouter des fonctions.