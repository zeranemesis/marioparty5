# MWCC codegen notes (assembled from agent findings)

Hard-won facts about how Metrowerks CodeWarrior generates code for this
project, collected from decompilation work. These apply across units, not just
the one that produced them. Every rule here was verified against a diff.

Two flag sets matter and they behave differently:

| unit kind | flags | register allocation |
|---|---|---|
| DOL (`game/`, `dolphin/`, `msm/`, `musyx/`) | `-O4,p` | optimised, aggressive inlining |
| REL overlays (`src/REL/*`) | `-O0,p` `-char unsigned` | nearly literal, declaration-driven |

---

## Chained assignment is not the same as a shared local

The single highest-value finding for `-O0,p` float code. Confirmed on
`m510dll/opening` `fn_1_966C` (1184 bytes, 59 differing rows -> 4).

When a value is written to two fields of the same object:

```c
/* WRONG: a named local pins the value into a callee-saved FPR (f31),
   and each read is re-emitted. */
rate = view3->offset;
slide->x = rate;
slide->from.x = rate;

/* RIGHT: one expression, one load, two stores, volatile temporary f0. */
slide->from.x = slide->x = view3->offset;
```

| source shape | emitted |
|---|---|
| shared `f32` local | `lfs f31, ...` then two `stfs f31, ...` |
| chained assignment | `lfs f0, ...` then two `stfs f0, ...` |

This is the same distinction as R4 in [mwcc_temp_allocator.md](mwcc_temp_allocator.md):
a named local is a store into a **callee-saved** register, an expression
temporary is **volatile**. `f0` in the original is a temporary, so the source
cannot have used a local.

**The chain order is observable.** The target stores `0x18` (the scalar) before
`0xC` (the `Vec` member), i.e. `slide->from.x = slide->x = v`, not the reverse.
Getting the chain backwards costs 2 rows per pair and no more.

The same applies to a lerp written twice:

```c
slide->from.x = slide->to.x = view3->offset + rate * (target - view3->offset);
```

Writing it as two statements forces a `lfs` of the just-stored field.

## Pointer locals: a two-step `addi` means the base is a real local

`addi rX, r30, 0x1c` followed by `addi rY, rX, 0x8` is not how MWCC folds
`&work->view.obj`. Given the struct directly it emits a **single** `addi`
with the offset summed (`addi rX, r30, 0x24`). The two-step form only appears
when the base is a separate named local:

```c
M510_VIEW *slot0 = &work->view0;      /* addi r22, r30, 0x1c  */
M510_VIEWOBJ *view0 = &slot0->obj;    /* addi r27, r22, 0x8   */
```

So a two-step `addi` in the target is a *structural* fact about the original
source: the base pointer was named. Do not try to write the offset by hand.

## Getting the callee-saved allocation right

Three levers, in the order worth trying.

1. **Count the locals.** The prologue uses a fixed set of callee-saved
   registers and nothing is reused: every pointer that is assigned in the
   prologue occupies its own register. If the target uses r19..r29 that is 11
   distinct named locals, and one too few puts the whole prologue one register
   low. Add the missing one rather than reshuffling.
2. **Only live locals are allocated.** A local that is declared and never read
   spills to the stack instead of taking a register (see note 2 above). A local
   that is assigned but never read still costs a spill. This is the lever for
   *adding* a register consumer: read the value somewhere to make it live.
3. **Interleaving matters, and it is not a sort.** Order is not "declaration
   order" in any simple sense. On `fn_1_966C` the working order was

   ```c
   M510_SLIDE *slide  = &work->slide;
   M510_VIEW  *slot0..slot3 = &work->view0..3;   /* r22,r21,r20,r19 */
   M510_VIEWOBJ *view3 = &slot3->obj;            /* r29 */
   s32 i;                                        /* r28 */
   M510_VIEWOBJ *view0 = &slot0->obj;            /* r27 */
   M510_VIEWOBJ *view1 = &slot1->obj;            /* r26 */
   M510_VIEWOBJ *view2 = &slot2->obj;            /* r25 */
   M510_SLOT *slot;
   f32 rate;
   s32 result;
   ```

   with `view3` and `i` declared *before* `view0`. The natural reading order
   gives r28/r27/r26/r29 instead and costs 4 rows.

**Do not brute-force this.** A 500-permutation search over the declaration
orders of this function did not beat the hand-derived order above, because the
winners were not permutations at all - they changed *which* locals exist. Fix
the body first, then the count, then the interleaving; search last.

## Postfix increment on a struct field

`lwz r3, 0x14(r30); addi r0, r3, 1; stw r0, 0x14(r30); cmpwi r3, 0x5a` is

```c
if (work->frame++ > 0x5A) { ... }
```

The named-local spelling (`frame = work->frame; work->frame = frame + 1;
if (frame > 0x5A)`) reloads into a callee-saved register and costs 4 rows per
occurrence. There are four of these in one function; the postfix form is worth
more than any allocation tweak.

## Redundant casts on already-narrow fields

`(u16)work->phase != 0` on a `u16` field emits a stray `clrlwi`. The plain
`work->phase != 0` is the original's spelling. Same for casts to the field's
own type generally: check the field width before adding a cast.

## Method that worked, in order

For each of the four findings the productive step was to read the **target's
instruction shape** and ask what C produces it, not to try source variants:

1. `tools/unit_status.py <unit>` for the function list.
2. `python tools/m2c_draft.py <unit>` for a draft (never copy it).
3. `python tools/fndiff.py <unit> <fn> --all` and read the **whole** `!` list,
   not the summary. Group the rows: prologue / arithmetic / stores.
4. For each group, identify the *shape* of the target instruction stream
   (`stfs` pairs, two-step `addi`, postfix counters) and derive the C from it.
5. Re-run `fndiff` after each change and keep the best version. Save a copy
   before experimenting: several plausible-looking changes are regressions.

Getting from 77% to 98% took four shape-reading passes. The last 4 rows were
the emission order of four address computations, which no source rewrite
recovered - that is what `Equivalent` is for.
