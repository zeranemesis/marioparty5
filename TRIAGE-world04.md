# w04Dll/REL/w04Dll/world04 - triage of the remaining 32 functions

Measured 2026-10-03 on branch `decomp/w04Dll` (worktree created from
`decomp/m524Dll` @ 2235890 for this session).

**Where it stands:** 34/66 functions byte-identical, 4576/80608 code bytes
(5.68%), unit fuzzy 51.01%. Start of this session: 33/66, 4500/80608.
Section sizes vs target (`objdump -h`):

| section | target | ours | delta |
|---|---|---|---|
| `.text` | 80608 | 104424 | **+29816** |
| `.rodata` | 608 | 644 | +36 |
| `.data` | 4704 | 4507 | **-197** |
| `.bss` | 32676 | 32770 | **+94** |

Almost none of the 76 KB is data: `.text` is the whole story, and only ~23 KB
of it is real bloat (the rest is the 61.1 KB that objdiff already counts as
matching-but-not-yet-padded).

---

## Ranked list, cheapest first

### Band A - 1 to 4 rows from done. Finish these next.

1. **`MB4_CoinEffInit`** (264 B, 98.30%, 8 rows). Everything matches except
   frame size (target 0x30, ours 0x20) and one dead store the target has and
   we do not:
   ```
   extsh r0, r3
   stw  r0, 0x8(r1)     <- target only, never reloaded
   stw  r0, 0x0(r31)    <- ed->hu3dModelIdx
   ```
   Hypothesis: an `s16` local assigned from the call. **Tested and refuted:**
   declaring `s16 mdlIdx; mdlIdx = (s16) MBEffCreate(...); ed->hu3dModelIdx =
   mdlIdx;` makes MWCC register-allocate it (r26) instead of giving it a
   stack home, and drops `_savegpr_27` to `_savegpr_26` - 11 rows, worse.
   Next thing to try: force a stack home (`volatile`, or address-taken) and
   check whether the home store is then followed by a forwarded register
   store, which is the shape the target has.

2. **`MB4_BubbleEffUpdate`** (504 B, 98.69%, 6 rows) and
   **`MB4_CoinEffUpdate`** (472 B, 97.37%, 9 rows). Both are down to the
   **same single 4-byte instruction**, and it is the reason each is 4 bytes
   short:
   ```
   fcmpo cr0, f1, f0
   cror  eq, lt, eq      <- target has it, we never produce it
   beq   <loop tail>
   ```
   We always emit `ble <loop tail>` instead - same semantics, one
   instruction shorter. Six spellings were tried and all fold to `ble`:
   `if (p->timer > 0.0f)`, `if (!(p->timer > 0.0f))`,
   `for(...; i++, p++) { if (!(...)) continue; }`,
   `if (...) {...} else { continue; }`,
   `s32 dead = !(p->timer > 0.0f); if (dead) continue;` (this one is much
   worse: `mfcr`/`extrwi`/`cntlzw`/`srwi`/`cmpwi`),
   `(p->timer > 0.0f) == 0` (also `mfcr`/`extrwi`/`cmpwi`, 3 rows).
   `cror eq,lt,eq` means MWCC computed `lt|eq`, i.e. the boolean value of
   `a <= b`, rather than branching on it directly. **This is the single
   highest-value open question in the unit: it is worth 2 functions and 976
   bytes, and both are otherwise 100% correct.** Guesses not yet tried:
   the negation applied to a `BOOL`-typed lvalue; the test lifted into a
   `for`/`while` *condition* rather than a body guard; a comma expression.
   `MB4_BubbleEffUpdate` has one more row: the target sign-extends the
   register it just stored (`extsh r0, r0`) instead of reloading
   (`lha r0, 0x0(r31)`) before `cmpwi r0, 0x168`, so the wrapped counter
   must be read into a local at the point of use. `s16 n` local was tried
   and MWCC inserts an extra `extsh r26, r0` before the store (7 rows);
   `s32 n` was tried and drops the `extsh` entirely (5 rows but wrong bytes).
   `p->active++` with no local is the best of the three (6 rows).

3. **`MB4_CoinNearGet`** (296 B, 97.30%, 2 rows). One instruction out of
   place in the prologue: the target does
   ```
   stw   r3, 0x8(r1)      <- pos, spilled
   stfs  f1, 0xc(r1)      <- maxDist, spilled
   mr    r27, r4          <- outPos into a callee-saved register
   ```
   we do the `mr r27, r4` before the `stfs`. Both are 296 bytes; nothing
   else differs. Likely parameter-materialisation order inside the prologue.

