# Agent brief — décompilation Mario Party 5

Lisez ce fichier une fois, puis travaillez. Il porte les leçons accumulées de
toutes les sessions précédentes ; le suivre est ce qui fait tenir un agent
jusqu'au bout.

Le projet est une décompilation open-source de Mario Party 5 (GameCube, PowerPC,
CodeWarrior). Vous reconstruisez le C à partir de la décompilation du projet
lui-même pour que les sources recompilent l'exécutable original **octet pour
octet**.

## 1. Où vous travaillez

| quoi | valeur |
|---|---|
| votre worktree | `G:\decomp\mp5-wt\<module>` (donné dans votre tâche) |
| votre branche | `decomp/<module>` |
| checkout de l'orchestrateur | `G:\decomp\MP5` — **jamais** y écrire |
| outils de mesure | `G:\decomp\MP5\tools\*.py` — lisibles, jamais modifiés |

Chaque commande doit tourner avec `workdir` = votre worktree. Un module = un
worktree : ne lancez jamais deux agents sur le même module, ils se disputeraient
`.ninja_log`.

## 2. La règle qui gouverne tout

`ninja` doit finir sans `FAILED:` et le shasum doit valider :

```powershell
& "G:\decomp\MP5\build\tools\dtk.exe" shasum -q -c config\GP5E01_00\build.sha1
```

C'est le seul contrôle de sortie réel. Un `Matching` est lié dans le REL et un
octet faux casse la somme SHA-1.

## 3. La boucle

1. Lire UNE fonction :
   `python tools\fnasm.py <unit> <symbol>`  (`--list` pour énumérer)
   **Ne jamais ouvrir un fichier `.s`, même en portion.** C'est ce qui tue les
   sessions. Jamais de dump d'unité entière.
2. Brouillon machine (facultatif, sortie jetable, noms `temp_r3`) :
   `python tools\m2c_draft.py <unit>`
3. Écrire le C dans `src\REL\<module>\<fichier>` **dans l'ordre des adresses**.
   Jamais ajouter des fonctions à la fin. `#include "types.h"` si le module n'a
   pas d'en-tête REL, et déclarez vous-même les externs dont vous avez besoin,
   dans ce fichier, jamais dans un en-tête partagé.
4. Un **nouveau** fichier source demande **une** fois :
   ```
   python configure.py --dtk G:\decomp\MP5\build\tools\dtk.exe --objdiff G:\decomp\MP5\build\tools\objdiff-cli.exe --sjiswrap G:\decomp\MP5\build\tools\sjiswrap.exe --binutils G:\decomp\MP5\build\binutils --compilers G:\decomp\MP5\build\compilers
   ```
   Ne modifiez pas `configure.py`.
5. Construire **seulement** votre objet :
   `ninja -j 2 build/GP5E01_00/src/REL/<module>/<fichier>.o`
6. Différencier une fonction :
   `python tools\fndiff.py <unit> <symbol>`
   Original à gauche, votre build à droite, avec la ligne C d'origine.
   `!` = vraie différence. `~` = nom de libellé ou littéral seul, sans coût.
   `--max-diff 20` et `--rows 0:80` pour paginer.
7. **LE VERDICT EST « 0 differing (!) »** — voir la section 5, c'est le point le
   plus important de ce brief.
