# MP5 decomp — session status

## 2026-10-04 — campagne « 12 agents en parallèle » (voir `docs/WAVE1.md`)

La campagne a remplacé le travail séquentiel par une flotte. Outillage créé :

| outil | rôle |
|---|---|
| `docs/AGENT_BRIEF.md` | le brief que chaque agent lit en premier. Tout le savoir codegen accumulé y est. |
| `tools/dispatch_list.py` | classe les unités `src=n` (sans source) et nomme leurs plus petites fonctions. Filtre les unités absentes de `configure.py`. |
| `tools/wt_new.ps1` | provisionne un worktree : git worktree, hardlinks `orig/`, copie `build/GP5E01_00` (229 Mo), copie `extern/musyx`, copie `build/tools`, `configure.py`. ~2 s. |
| `tools/wt_cfg.ps1` | recopie le `configure.py` **du main** dans un worktree et relance `configure.py`. |
| `tools/install_unit.ps1` | copie **un seul** fichier source d'un agent vers le main, en forçant `LastWriteTime`. |

Protocole d'intégration, dans cet ordre, et sans exception :
1. `install_unit.ps1` — le fichier source seul, **jamais** le `configure.py` de l'agent.
2. `python configure.py ...` une fois si le fichier est nouveau pour le main.
3. `ninja -j 4` doit finir **sans `FAILED:`** et **sans `error:`**.
4. `dtk shasum -q -c config/GP5E01_00/build.sha1` doit afficher `135 files OK`.
5. `python tools/unit_status.py <module> --no-build` **dans le main** : c'est la seule mesure qui fait foi. `fndiff.py` (objdiff 2.7.1) sous-estime ; ne jamais l'utiliser pour décider.

Un agent = un module = un worktree. Deux agents sur le même module se disputeraient
`.ninja_log`. Un module = un fichier = une unité de dispatch.

Piège récent, à connaître : un source peut compiler dans son worktree et **échouer**
dans le main, parce que le worktree porte des fichiers que la branche principale n'a
pas (`src/REL/m530Dll/kinomi.c` incluait `REL/m530Dll.h`, absent du main). Règle
donnée aux agents : n'inclure un en-tête REL que s'il existe aussi dans le main.

Dernier état mesuré : **28.53 % de code**, **6 726 / 21 157 fonctions** identiques,
**346 / 881 unités** liées, `135 files OK`. Début de session : 28.34 % et 6 551
fonctions. 33 unités fermées sont passées de `[no compiled source]` à mesurables.

---

Last updated: 2026-10-03. Branch `decomp/m524Dll`, HEAD `1cc89b2`.
All work is local, nothing pushed. DOL units are serial (main checkout only); REL can parallelise.

## Campaign metric (from `build/GP5E01_00/report.json`, 881 units)

| scope | code matched | data matched | linked |
|---|---|---|---|
| All | 2 671 940 / 9 481 492 (28.18 %) | 620 075 / 2 644 182 (23.45 %) | 346 / 881 |
| DOL | 1 478 600 / 1 730 512 (85.44 %) | 515 474 / 926 941 (55.61 %) | 232 / 307 |
| Modules | 1 193 340 / 7 750 980 (15.40 %) | 104 601 / 1 717 241 (6.09 %) | 114 / 574 |

Session start was 2 653 972 (27.99 %) / modules 1 175 372 (15.16 %), 6351 functions.
Net session gain so far: **+17 968 code bytes, +37 functions, +0.24 points**.

`CHECK config\GP5E01_00\build.sha1` passes. That rule is a genuine **output** check
(`dtk shasum -q -c` over `main.dol` + every `.rel`), and `build.sha1:1` holds
`ba946b5ba8d6c9e6e6c378f8e3215cbb8ae5ffc9`, the public GP5E01 DOL hash. So the ROM check is real.

## Acceptance criterion
`matched_code` 100 % + `matched_data` 100 % + 6635/6635 functions + 408/408 units + valid SHA-1,
except units proven inaccessible, which go in `docs/capped.md`.

## Confirmed facts / settled questions

