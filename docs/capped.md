# Units capped — mur d'outillage prouvé

Une unité est *cappée* quand aucune forme de source C ne peut reproduire les octets
de l'original, **et** que la raison est mesurée et non devinée. Une fonction
individuelle peut être cappée sans que son unité le soit.

Références croisées : `docs/AGENT_BRIEF.md` section 7 (murs d'outillage),
`docs/WAVE1.md` (faits de codegen vérifiés).

## Pourquoi MWCC 1.3.2 ne peut pas atteindre l'original

L'original est compilé par CodeWarrior `0b 02 04 07 01 01 02 00`. L'outillage
installé émet `0a 02 04 02 01 00 02 00`. Cet écart décide seul de plusieurs
familles de differences que le C ne peut pas réparer.

## Caps confirmées

### C1 — le bloc `assert` de `singleton.h` est hors ligne dans l'original

L'original émet `cmpwi` / `beq` vers un bloc froid puis un `b` pour le sauter.
MWCC 1.3.2 inline toujours le bloc. Vingt-et-une formulations de source et
vingt-et-une versions de MWCC ont été essayées ; toutes inlinent.

Mesure de contrôle : un agent a obtenu 3 sur 4 lignes de diff à zéro en
retirant le `stw` de mort que MWCC ajoute au chemin inliné — donc ce n'est pas
la faute du type de la condition.

Conséquence : toute fonction contenant cet `assert` ne peut probablement pas
atteindre 100 %. Écrire le corps fidèle, poser un commentaire d'une ligne,
passer à la suite.

### C2 — l'argument de `__ptmf_scall` est en `r12`, pas `r4`

L'original suit l'ABI du builtin CodeWarrior : PTMF en `r12` (r3 = self), avec
un `nop` après le `bl`. MWCC utilise `r4` et n'émet pas de `nop`.

Un `__asm` inline reproduit exactement les trois instructions, mais MWCC cesse
alors de garder `self` dans r31 et tout le prologue se décale.

Mesuré sur `m511Dll/REL/m511Dll/camera` `fn_1_B4C0` et
`m579Dll/REL/m579Dll/zako` `fn_1_16554`.

### C3 — le pivot d'un `switch` sur constantes ne peut pas être le cas minimal

L'original `m549Dll/REL/m549Dll/game` `fn_1_AD78` émet, dans cet ordre :

```
cmpwi rX, 0 / beq / blt
cmpwi rX, 6 / b
```

c'est-à-dire qu'il teste **le plus petit cas en premier** et que le sous-arbre
gauche de ce test est vide. MWCC 1.3.2 construit un arbre équilibré et pivote
toujours sur l'indice n/2 :

```
cmpwi rX, 6 / beq / bge
cmpwi rX, 0 / beq / b
```

Pour que le pivot soit le cas minimal avec sous-arbre gauche vide, il faut
n = 1. Toutes les échappatoires ont été essayées et produisent une sortie
fausse identique octet pour octet : un `case -1:` de mise en forme (pivot
correct mais `cmpwi rX,-1` mort, 7 lignes de différence), un `default:` fusionné,
l'ordre des cas inversé, la suppression du `break` final. Il reste 5 lignes de
différence : c'est une vraie différence de codegen, pas un artefact.

**Le cas inverse fonctionne** : quand le pivot est bien le cas médian, un
`case 0:` explicite fait enraciner MWCC à 2 au lieu de 3 et le match passe.
C'est pourquoi la plupart des `switch` sur constantes de ce projet matchent ;
seule la forme « pivot minimal » est inaccessible.

### C5 — arbres de dispatch divergents sur une petite plage de cas

Pour un sous-arbre `{3,4}`, CodeWarrior émet deux instructions :

```
cmpwi rX, 4
bge  END
b    case3
```

c'est-à-dire un test `>=` comme borne hors-domaine. MWCC 1.3.2 en émet
toujours trois :

```
cmpwi rX, 4
beq  END
bge  case3
b    case3
```

L'instruction en trop décale tous les branchements suivants. L'ordre des
sources ne change rien : `0,1,2,3,4` et `0,1,2,4,3` sont tous deux re-triés par
MWCC, et fusionner `case 3` dans `default:` ne change rien non plus. **Tous les
corps de cas, jusqu'à l'épilogue inclus, sont identiques octet pour octet** :
seul l'arbre de dispatch diffère.

C'est une divergence CodeWarrior/MWCC distincte de C3 : C3 porte sur **le cas
testé en premier**, C5 sur **un test hors-domaine supplémentaire**.

### C6 — `sqrtf` en `extern inline` ajoute 16 octets de `.rodata`

`math.h` de MSL déclare `sqrtf` comme `extern inline`, et MWCC émet deux
statiques locales avec lui, `_half$localstatic3$sqrtf__Ff` (0.5f) et
`_three$localstatic4$sqrtf__Ff` (3.0f), pour 16 octets de `.rodata`. Tout objet
qui atteint `math.h` — directement, ou via `humath.h`, `game/hu3d.h`,
`game/hsfformat.h`, `game/sprite.h` — les porte.

`#define _MATH_H` avant le premier include supprime la définition inline et les
16 octets. Attention : le symptôme disparaît souvent en révélant un **second**
défaut, `.data` ou `.bss`, qui était masqué. Mesuré sur `m510dll/score`,
`m503Dll/score`, `m504dll/opening`.

Ce mur est un piège de diagnostic, pas une impossibilité : la cause est
identifiée et la parade existe.

## Caps partielles — une fonction dans une unité cappable

| unité | fonction | mur | lignes `!` restantes |
|---|---|---|---|
| `m510dll/REL/m510dll/ball` | `fn_1_509C` | constante `0x44003`plit par MWCC en `lis 0x4` + `addi 0x4003` ; l'original garde `lis 0x44` + `addi 0x3`. Le reste est une maison de pile imposée par une copie de paramètre. | 20 |
| `m510dll/REL/m510dll/ball` | `fn_1_51B4` | tie-break de registres : l'original donne r31 au pointeur entrant et r30 au compteur de boucle ; MWCC alloue les locals avant les paramètres. | 10 |
| `m510dll/REL/m510dll/ball` | `fn_1_5230` | même tie-break, r29/r30. | 8 |
| `m562dll/REL/m562dll/main` | `fn_1_FB0` | un `mr r3,r31` de plus avant un `bl`. Paramètre simple, cast `void*`, copie locale avec et sans initialiseur, affectation différée : tous mesurés, aucun ne gagne. | 1 |
| `m562dll/REL/m562dll/main` | `fn_1_1F54` | registres exacts, cadre 0x20 contre 0x10 : MWCC spill le paramètre entrant dès qu'un local en est copié. | 0 (cadre) |
| `m549Dll/REL/m549Dll/game` | `fn_1_AD78` | voir C3. | 5 |
| `m580Dll/REL/m580Dll/m580` | `fn_1_1EC4` | l'original fait `clrlwi` en place puis `li r6,1` ; MWCC remonte la constante d'abord et met le masque dans r0. Quatre formulations testées, toutes identiques. | 4 |
| `m511Dll/REL/m511Dll/camera` | `fn_1_9A0C` | voir C1. | 2 |
| `m511Dll/REL/m511Dll/camera` | `fn_1_B4C0` | voir C2. | 2 |
| `m579Dll/REL/m579Dll/zako` | `fn_1_16554` | voir C2. | 2 |
| `m580Dll/REL/m580Dll/m580` | `fn_1_1EC4` | tie-break de registres, ci-dessus. | 4 |

## Rétractations — des caps annoncées puis invalidées

Une cap qui repose sur une prémisse fausse coûte une session entière. Deux
prémisses de ce fichier ont été invalidées par la suite ; elles sont conservées
ici parce que le genre d'erreur se répète.

### R1 — « MWCC replie une constante 16 bits en un seul `li` » : FAUX

Ce « mur » a été annoncé deux fois avant d'être réfuté. La prémisse était que
`lis r4,0x4000`materialisait la constante `0x4000`. Non : `lis`materialise
`0x4000 << 16` = **0x40000000**. Le masque réel est **0x40000001** (bit 30 OR
`HU3D_ATTR_DISPOFF`), et MWCC 1.3.2 en émet `lis r4,0x4000` + `addi r4,r4,1`
**exactement**.

Les huit formulations proposées comme échappatoires — local `u32`, `volatile`,
`static const`, casts `(u32)` et `(s16)`, `0x4000` puis `a += 1`, `|` avec
promotion `unsigned long` — étaient toutes repliées, simplement parce que la
valeur écrite tenant réellement dans un immédiat 16 bits, un `li` unique était le
codegen **correct**. Le blocage était la constante, jamais le codegen.

Règle générale : **devancer la valeur complète d'un `lis`** (`imm << 16`, combiné
avec l'`addi`/`ori`/`subi` suivant et la règle de sign-extended-half) avant de
conclure qu'une constante est irréproductible.

### R2 — « m504dll/stage.c existe et est à 90 % » : FAUX

Cette unité n'a jamais existé sur aucune ref (`git log --all -- src/REL/m504dll/stage.c`
est vide). Le travail réel était dans `wanwan.c`. Vérification à faire avant
d'envoyer un agent sur une unité annoncée comme avancée :
`git log --all -- <chemin>`, et non `Test-Path` seul.