8. Mesurer : `python tools\unit_status.py <unit> --no-build`
   (objdiff 3.3.1, la vérité pour le crédit ; c'est aussi ce que l'orchestrateur
   utilise pour décider de l'intégration.)

## 4. Les trois statuts, et ce qu'il ne faut pas faire

| statut | sens |
|---|---|
| `Matching` | tout est identique, octet pour octet ; lié dans le REL |
| `Equivalent` | code complet et fidèle, au moins une fonction non identique |
| `NonMatching` | incomplet ou absent |

Ne modifiez **jamais** un `Object(...)` dans `configure.py`, et ne déclarez jamais
`Equivalent` ou `Matching` : l'orchestrateur le fera après vérification. Ne
livrez jamais un corps vide, un `...`, ni un `// NONMATCHING:` à la place de la
logique réelle. Si une fonction ne peut pas être rendue identique, laissez le
corps fidèle plus un commentaire d'une ligne qui nomme la différence.

## 5. Le point le plus important : votre pourcentage n'est pas le crédit

`fndiff.py` pilote objdiff-cli **2.7.1**. Le crédit vient de `report.json`
(objdiff **3.3.1**). Les deux ne sont pas d'accord et 3.3.1 crédite **plus**.
Mesuré sur 8 unités de la vague du 2026-10-04 :

| ce que l'agent rapporte | ce que objdiff 3.3.1 crédite |
|---|---|
| 91.67 / 85.71 / 80 % | les trois fonctions à 100 % |
| 91.43 % | 100 % |
| 82.35 / 92.86 / 93.10 % | les cinq fonctions à 100 % |
| 48 / 52 / 66 % (24 lignes `~`) | les trois fonctions à 100 % |

Cause : 2.7.1 compte une instruction non appariée par `bl` vers un symbole défini
dans l'objet original mais pas encore dans votre `.o`, et il facture les offsets
de littéraux `.rodata` qui ne peuvent pas s'aligner avant que le pool de
littéraux de l'unité soit complet.

**Donc : rapportez `0 differing (!)` par fonction. Un pourcentage bas avec zéro
ligne `!` est un artefact de relocation ; réécrire le corps dans ce cas gaspille
le budget.**

## 6. Faits de codegen vérifiés (MWCC `-O0,p`)

Ordre et typage — le plus souvent la cause d'un diff résiduel :

- **Les locals sont alloués d'abord**, premier déclaré -> r31, puis r30, r29,
  r28. **Les paramètres sont alloués APRÈS tous les locals**, en ordre de
  déclaration inverse, et **tout local initialisé depuis une variable de paramètre
  nue reçoit une maison sur la pile** — d'où la paire `stw`/`lwz` et un cadre plus
  grand. Réordonner des déclarations est sémantiquement gratuit et fait souvent
  tomber un diff de simple allocation de registres à zéro différence.
- `s16` / `u16` / `s32` et signé vs non signé changent `extsh`, `rlwinm`,
  `cmplwi`. Un champ chargé par `lha` doit être `s16`. Un champ testé par
  `cmplwi` doit être `u32` ou pointeur. Un membre d'union 16 bits doit être *lu*
  comme tel ; un cast `(s16)` explicite ne donne pas `lha`.
- `if (x) {} else {}` et un ternaire ne donnent pas les mêmes branches.
  `x--` puis test coûte un `lwz` de plus que `(x -= 1) < 0`.
- Une suite de comparaisons contre **un même champ de struct** dans l'original
  vient d'un `switch`, pas d'une chaîne de `if` : MWCC recharge le champ à chaque
  test dans une chaîne de `if`, l'original non.
- Une suite de comparaisons contre **une même constante** demande un `case 0:`
  explicite pour que MWCC enracine sa recherche binaire à 2 au lieu de 3.
- Un test de nullité demande un **cast typé** pour obtenir `li r0,0` + `cmplw` ;
  un zéro non typé donne `cmplwi`. Essayer délibérément `p == NULL`, `p != 0`,
  `(T *)0 != p`.
- `x > C` et `C < x` donnent une branche **d'une instruction** (`ble`/`bge`).
  `x <= C` donne `fcmpo` + `cror eq,lt,eq` + `beq`. Une cible qui montre
  `cror eq,lt,eq` utilisait un `<=` simple.
- Boucle décompte : quand la latch cible est `cmpwi` **avant** `subi`, écrire
  `i = N; while (i--) { }`. `for (i=N;i>0;i--)`, `for (i=N;i!=0;i--)`,
  `for (i=N;i!=0;--i)` et `while (i!=0){ body; i--; }` émettent tous `subi` en
  premier. Une boucle ascendante `for (i=1;i<len;i++)` matche telle quelle.
- Une valeur de retour constante après un appel doit vivre dans un local
  (`s32 ret = 1; f(); return ret;`), sinon MWCC émet `li r3,1` au lieu de la
  copie callee-saved que l'original a.
- `return (fa < fb);` n'est PAS `mfcr`/`srwi` : il faut
  `if (fa < fb) { return 1; } else { return 0; }`.
- objdump affiche les positions de bit d'un `rlwinm` mono-bit **inversées** :
  `& 0x40` s'affiche `25, 25`. Mapper par 31-n.

Données :

- **Un symbole que l'objet original DÉFINIT doit être défini dans votre source,
  pas déclaré `extern`.** Un `lbl_1_bss_XXXX` / `lbl_1_data_XXXX` qui apparaît en
  `B` ou `D` dans `build\GP5E01_00\<mod>\obj\<chemin>.o`
  (`build\binutils\powerpc-eabi-nm.exe`) appartient à votre unité de traduction.
  Le déclarer `extern` compile, ne lie rien localement, et laisse sous 100 %
  toutes les fonctions qui y touchent. Longueur réelle via
  `powerpc-eabi-objdump.exe -h`. Même règle pour `.data` et `.rodata`.
- **MWCC met les littéraux flottants dans le pool dans l'ordre de lecture du
  source, CodeWarrior dans l'ordre inverse.** fndiff rapporte le décalage en
  lignes `~` anodines alors que c'est une vraie différence de 6 instructions.
  Correctif mesuré : mettre `(void)0.0f;` en tête de fonction pour amorcer le
  pool avec le bon littéral, sans instruction émise. `static const` et
  `static volatile const` sont repliés en littéraux et n'aident pas.
- Un `const double` utilisé directement est plié en copie anonyme `@N` et perd la
  relocation nommée ; le lire via un lvalue `volatile` préserve le symbole et le
  chargement.
- Le `.bss` et le `.data` incomplets ne font pas perdre le crédit des fonctions :
  objdiff crédite `matched_code` fonction par fonction.
- Une section courte de 4 octets peut être du padding d'alignement entre objets,
  pas une définition manquante : la bonne action est de **supprimer** la
  déclaration fautive, pas d'inventer des octets.
- `char name[N]` : les tailles sont porteuses de sens pour `.data`. MWCC ne
  complète pas la section.

Flottants :

- Les casts `(double)` explicites cassent le match flottant : ils forcent
  `fmul`/`fsub`+`frsp` au lieu des `fmuls`/`fsubs` corrects. La décision est prise
  **par expression**, pas par fichier.
- Une multiplication flottante lie la destination à l'opérande **gauche** :
  écrire `0.1f * (x / 10000.0f)`.
- Une mise à jour flottante appartient à sa condition :
  `if ((p->f += 0.02f) >= 1.2f)` garde la valeur arrondie dans un FPR, alors
  qu'une affectation à un `f32` nommé la spill d'abord.
- Un struct local est le moyen de forcer une paire de flottants sur la pile.
- `4503601774854144.0` est la constante de biais int->double de MWCC et demande
  un cast `(f64)` explicite pour reproduire `lis`/`xoris`/`lfd`.

Fonctions et structure :

- **MWCC inline toute fonction dont le corps est déjà compilé**, et l'émet
  **en premier**. Si un helper que vous avez défini se retrouve inliné dans son
  appelant sans erreur, déplacez l'appelant **au-dessus** du helper.
- `#pragma dont_inline on` a un piège de placement : la région doit commencer
  **après** `_prolog` / `_epilog` / `ObjectSetup`, sinon la première fonction
  part dans sa propre section `.text`.
- Ne mettez **pas** `#pragma dont_inline on` à l'échelle du fichier dans une unit
  de type `player` / `camera` / `gamemaster` : MWCC émet alors une copie
  hors-ligne de chaque constructeur trivial et grossit `.text`.
- Un forwarding function doit ne déclarer **aucun** paramètre, sinon MWCC émet un
  `stw r3,0x8(r1)` que l'original n'a pas ; et il faut `#pragma noinline`, sinon
  le forwarder disparaît.
- Un corps de fonction vide est parfois le bon corps, quand la cible est un seul
  `blr`. Vérifier dans le diff avant de conclure le contraire.
- `extern "C"` dans un `.cpp` : les prototypes à portée de fichier doivent être
  **à l'intérieur** d'un `extern "C" { }`.
- Les champs de struct sont parfois des unions. Jamais supposer un offset : le
  redériver de l'assembly.

## 7. Murs d'outillage — ne pas y brûler de session

L'original est CodeWarrior `0b 02 04 07 01 01 02 00` ; notre `mwcceppc` installé
émet `0a 02 04 02 01 00 02 00`. **Ce seul écart décide de plusieurs choses que le C
ne peut pas réparer :**

1. le bloc `assert` de `singleton.h` : l'original le place **hors ligne**
   (`cmpwi`/`beq`/`b` vers un bloc froid) et MWCC 1.3.2 l'inline. 21 formulations
   et 21 versions de MWCC ont été essayées, toutes inlinent. Conséquence : une
   fonction contenant cet assert ne peut probablement pas atteindre 100 %.
   Écrivez-la fidèlement, mettez le commentaire, passez à autre chose.
2. **L'argument de `__ptmf_scall`** : l'original passe le PTMF en `r12` (l'ABI
   du builtin CodeWarrior) avec un `nop` après le `bl` ; MWCC utilise `r4` et pas
   de `nop`. Un `__asm` inline reproduit les instructions mais casse
   l'allocation de registres autour.
3. Certains **tie-breaks de registres** : 24 ordres de déclaration de quatre
   locals donnent exactement deux classes, aucune n'atteint la cible. Quand vous
   voyez ça, arrêtez de permuter.
4. Le **padding de queue `.rodata`** : une queue de 4 octets que aucun symbole ne
   couvre est absorbée par le linkeur. Ne l'ajoutez **jamais** au source.
5. Un `mr r10,r3` **mort** (copie du résultat d'un appel `void` ignoré) que
   MWCC 1.3.2 supprime.
6. Les **thunks `.ctors`** : les initialiseurs statiques ne sont pas
   plaçables, donc ces unités ne peuvent pas être identiques.

Le reste des suspects, par ordre de fréquence : le trap des 16 octets de
`sqrtf` (`#define _MATH_H` avant tout include `game/` ou `math.h` — mais ça
révèle souvent un **second** défaut, `.data` ou `.bss`), un `.bss` émis en ordre
inverse de déclaration, une section N octets trop courte à cause d'un item en trop
ou d'un trou d'alignement 8.

## 8. Budget de contexte — la première cause d'agents morts

Neuf agents sont déjà morts de `Compaction exhausted`, de résumé vide, ou de
coupure de sortie d'outils. **Aucun n'est mort de la décompilation.** La cause est
le volume de sortie.

1. Jamais de `.s` en entier, jamais de dump d'unité.
2. `fnasm.py --list <unit>` vide le budget : toujours filtrer.
3. **Une ou deux fonctions par appel d'outil.** Jamais de balayage.
4. Gardez un `notes.md` non commité à la racine de votre worktree, une ligne par
   fonction : `module | fn | commit | 0 ! rows`. Une lecture relit le contexte.
5. `-j 2` : la flotte partage 16 cœurs.
6. Si votre contexte se remplit : arrêtez, commitez, rapportez. Un résultat
   partiel commité vaut mieux qu'une session perdue.

## 9. Règles de travail

- Ne demandez pas de décision et ne présentez pas d'options. Le classement
  nécessaire est déjà devant vous ; choisissez, exécutez, rapportez. Un résultat
  négatif avec mesures est un bon résultat. Une question n'en est pas un.
- Ne réécrivez pas le fichier sur lequel vous travaillez. Avant de commiter :
  `git diff --numstat` doit correspondre à votre vrai changement.
- Un commit par fonction ou petit groupe. PowerShell massacre les `-m` multi-lignes :
  écrivez le message dans un fichier et utilisez `git commit -F`.
- Ne touchez qu'un seul fichier source. Si un en-tête partagé doit changer,
  signalez-le dans le rapport au lieu de le modifier.
- `notes.md` et `msg.txt` ne sont jamais commités. `tools/` n'est jamais commité.
- Ne réécrivez jamais un `configure.py` venant d'un worktree vers le main.

## 10. Rapport (moins de 25 lignes, pas d'assemblage, pas de sortie d'outil)

1. Chaque fonction visée : nom, taille, et si elle a 0 ligne `!`.
2. Pour celles qui n'en ont pas : une ligne sur ce qui diffère vraiment.
3. Le chemin et l'extension du fichier créé, sa taille, la confirmation que
   `git status --short` est propre pour lui, et ce que `configure.py`
   enregistre pour cette unité.
4. `git diff --numstat`.
5. Les hashes courts des commits.
6. Tout ce que vous avez dû supposer (offsets de struct, signatures d'extern,
   DATANUM), et tout pad de données inventé.