- **`Equivalent` is not fraud.** Units declared `Equivalent` in `configure.py` can show low
  `matched_code` (e.g. `w04Dll/world04` 5.7 %, `w02Dll/world02` 1.9 %) because their sources are
  faithful transcriptions marked `NONMATCHING: complete and faithful`, i.e. semantically correct but
  not byte-identical. Low byte-identity is the *definition* of `Equivalent`, not evidence of empty stubs.
  Check the source size before accusing.
- **`next_targets.py` column `stat` comes from `configure.py`, not objdiff** (`configure_status()`,
  tools/next_targets.py:47-53). Its `pct` column is only meaningful for `Matching`/`NonMatching` units.
- **objdiff symbol names are cosmetic — REFUTED idea.** Renaming static data symbols
  (`MotParamTbl` -> `lbl_1_data_37C`, `ReadyPosTbl` -> `lbl_1_data_454`) in `m570Dll/player` changed
  nothing: still 4036/7592, 17/24, fuzzy 98.88 %. objdiff resolves symbols by address, so the `~`
  label-only rows in a diff do not cost matched bytes. Do not spend time renaming. Reverted.
- **`fndiff.py` needs objdiff 2.7.1.** It passes `-o`, which objdiff 3.x removed; a 3.x build replies
  "Unrecognized argument: -o" and prints no diff at all. The tool prefers
  `build/tools/objdiff-cli-2.7.1.exe`, falling back to `objdiff-cli.exe`. Do not overwrite
  `objdiff-cli.exe` itself — `configure.py` needs 3.x to generate `report.json`.

## Unit-wide defects that pay off (the productive pattern)

Chase these before per-function tweaking; one root cause fixes dozens of functions.

- a `for` counter declared `s16` forces an `extsh` pair and a wider `cmpwi`; the original uses `s32`
- dead conjuncts in a condition — MWCC evaluates the boolean at runtime via `subf`/`cntlzw`/`srwi`;
  deleting the dead side matches
- giving a named local to a call result forces the callee-saved register copy the original has
- a `switch` needs an explicit `case 0:` before MWCC roots its binary search at 2 instead of 3
- loop form matters: `for (j = 0, mat = h->material; j < n; j++, mat++)` vs split initialisation

## Paired-single only reachable through inline asm (DOL, MWCC 1.2.5n)

The DOL builds at `-O4,p` with `-lang=c`, which does **not** make MWCC emit
`psq_l` / `ps_add` / `ps_muls0` from plain C. Verified by compiling a probe
containing four spellings -- struct with three floats, struct with four floats,
`f32*` arrays of 3 and 4 elements, plain `out[i] = a[i] + b[i]` -- and counting
`ps_` instructions in the result: **zero**. So any DOL unit whose original uses
paired-single cannot be reproduced with plain C; it has to be inline asm, the
way `src/dolphin/mtx/mtxvec.c` already is.

Two further constraints on such a block:

- `nofralloc` suppresses the frame but **not** the `blr`. Omitting the explicit
  `blr` silently drops the last instruction and every function lands at
  83-95 % fuzzy with **zero** matched bytes, which reads as "close" while
  contributing nothing.
- `objdump` from `build/binutils` decodes `psq_l f2` as `lq r2` and `ps_add` as
  `vsel`/`xsaddsp`. The bytes are correct. Do not chase that as a defect.

Also: `static const f32` naming the float literals does **not** reproduce the
original `.sdata2` layout -- they get folded back into inline literals and each
`lfs` in an asm block becomes its own padded entry.

Worked example: `main/dolphin/mtx/vec`, 0 / 904 B and 0 / 12 functions to
408 / 904 B and 9 / 12 functions, with the ten `PSVEC*` routines in asm and
`C_VECHalfAngle` / `C_VECReflect` left in plain C because the original writes
those two in scalar code.

## Known toolchain walls
MWCC 1.3.2 vs CodeWarrior `0b 02 04 07`; assert in `singleton.h` (`cmpwi` vs `cmplwi`);
r29/r30 tie-breaks (24 `d_unsigned` orders); thunks in `.ctors`; dead `mr r10,r3`.

Two more measured 2026-10-05, do not re-explore:

- **score-constructor register swap.** `m510dll/score` `fn_1_CE04` is 82.91 % and
  *all 40* differing rows are one defect: the original keeps `score` in r30 and
  `id` in r29, MWCC 1.3.2 puts them the other way round. Swapping the two
  declarations changes nothing, in either direction. `m575dll/score` is the same
  family.