Un seul essai ne prouve rien. chacun de ces suspects a été trouvé par une seule
série d'essais ; il faut soit trouver la forme qui gagne, soit répéter la série.

| unité | fonction | ce qui a été essayé une seule fois |
|---|---|---|
| `sdroomdll/REL/sdroomdll/desk` | `fn_1_26F0C` | `fsub f2,f0,f2` contre le `fsubs f2,f0,f2` de l'original, à une conversion s16→f64. Paramètre f32 médian (ajoute `frsp`), paramètres f64 (donne `lfd` au lieu de `lfs`), cast `(f32)` explicite : tous worse. |
| `m521Dll/REL/m521Dll/gamemaster` | — | la mise en page d'un `.ctors` de thunks rend l'unité non plaçable, donc non identique. |
| `m511Dll/REL/m511Dll/boat` | — | déclaré `Equivalent` à −128 octets de code. Non résolu. |
| `m517Dll/REL/m517Dll/score` | — | le bloc `singleton.h` landé dans le mauvais endroit, et le `.rodata` qui en découle est laid avant padding. |

## Ce qui n'est PAS un mur

À titre de garde-fou, parce que ces diagnostics ont déjà coûté du temps :

- **Un pourcentage objdiff bas sans ligne `!`** : artefact de relocation. 2.7.1
  compte une instruction par `bl` vers un symbole du même objet pas encore écrit,
  et facture les offsets de `.rodata` qui ne peuvent pas s'aligner avant que le
  pool de littéraux de l'unité soit complet. 3.3.1 crédite plus. Mesuré sur huit
  unités.
- **Une queue de `.rodata` de 4 octets** : absorbée par le linkeur. Ne pas
  l'ajouter au source.
- **`lbl_1_bss_X` déclaré `extern`** alors que l'objet original le définit : le
  symbole est dans `B` ou `D` dans `powerpc-eabi-nm`. Le définir en local.
- **Une section N octets trop courte** : un item en trop, ou un trou d'alignement
  8. Regarder `nm -S`, pas deviner.
- **Un `.bss` émis en ordre inverse de déclaration** : rappel final, garder
  l'ordre des déclarations pour les symboles définis.

## Trame de rapport pour une cap

1. Unité, fonction, taille en octets.
2. Les instructions exactes qui diffèrent, côte à côte.
3. Le numéro de version de MWCC qui les produit, et celui de l'original.
4. Chaque formulation de source essayée, avec son nombre de lignes `!`.
5. La raison pour laquelle aucune ne peut arriver à zéro, formulée comme une
   contrainte sur ce que MWCC sait faire.
6. Ce qu'il faudrait pour trancher : une version de MWCC différente, ou une
   démonstration que la contrainte est absolue.