# w07Dll / REL/w07Dll/world07 — triage of the remaining 43,592 bytes

Measured 2026-10-03 on `decomp/w07Dll`, branch created at 2235890.

## Headline

**The 29 failures were wrong code, not unwritten functions.** All 29 had
source in `src/REL/w07Dll/world07.c` (3019 lines) from an earlier session,
and all 29 carried a `// NONMATCHING: register allocation order differs`
comment. That uniformity was the tell: 29 independent functions do not all
mis-allocate registers by chance, so the cause had to be unit-wide.

It was. Two unit-wide structural defects, both fixed this session:

| section | before | target | after | verdict |
|---|---|---|---|---|
| `.text` | 54480 | 46268 | 48480 | +2212 (was +8212) |
| `.rodata` | 1164 | 640 | 660 | +20, fully explained |
| `.data` | 410 | 916 | 914 | −2, the `gap_04` hole |
| `.bss` | 836 | 834 | 836 | +2, see below |

`.rodata` +20 = MSL's 16-byte `_half`/`_three` `sqrtf` pair plus a 4-byte
tail no symbol covers (BRIEF §9: never pad the source to close it).
`.data` −2 = `gap_04_0000030E_data`, inter-object 8-byte alignment that
`dtk rel make` regenerates itself. `.bss` +2 has not been chased.

**Neither defect moved the identical-function count**, which stayed 23/52
and 2676/46268 code bytes. The 23 identical functions are all small
helpers; the bytes are all in the big board-event functions. But both
defects were *prerequisites*: no amount of register tweaking reaches 100 %
on a function that is 1220 bytes too long because its callees were
inlined into it.

## What remains, by size band

Ranked by target bytes. `d` = rebuilt − target.

### Multi-kilobyte (11 functions, 31,432 B)

| function | target | rebuilt | d | % | note |
|---|---|---|---|---|---|
| MB7Ev_KoopaCube | 5428 | 6648 | **+1220** | 38.87 | worst in the unit; still structurally oversized |
| MB7Ev_KoopaWalkUpdate | 5052 | 4976 | −76 | 48.06 | size is now right, bodies are wrong |
| MB7Ev_KoopaFire4Hook | 3448 | 3544 | +96 | 58.24 | 68 B over: see `MB7_FastSqrt` below |
| MB7Ev_KoopaFire3Hook | 3436 | 3532 | +96 | 57.98 | same shape as Fire4 |
| MB7Ev_Hou | 3932 | 3976 | +44 | 68.01 | size right, bodies wrong |
| MB7Ev_KoopaFire1Hook | 3104 | 3176 | +72 | 65.87 | same shape as Fire4 |
| MB7Ev_KoopaFire2Hook | 2732 | 2740 | **+8** | 65.55 | closest big function in the unit |
| MB7_EffDustHook | 2556 | 2576 | +20 | 96.58 | closest of all; see "next" below |
| MB7Ev_DokanIn | 2340 | 2208 | −132 | 47.35 | |
| MB7Ev_KoopaCubeDustEffHook | 1784 | 1812 | +28 | 83.66 | size right |
| MB7Ev_Dokan | 1680 | 2396 | **+716** | 49.75 | second-worst; +716 unexplained |

The four `MB7Ev_KoopaFire*Hook` and `MB7_EffDustHook` /
`MB7Ev_KoopaCubeDustEffHook` are the **highest yield left**: they are all
within 8–96 bytes of the right size, so whatever is wrong is local.

### Hundred-byte band (11 functions, 4,764 B)

`MB7_Create` 636 (63.52), `MB7Ev_DokanSet` 584 (57.76), `MB7Ev_HouPlayerUpdate`
880 (89.69), `MB7Ev_KoopaWalkPlayerUpdate` 744 (84.95), `MB7Ev_DokanPlayerSet`
452 (63.79), `MB7_Kill` 432 (88.89), `MB7Ev_RedResetHook` 612 (57.52),
`MB7Ev_MasuRedInit` 484 (78.69), `MB7Ev_KoopaWalk` 1220 (93.14),
`MB7_ObjectCreate` 1004 (75.84), `MB7_MasuPathCheck` 180 (80.00).

### Tens-of-bytes band (7 functions, 1,076 B)

`MB7Ev_KoopaCubeDustCreate` 152 (50.00), `MB7Ev_KoopaClose` 132 (69.70),
`MB7Ev_KoopaCreate` 124 (84.85), `MB7Ev_KoopaMotionCreate` 124 (64.71),
`MB7Ev_MasuFlagGet` 116 (68.97), `MB7Ev_KoopaWalkDustCreate` 116 (48.28),
`MB7Ev_KoopaWalkCoinLose` 108 (77.78).