- **A more faithful spelling can score worse.** `m575dll/score` `fn_1_3A7C` is
  99.34 % on a single row: the original passes the local `esp` to `espDispOff`
  where the source passes `self->espB`. The two are equal by construction
  (`self->espB = esp;` two lines earlier), but using the local keeps `esp` live
  and flips `score` out of r30, costing 15 rows. The field access is the spelling
  that matches; keep it.

## CRITICAL: never trust an agent's "before" number

Worktrees are branches off older commits. An agent measures its baseline *inside its own worktree*,
which can be far behind the main checkout, so its "before -> after" can describe work the main
checkout **already contains**. Confirmed twice this session:

- `m518dll/m518`: the agent reported 110/122 -> 113/122, 59.87 % -> 63.87 %, landing `fn_1_8E2C` at
  100 % and `fn_1_83FC` at 99.36 %. Main was *already* 113/122, 63.87 %, 38 236/59 864 bytes with
  `fn_1_8E2C` at 100 % and `fn_1_83FC` at **99.96 %** — strictly better. Its work was not integrated;
  integrating it would have been a small regression.
- Rule: before integrating anything, run `python tools\unit_status.py <unit>` in the **main
  checkout** and compare per-function, not just totals. A worktree file can be a different size and
  still match no better.

## CRITICAL: scope agent dispatches to a handful of named functions

Three agents have now died on context exhaustion, not on the decomp problem itself:
`m540Dll/m540` twice on "Compaction did not run: the model returned an empty summary", and
`systemDll/nel` on "Tool calls cutoff by max_tokens". All three were whole-unit dispatches on large
units (m540 is 88 KB / 226 functions, nel 116 KB, mdmini 100 KB).

Consequence: **do not dispatch a whole unit.** Name 3-5 specific functions instead, ascending by size,
and tell the agent explicitly to leave the giant ones alone. One file per agent, so two agents never
produce conflicting copies of the same source file and integration stays a clean single copy.

An agent that dies is not necessarily a total loss — both dead m540 agents left real, integrable work
(the second one was worth +1120 bytes and +6 functions despite dying mid-task). Always check
`git diff --stat -- src include` in the worktree before discarding a failed agent, and measure the
result in the main checkout before keeping it.

## Open issue: object -> REL propagation

After integrating `m540Dll/m540.c`, ninja recompiles `obj\REL\m540Dll\m540.o` and objdiff re-measures
(a real +2776 B gain), but **no relink happens** and `CHECK` never re-runs — ninja reports "no work to
do". Forcing a fresh timestamp on the source does not change this. In `build.ninja`,
`m540Dll.preplf` does list `obj\REL\m540Dll\m540.o` as an input, and `m540Dll.rel` depends on
`m540Dll.plf` plus `main.elf`. So `obj/REL/<mod>/<unit>.o` appears to be an objdiff-side object that is
not what actually feeds the linked `.rel`.

Consequence: `CHECK` can pass against a `.rel` that is stale with respect to recent source edits.
Verified state as of this session: `dtk shasum -q -c config\GP5E01_00\build.sha1` reports
`135 files OK` when run explicitly, and `build.sha1:1` is
`ba946b5ba8d6c9e6e6c378f8e3215cbb8ae5ffc9`, the public GP5E01 `main.dol` hash. Do not treat the
`CHECK` edge as proof that a given source edit is covered; run the explicit shasum and understand
that objdiff metrics, not the link chain, are the campaign's working measure.

## Regenerating build.ninja

Pass the **binaries**, not the tool directories — a directory is treated as a source tree to build
from and configure.py fails looking for `Cargo.toml`:

```
python configure.py --dtk G:\decomp\MP5\build\tools\dtk.exe --objdiff G:\decomp\MP5\build\tools\objdiff-cli.exe --sjiswrap G:\decomp\MP5\build\tools\sjiswrap.exe --binutils G:\decomp\MP5\build\binutils --compilers G:\decomp\MP5\build\compilers
```

This is mandatory after adding a source file for a unit that had none: the generator skips objects
whose source is absent, so the compile edge is missing and the unit reports `[no compiled source]`
even though a link edge exists.

## Dispatch outcomes (audit trail)