4. **`MB4_SakanaOffsetInit`** (180 B, 98.67%, 10 rows) - **capped, pure
   register tie-break.** Byte-identical instruction sequence to the
   already-100% `MB4_SakanaPosInit` except the target puts the inner-loop
   running pointer in r31 and the outer species counter in r30, while we
   get the exact reverse. Both declaration orders (`u8 *base; s32 species,
   fish;` and `s32 species, fish; u8 *base;`) compile to the same bytes -
   declaration order is not the lever here, unlike most register ties.

5. **`MB4_CoinEffCreate`** (468 B, 96.92%, 38 rows) - one structural cause
   left. Four of five fixed this session; the residue is that `color` gets
   r30 where the target has r23, which shifts every other home down one.
   Allocation order target: p, mgr, i, vel, pos, ed, obj, temp, color.
   Ours: p, color, mgr, i, vel, pos, ed, obj - i.e. **only `color` is in the
   wrong slot**, everything relative to it is already right. Two declaration
   reorderings did not move it. Worth one more attempt at
   `HU3DMODEL *model;` as a real local (the target loads the `&Hu3DData[..]`
   pointer into r24, which could be a live local rather than a temp) - that
   would add a slot and may re-order the tail.

### Band B - 100 to 700 bytes, one structural cause each.

6. **`MB4_BubbleEffCreate`** (760 B, 98.42%, size -4) - *not yet diffed.*
   Same -4 signature as Band A item 2; check whether it is the same `cror`.
7. **`fn_1_C6D0`** (1016 B, 95.11%, +28) - *not yet diffed.*
8. **`PathCheck`** (1424 B, 81.22%, -76) - *not yet diffed.*
9. **`MB4_HermiteCalcV`** (1216 B, 76.86%, -104) - suspect
   `MB4_HermiteCalcAxis`: our object has a 340-byte out-of-line copy with
   **zero `bl` relocations against it**, i.e. it is already inlined at
   every call site and the copy is dead. The target has no such symbol. Same
   for `MB4_SakanaPartBase` (72 B, no callers) and `MB4_CapsuleBubbleBurst`
   (900 B, no callers). That is 1312 bytes of dead `.text` in our object
   that the target does not have; removing them is pure win but will not by
   itself make any function match.
10. **`fn_1_DC9C`** (1448 B, 65.30%, -220), **`fn_1_DB68`** (308 B, 60.31%,
    -52), **`MB4_BezierCalcV`** (284 B, 77.86%, -60), **`MB4_LightSet`**
    (280 B, 67.87%, -88) - all *smaller* than target, so these are missing
    computation rather than bloat.
11. **`MB4_AngleLerp`** (592 B, 88.09%, +64), **`fn_1_D3D8`** (360 B,
    81.10%, -40), **`MB4_CoinCreate`** (664 B, 76.15%, +40),
    **`MB4_SakanaCreate`** (288 B, 68.65%, +44).

### Band C - the four multi-kilobyte "missing" functions. 22 KB, and the
### structural cause is known.

`objdiff` reports a function as **missing**, not as a mismatch, when the
rebuilt one is grossly oversized. All four of these exist in our object and
are simply 2-3x too big; once they are in the right size range objdiff will
score them and the fuzzy number will move a lot even before they match:

| fn | target | ours | delta |
|---|---|---|---|
| `MB4_CapsuleBubbleUpdate` | 6028 | 18804 | +12776 |
| `MB4_CapsuleEffHook` | 5612 | 11316 | +5704 |
| `MB4Ev_Iruka` | 5640 | 10872 | +5232 |
| `MB4Ev_IrukaBubbleCreate` / `MB4Ev_PlayerBubbleCreate` | 456 ea. | 1080 ea. | +624 ea. |

`MB4_CapsuleBubbleUpdate` is the best single target in the unit: +12776
bytes, 12% of everything left. **Start by proving whether
`MB4_CapsuleBubbleBurst` (900 B, currently a dead out-of-line copy) is
inlined into it** - if so that accounts for a chunk of the delta and tells
you the original had no such helper at all.

### Band D - large, wrong, not yet diagnosed.

`MB4Ev_Kaisoo` 7744/8076, `MB4Ev_Kihon` 8360/11144, `MB4_Create`
6448/7056, `MB4Ev_Blow` 4752/5152, `MB4_SakanaInit` 2988/3476,
`MB4_SakanaDraw` 5892/4632, `MB4_CoinUpdate` 2604/2348,
`MB4_MainObjUpdate` 2664/2816, **`MB4_PathTimeGet` 5564/424** (frame
`0x300` in the target against a 424-byte body in ours - this one is simply
not decompiled).

---

## Reusable findings from this session