These 18 small functions are where the 4 KB is cheapest: none is over its
target size by more than a few bytes, and `MB7_Kill` at 88.89 % and
`MB7_MasuFlagGet` at 68.97 % are pure register/spelling problems of the
kind BRIEF §6 lists answers for.

## Named, measured defects still open

1. **`MB7Ev_KoopaCube` +1220 and `MB7Ev_Dokan` +716.** Both are
   board-event state machines over `mb7_Work` byte offsets. The surplus
   is not inlining any more (that is fixed); it is repeated in-body work
   the original factors into the now-correct `bl` helpers, or loop bounds.
   These are the two functions to take first, and they are the two that
   justify a whole session each.

2. **`MB7_FastSqrt` inline is ~3× too fat.** The original has **no**
   `MB7_FastSqrt` symbol and **no** `bl` to it: the body is inlined at its
   four call sites in the Fire hooks. Target shape per Newton step is
   7 instructions (`lfd 0.5; lfd 3.0; fmul; fmul; fmul; fsub; fmul`), 3
   steps, seeded by `frsqrte f30,f0`, guarded by `fcmpo/ble` against
   `lbl_1_rodata_40` — i.e. the target *does* have the
   `if (x > 0.0f)` test. Our body omits that guard. Measured cost: leaving
   it out-of-line makes the four hooks 208/264/188/188 bytes SHORT; inlining
   it makes them 72/8/96/96 bytes over. So the guard plus whatever else the
   target does is worth roughly 70–90 bytes per hook. A single fix that
   recovers ~280 bytes across four functions.

3. **Four `fabs`/`fmod` where the target calls `MB7_AngleDiff`.** The
   target calls `MB7_AngleDiff` (a global, 180 B, at 0xB3 1C) five times,
   all out of line. Our object calls it once and calls `fabs` four times.
   A previous session hand-inlined the helper body at four sites instead
   of calling it. Same shape as the defect fixed this session, one level
   up.

4. **`.data` symbol order differs.** Target `.data` order is
   `mb7_ObjFileTbl`, `mb7_BackObjFileTbl`, `mb7_BackObjPosTbl`,
   `mb7_BackObjRotYTbl`, `mb7ev_DoorPosTbl`, `mb7ev_KoopaWalkStartPosTbl`,
   `jumptable_1_data_23C`, `mb7ev_DustColTbl`, then the string blobs.
   Ours puts `mb7ev_KoopaMotFileTbl` fourth. Sizes now agree, order does
   not. Cheap to fix when someone is already in the file, and it must be
   fixed before `Matching`.

5. **`.bss` +2.** Target 0x342 vs our 0x344. Not chased. `mb7_SaveWorkP`
   (4), `mb7_MainMdlId` (2), `mb7_MainObj` (4), `mb7_Work` (0x338). Recall
   BRIEF §6: MWCC emits `.bss` in **reverse declaration order**, so this is
   a declaration-order question, not a size question.

6. **`MB7_DistRatioGet` and `MB7_AngleDiff` are global (`T`) in the target**,
   not static. They are at the very end of the target's `.text`
   (0xB260, 0xB31C), after `MB7Ev_MasuFlagGet`. Our source has them at
   lines 243 and 253, near the top. Section-internal order does not affect
   per-function identity but does affect the object.

## Caps encountered

- **`_prolog`/`_epilog`/`ObjectSetup` must be OUTSIDE the `dont_inline`
  region.** With the pragma file-scope, MWCC pushes those two into their
  own `.text` sections and `ObjectSetup` drops from 100 % to 61.90 %.
  Costing one function and 84 code bytes if you get it wrong.
- **`MB7_FastSqrt` must be INSIDE the `dont_inline` region's exception.**
  Covering it emits a symbol the original does not have.
- No `singleton.h` assert anywhere in this unit: no `__assert_fail`, no
  `fn_1_*` assert callee in the target's relocations. That known wall does
  not apply here.
- No `.ctors` and no 8-byte adjustor thunks in this unit: `__sinit` is not
  a problem, so the file-scope `dont_inline` costs nothing in `.ctors`.

## Recommended next session

Take `MB7Ev_KoopaFire2Hook` first (+8 bytes over, 65.55 %) — it is the
cheapest big function in the unit — and use it to characterise the
`MB7_FastSqrt` inline and the `MB7_AngleDiff` call-vs-fabs difference,
both of which then transfer to Fire1/3/4 and `MB7_EffDustHook`.
`MB7_EffDustHook` at 96.58 % / +20 B is the single closest thing to a
promotion in this unit and should be checked for rodata-label churn before
anything else.