- **m570Dll/player — INTEGRATED and verified.** 7/24 -> 17/24 functions, 1356 -> 4036 / 7592 bytes
  (17.86 % -> 53.16 %). Re-measured independently in the main checkout: identical.
  Remaining 7 functions = 3556 bytes, all 90.66-99.91 %, all blocked on the same codegen difference:
  the original emits `cmpwi r0,5` + `bne +4` (branch to the next instruction) before the shift-start
  `lfs`, while MWCC merges the two successors into a `beq` over the block. Six source formulations
  were built and measured, all worse. `fn_1_58EC` is separate: `Hu3DMotionMaxTimeGet` is called as
  `f32` there and as `s32` elsewhere, and MWCC rejects both declarations.
- **w04sDll/world04 — REJECTED, not integrated.** It wrote 5 empty stubs covering 24 804 bytes,
  then declared the unit `Object(Equivalent, ...)`. Zero matching progress (41.87 % before and
  after). Its report contradicted its own table. The only salvageable nugget is a constant fix in
  `MB4_CoinDispUpdate` (`0x8C2D` -> `0x8C002D`), left on the shelf.
- **storyresultdll/rank — ZERO WORK.** Empty report, and `git status` confirms no `src/` change at
  all; only scratch logs and untracked tool scripts were left. An empty result must still be verified
  against `git status` before being believed.

- **mdminiDll/mdmini — INTEGRATED and verified.** Main had **no source at all** for this unit:
  `[no compiled source]`, 0/58 functions, 100 540 B. The agent wrote `src/REL/mdminiDll/mdmini.c`
  (22 558 B) from scratch. Now 16/58 functions, 2756/100 540 bytes (2.74 %) — exactly the reported
  numbers. Required adding the file **and** re-running `configure.py` (see above); the SHA stayed valid.
  Its report also records that MWCC **inlines any callee whose body it has already compiled and emits
  it first**, so callers must be defined *above* their helpers — this is why `mdparty.c` defines
  `MdInit`/`MdClose` after `MdMain`. Defining them the other way silently inlined MdInit into MdMain
  and cost the `MdMain` match with no compile error. Also: `frandmod` must stay *undeclared*; typing it
  `u32` suppresses the `xoris r0,r0,0x8000` pair needed by the implicit-int -> float conversion.
- **m540Dll/m540 — INTEGRATED and verified, from a dead agent.** The agent died twice on a
  compaction error, but left real work. Measured in its worktree: 35 600/88 204 (40.36 %), 177/226
  functions, against main's 37.2 %. Copied **only** `src/REL/m540Dll/m540.c`; that worktree's
  `configure.py` is contaminated with the rejected w04sDll `Object(Equivalent)` fraud and must never
  be copied.
- **m518dll/m518 — NOT integrated, main is already better.** See the stale-baseline warning above.
  The unit sits at 113/122, 38 236/59 864 (63.87 %) in main. Still genuinely unclaimed there:
  `fn_1_4B80` (8332 B), `fn_1_6E08` (2816 B), `fn_1_7908` (2784 B), `fn_1_88C8` (1368 B) — 15 300 B,
  ~83 % of what remains.

- **Spill pressure can run the wrong way.** In `fn_1_6E08`/`fn_1_7908` the original *spills* values that our
  build keeps in registers (target frame `-0xb0` vs ours `-0xa0`; target stores a struct address once
  `addi r17,r1,0xa` + `mr r4,r17`, ours stores/reloads the address; target spills
  `minX/minZ/maxX/maxZ` to slots `0x8..0xb` and reloads with `lbz r0,0xa(r1); extsb r4,r0`, ours holds
  them in `r17`-`r19`). So our live-value pressure is LOWER than the original's. The counter-intuitive
  fix is to *increase* pressure — keep values live across calls, avoid narrowing types, add computations
  that force spills — not to reduce register usage.
- **The magic-base float idiom**: `lfd f1, <4503601774854144>` (exactly 2^52 + 2^31), `xoris rX, 0x8000`,
  store/reload the magic base, `fsubs`, then `lfs <0.0f>` and `fcmpo`. Operands are s32 deltas in
  [-18, 18]. `2^52 + (v ^ 0x8000) - (2^52 + 2^31)` does NOT reduce to `v`, so this is neither a plain
  sign test nor `200.0f * <s16>`. Unsolved; probably shared across the unit.