1. **`#pragma dont_inline on` around a helper kills a silent inline.**
   MWCC inlines any function defined in the same TU. `MB4_PathTimeGet` was
   being inlined into the forwarder `MB4_PathNextTimeGet`, giving a 408-byte
   function against a 76-byte target. This is invisible in the source and
   easy to miss because the helper still gets its own out-of-line symbol too.
2. **Argument evaluation order in a call is the parameter DECLARATION
   order**, and the ABI can hide it. `MB4_PathTimeGet(f32 tMax, Point3d*,
   Point3d*, Point3d*, Point3d*)` and `MB4_PathTimeGet(Point3d*, Point3d*,
   Point3d*, Point3d*, f32 tMax)` generate the *same* register assignment
   (f32 -> f1, four pointers -> r3..r6 in order) but the caller's code
   materialises the arguments in different order. Read the callee's
   parameter order off the caller's prologue.
3. **Cache the `OMOBJ` pointer in a local when a function touches
   `global->data` before and after a loop.** Both `MB4_CoinNearGet` and
   `MB4_CoinEffCreate` go from "rematerialise the global" to matching the
   target the moment the object pointer is kept live in a callee-saved
   register. This is the single most reusable structural fix found: it took
   `MB4_CoinNearGet` from 53.95% to 97.30% and `MB4_CoinEffCreate` from
   59.66% to 96.92%.
4. **Copy a struct with one assignment, not field by field.**
   `*outPos = *(Point3d *)p;` gives MWCC's 3-word `lwz`/`stw` block copy;
   `outPos->x = ...; outPos->y = ...;` gives three `lfs`/`stfs` float moves.
   The target always uses the block copy.
5. **A float "greater than" guard that skips to the loop tail wants
   `cror eq,lt,eq` + `beq`, not `ble`.** We have not found the spelling
   that produces it - see Band A item 2. Worth solving once, then apply
   everywhere.
6. **Two adjacent halfword fields are zeroed by two statements.**
   `MB4_CoinEffCreate` zeroes `p->field2` then `p->active` (`sth 0x2` then
   `sth 0x0`), not `p->active` alone.
7. `objdiff` "missing" != "not written". Six functions here were listed as
   missing purely because the rebuilt body was oversized.

## Data sections (`.data` -197, `.bss` +94)

`.data` is only 4704 bytes so it cannot be the reason 76 KB is missing, but
both sections are wrong and will block promotion even after `.text` lands.

* `.bss` is **missing `mb4_SakanaObj`** (4 bytes) entirely, and
  `mb4_Work` is **252 bytes too large** (`0x32636` target, `0x32732` ours).
* `.bss` declaration order is wrong. The target puts every scalar first -
  `mb4_MainObj`@0, `mb4_CoinObj`@4, `mb4_CoinEffObj`@8,
  `mb4_BubbleEffObj`@0x12, `mb4_SakanaObj`@0x16, `mb4_CoinMaxDist`@0x20
  (0x12 bytes), `mb4_PathValidF`@0x32, `mb4_PathLen`@0x34,
  `mb4_PathMasuNo`@0x36 - and `mb4_Work` **last** at 0x40. Ours puts
  `mb4_Work` third at 0x28 and the scalars above 0x32760. This is the
  BRIEF reverse-declaration-order rule: `mb4_Work` must be the last static
  declared.
* We have an `sp14_unused` (4 bytes at `.bss` 0) with no counterpart in the
  target.
* `.data` is 197 bytes short and the table order does not match: the target
  has `mb4ev_IrukaPath` at `.data` 0x00 and `mb4ev_SakanaTbl` at 0x1848;
  ours has them at 0x1732 and 0x3788. Apply the BRIEF data recipe
  (forward-`extern`, data before the functions, `(u32)&sym` casts, declare in
  ascending target offset). The 18-byte `Point3d` groups - `lbl_1_data_10CC`
  and the thirteen 0x12-byte entries from 0x4312 to 0x4468 - need explicit
  `Point3d[N]` declarations rather than being spelled out field by field.
* `gap_04_*_data` and `gap_05_*_bss` in the target are alignment holes with
  no source origin; ignore them.

## Toolchain wall, proven

* `MB4_SakanaOffsetInit` - pure r30/r31 tie-break, two declaration orders
  tried, byte-identical output both times.
* `MB4_CoinEffCreate` - `color` is allocated one slot too high; two
  declaration reorderings did not move it.
* The `cror eq,lt,eq` form in `MB4_BubbleEffUpdate` / `MB4_CoinEffUpdate` -
  six spellings tried, all fold to `ble`. Not yet proven unclosable.

## Not applicable here

No `singleton.h` assert pattern was found in this unit, and there is no
`.ctors`/adjustor-thunk problem: `world04` has no C++-generated thunks.