- **Recovering the original's unnamed constant tables**: grep `^\.obj lbl_1_rodata_` in `m518.s`. The
  target `.o` has zeroed section file-offsets, so ELF reads fail and objdiff cannot show the data.
- **Useful reference point**: `fn_1_6C0C` is the already-matching twin of `fn_1_6E08`'s first loop; when
  one function matches, its structural twin often reveals the shape of the other.

## Recorded codegen walls

- **MWCC allocates GPR locals in REVERSE declaration order** — the last-declared local gets the
  lowest temp register number. Reordering declarations is semantically neutral and flips a
  register-allocation-only diff to zero differing rows (verified: `fn_1_9080` 86.44 % -> 93.22 % ->
  100 % by moving `s16 result;` above `s32 best;`). Diagnostic: a cascade of pure register-number
  swaps with identical opcodes and one consistent offset direction.
  Caveat: it does **not** apply where a function has no locals — `mdminiDll/ObjectSetup` has none, so
  its 91-row register cascade has another cause and resisted texbuf sizes, global order and
  `_prolog`/`_epilog` placement.
- **The A268 result mask**: target is `clrlwi rX,r3,24` + `rlwinm r0,rX,0,25,25`; MWCC always narrows
  to 16. Affects `fn_1_83FC`, `fn_1_CEA0`.
- **objdump inverts the printed bit position for single-bit `rlwinm`** — `& 0x40` prints `25, 25`,
  `& 0x02000000` prints `6, 6` (31-n either way). Do not trust the printed bit number.
- **Explicit `(double)` casts break float matching**: with them MWCC emits `fmul`/`fsub` + `frsp`;
  without them the correct single-precision `fmuls`/`fsubs`.

## 62-worktree scan (unintegrated work)

`git status --porcelain -- src include` across every worktree, then SHA-256 compared to the main
checkout:

- **Already integrated** (`SAME-AS-MAIN`): `m529dll/m529.c`, `m563dll/m563.c`, **`m571dll/m571.c`**
  (the earlier 36/87 -> 46/87 session was in fact merged). Nothing to recover.
- **Divergent but not better**: `m504dll/com.c`, `m538Dll/m538.c`, `m577Dll/stagedata.c`
  (only 56 B missing — negligible), `m579Dll/game_master.cpp`, `w02Dll/world02.c` (already `Equivalent`).
- **New files absent from main**: `m530Dll/player.c`, `m531Dll/audio.cpp`, `m532Dll/game_master.cpp`,
  `m539Dll/drum.cpp`, `m576Dll/score.c`.
- **Best find**: `m518dll/REL/m518dll/m518` is at 38236/59864 (63.87 %) but **three functions have no
  body at all** — `fn_1_4B80` (8332 B), `fn_1_6E08` (2816 B), `fn_1_7908` (2784 B) — 13 932 bytes
  never written, plus 8 functions at 97-99.9 %. Both the worktree and main lack those bodies, so this
  is genuinely unclaimed work. Dispatched.
- Some worktrees have a stale `build.ninja` pointing at `D:/decomp/MP5` and cannot build; they need
  `configure.py` re-run or main's `build.ninja` copied in.

## Currently dispatched (background agents)

| unit | main baseline | worktree |
|---|---|---|
| `systemDll/REL/systemDll/nel` | 0 %, 116 136 B | `mp5-wt/m570dll` |
| `m540Dll/REL/m540Dll/m540` | 40.36 % after integration, 52 604 B missing | `mp5-wt/w04sDll` (resume pending) |

Best remaining free targets, in case more agents are dispatched (all measured in the main checkout):
`mdbeachDll/mdbeach` 94 932 B, `m541Dll/game` 94 172 B, `mgmatchDll/mgmatch` 74 756 B,
`m551Dll/game` 68 096 B, `sd00dll/stage` 64 056 B, `m535Dll/m535` 48 856 B.

Integration rule: copy the unit's source file into the main checkout, `ninja -j 4` must end
`135 files OK`, then re-measure with `tools\unit_status.py` **in the main checkout** and compare
per-function against main's current state. Never trust an agent's self-reported numbers or baseline.
Never copy a worktree's `configure.py` without checking it — some are contaminated.
Never write empty function bodies or `Object(Equivalent)` to make a unit look finished.