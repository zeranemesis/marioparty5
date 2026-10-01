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

## Finding 5: a full 32-bit base defeats displacement folding

`dolphin/os/OSAudioSystem` shows the other side of finding 3. The target
keeps the complete pointer `0xCC005000` in a register and reaches every
field with a small displacement:

```
addi r3, r3, 0x5000     ; r3 = 0xCC005000, kept live
sth  r0, 0x12(r3)       ; cpucode
addi r31, r3, 0xa       ; r31 = &init_vector2, kept for the rest
lhz  r0, 0x0(r31)       ; ...
```

Written with a struct pointer and a `volatile u16 *` field pointer, MWCC
instead collapses the base to the `lis` half and rebuilds the offset on
every access, saving 3 callee-saved registers instead of 7:

```
lis  r3, 0xcc00
lhz  r0, 0x500a(r3)     ; instead of lhz r0, 0x0(r31)
sth  r0, 0x500a(r3)
```

Semantics and instruction count are identical (125 rows either way); only
the register assignment differs. Three things that did **not** recover it:

- Accessing fields through `regs->field` rather than a saved pointer: worse
  (24% vs 27%), because every access re-derives the address.
- Hoisting the `vec` assignment before the first struct store: no change.
- Adding explicit locals for the three mail words: pushed the count to 126.

So this is a genuine allocation difference, not a missing local: the counts
matched while the prologue still saved 3 registers against the target's 7.
Treat "the base is materialised once and reused" as a property to check in
the target prologue (`stmw r25` = 7 saved) rather than something to force
from the source.

## 6. DCE on an unused call parameter (OSMemory)

`MEMIntrruptHandler` computes `flags` and passes it to the error handler, but the
target never uses it afterwards, so the current compiler keeps a dead store that
MWCC removed:

```
rlwimi r0, r4, 0x1f, 0x1f, 0x1f     ; flags (kept in the target)
add    r5, r5, r6
```

Rewriting the call so the value has to survive (`__OSErrorHandler(..., flags)`
instead of a local that is dropped) restores both instructions. When a target
keeps a computed value that "cannot matter", the value must be reaching the
callee through a real argument rather than through dead local storage.

## 7. Pointers are 4 bytes wide inside struct layout (MWCC 2.3.3 / GC 1.2.5n)

This is the big one. In struct layout this compiler gives every pointer member a
size of **4**, while the generated code still handles pointers as full 64-bit
values. Struct fields therefore overlap:

```c
typedef struct OSStopwatch {   /* sizeof == 0x2C */
    char* name;   /* 0x00, next member starts at 0x04! */
    u32    hits;  /* 0x04 */
    OSTime total; /* 0x08 */
    OSTime min;   /* 0x10 */
    OSTime max;   /* 0x18 */
    OSTime last;  /* 0x20 */
    BOOL   running; /* 0x28 */
} OSStopwatch;
```

Verified directly with `mwcceppc.exe` (GC/1.2.5n) on a probe file: `hits` lands at
0x04 behind a `char*`, and a struct `{char* name; void* other; u32 hits;}` puts
`other` at 0x04 and `hits` at 0x08. `s64` members keep their 8-byte alignment, so
only pointer-then-small-field sequences are affected.

Consequences for this repo:

- Structs copied from `extern/aurora` (compiled by a host 64-bit compiler) have
  the wrong offsets and must be corrected field by field against the target
  before the unit can match.
- Field *order* must be reproduced verbatim; "fixing" the overlap (padding the
  pointer out to 8 bytes) silently changes every following offset.
- Every access still uses 32-bit `lwz`/`stw`: the game only ever needs the low
  half of a pointer, which is why this layout has never been noticed.

## 8. String pools are padded to 4 bytes per literal, and the section to 8

MWCC emits each string literal padded up to a 4-byte boundary into a single
`.data` pool, and references them as one base symbol plus an immediate offset
(`lis r4, pool@ha` / `addi r29, r4, pool@l` / `addi r3, r29, 0x38`). The pool's
size is *not* rounded up to the section alignment: a pool whose literals end at
0x71 gives a 0x71-byte section, while the target has 0x78. Spelling the trailing
padding out inside the last literal (`"...\n\0\0\0\0\0\0\0"`) reproduces the
target's section size byte for byte.

## 9. MWCC 2.4.7 spill placement, and how to force it (NMWException)

`__construct_array` keeps `ptr`, `dtor` and its loop counter in the frame and
reloads them around every indirect `bctrl`, while promoting only four values
(`p`, `sz`, `n`, `ctor`) into r28-r31. Written plainly, the same compiler keeps
seven values in r25-r31 and spills nothing: it only spills when the frame is
forced, because with enough callee-saved registers available it prefers
promotion over spilling.

The tell-tale in the target is a store immediately followed by a reload of the
same slot (`stw r0,0x18(r1); lwz r4,0x18(r1)`) with no use in between — a
register allocator would never emit that for a spilled-but-promoted value.
Declaring the cold parameters and the counter `volatile` reproduces the
placement and the register budget, taking the unit from 84.8% to 91.4%.

So: when a target reloads a variable around every call, `volatile` is the lever,
and it is worth trying on parameters, not just locals — but only on the values
the target keeps in memory. Marking `sz` and `n` volatile as well makes things
worse (they lose their register copies), which is how the right subset was found.

## 10. Correcting a struct offset in OSMemory was not a codegen problem

`__OSInitMemoryProtection` looked like the displacement-folding case of finding 5,
but the offsets were simply wrong. The target writes the block at 0xCC004000 with
displacements 0x20, 0x10 and 0x28, while the struct in `OSMemory.c` placed those
fields at 0x22, 0x10 and 0x2B — a two-byte shift introduced by a padding array
that was one element too long. `MEMIntruptHandler`, which addresses the block
with raw `MEM_BASE + 0x1E/0x20/0x22/0x24` offsets, confirmed the real layout.

Lesson: when a base pointer is folded, compare the *absolute* address first.
`(lis 0xcc00) + 0x4020` and `(lis 0xcc00; addi 0x4000) + 0x20` look like a
codegen difference but can be a field-offset bug, and the match percentage does
not move when only the offsets shift. Four further encodings of the constant
(`0xCC000000 + 0x4000`, `0xCC00 << 16`, a `(u32)` cast, raw offsets instead of
the struct) all left the function at 84.3%, so the folding itself is still
unexplained.

## 11. An inlined wrapper loses the callee-saved copy of its result

`voiceBlock` in `musyx/runtime/synthvoice.c` was the last function of its unit to
match. Written the obvious way -- `if ((voice = voiceAllocate(...)) != -1)` --
MWCC inlines the whole `voiceAllocate` / `voiceAllocateDo` chain and leaves the
inlined argument in the register the call returned in (`r3`), while the
caller's own `voice` is allocated `r31` for the second half of the function.
The two ranges are the same value, so the allocator coalesces them and the
result is correct, but `voice` is read out of `r3` in the first block and out of
`r31` in the second, with no `mr r31, r3` anywhere.

The target has the `mr r31, r3`: the value crosses a real (if inlined) call
boundary and gets a callee-saved home before the argument is substituted.
Writing the two calls out by hand reproduces it exactly:

    voice = voiceAllocateFind(prio, 0xFF, 0xFFFF, 1);
    voiceAllocateDo(voice, 1);
    if (voice != 0xFFFFFFFF) { ... }

Lesson: when only the *registers* of an otherwise identical body differ, suspect
inlining shape rather than types or declaration order. Roughly a dozen variants
that varied the parameter type, the return type, the local's type, the
comparison constant, the name of the inlined parameter and the ternary form all
produced the same split live range; only splitting the wrapper into its two real
calls changed anything. Note also that changing the inlined parameter from `s32`
to `u32` makes MWCC fold the callee's `if (voice != -1)` guard into the caller's
`voice != 0xFFFFFFFF` test and delete the duplicated check, which is a quick way
to tell whether a guard is still there.

## 12. `0xFFFFFFFF` is sometimes compared as `x + 0x10000 == 0xFFFF`

The same function tests its result twice: once with `cmpwi r, -1` and once, at
the merge point, with `addis r0, r, 1; cmplwi r0, 0xffff`. The second form is
the first one after the finding-5 fold (`0xFFFFFFFF` expressed as
`0x10000 - 1`), and MWCC cannot see that the two are the same test, so it emits
both. That is why the target keeps a redundant-looking guard after the inlined
block, and why changing the inlined parameter's signedness (which changes the
form of *its* guard) can delete it.

## 13. `gap_*` symbols are a build artifact, not something to reproduce

Most target objects end their `.sbss` / `.sdata2` sections with a symbol named
`gap_10_8028xxxx_sbss`, sized to round the section up to its alignment. It
carries a build address, is present in the linked objects of units already
marked `Matching` (for example `dolphin/os/OSArena` and `dolphin/demo/DEMOInit`),
and cannot be produced from C: no compiler flag or source change emits it. It
costs a few percent on the small-data sections of 181 sections across the ROM and
should be ignored when judging whether a unit matches.

## 14. `sqrtf`'s statics: a whole unit's `.sdata2` hinges on 16 bytes

`main/game/board/capmain` sits at 91.4% of `.text` with about 140 functions short
of matching, and almost every one of them is short for the same reason: a single
`lfs`/`lfd` whose relocation points 16 bytes too low. Comparing the two
`.sdata2` sections as sequences of 4-byte words shows the pools are *identical*
apart from four words that the target does not have at the front, so every
constant after them is at the wrong address and every reference to it mismatches.

Those four words are the two doubles `0.5` and `3.0` that
`libc/math.h`'s `sqrtf` holds in function-local statics:

    static const double _half = .5;
    static const double _three = 3.0;

MWCC emits them at offset 0 of `.sdata2`, before everything else, and a second
copy of the same pair survives further down the pool as two dead literals that
no instruction references. The target has only one pair, at the position where
the dead copies sit, and `sqrtf` reads it from there: MWCC deduplicated the
statics against the literals that were already in the pool.

Replacing the statics with plain literals in `sqrtf` reproduces that
deduplication exactly, and the result is not marginal:

| section | statics | literals |
| --- | --- | --- |
| `.sdata2` | 99.38% | **100%** |
| `.text` | 91.43% | **99.29%** |

It cannot be taken, though. `ninja` drops from `135 files OK` to `113 files OK`
with 22 module checksums failing, because MWCC also emits the statics of an
`extern inline` function that the translation unit never instantiates. In
`w01Dll/REL/w01Dll/world01.o` those four dead words are the entire difference
between a `.rodata` of 0x244 bytes (matching) and 0x234 (not), and the target
has the 0x244 version. Moving the declarations to file scope or giving them
external linkage does not help: both still drop the unused case, and both leave
`capmain` perfect and the 22 modules broken.

The two requirements are therefore mutually exclusive, and the reason is visible
in the toolchain rather than the source -- `capmain` is built with MWCC 2.6 and
`world01` with MWCC 1.3.2, and only one of the two deduplicates. Since a single
`libc/math.h` has to serve every unit, the global gate wins and the statics stay.
Worth remembering when a unit is *almost* matching: a whole section can be
correct except for a handful of leading bytes, and the giveaway is that every
mismatch is a relocation rather than an opcode. Dumping a section with
`objdump -s`, stripping the address and ASCII columns, and diffing the raw hex in
8-character chunks finds this in seconds where per-function objdiff output does
not.

The same four words sit at the front of `.rodata` in 77 sections across the ROM
(`tools/sectdiff.py --all --offending` lists them), always as the `0.5`/`3.0`
pair and always in units that inline `sqrtf` -- `resultDll/REL/resultDll/stage`,
`partyresultdll/REL/partyresultdll/toueidisp`, `m526Dll/REL/m526Dll/math` and
`mdpartyDll` among them. Inside a single module the two forms are needed at
once: `m526Dll/math` wants the statics gone while `m526Dll/m526` wants them
present, so the switch cannot be made per module either.

## Finding 15 -- MWCC emits functions in *reverse* declaration order

`.text` is not laid out in source order. `Runtime.PPCEABI.H/NMWException.o` is
the clean example: the target orders it

```
__destroy_arr, __construct_array, __dt__26__partial_array_destructorFv, __throw_catch_compare, ...
```

while a straightforward reading of the source (`__throw_catch_compare`, then the
class, then `__destroy_arr`, then `__construct_array`) compiles to the exact
reverse. Moving the `__construct_array` definition to *before* `__destroy_arr`
flipped the emission order to match and took `.text` from 93.02% to 94.60% --
and, more importantly, `extab` from 59.26% to 98.81% and `extabindex` from
92.86% to **100%**.

The reason the gain is so lumpy is that `extabindex` is a *sorted* table of
`(function, text offset, extab entry)` triples. One function out of order moves
every subsequent offset, so a single misordered definition invalidates the whole
section. Whenever a unit is close on `extab`/`extabindex` but not on `.text`,
check the emission order before touching any code: reordering two whole
functions is a much cheaper fix than any local expression change, and it is the
only kind of fix that can move `extabindex` to 100%.

Corollary worth remembering: `dtk elf disasm` labels functions in emission
order, so comparing `grep '^\.fn '` output against the reference `.s` detects a
misordering immediately, without objdiff.

## Finding 16 -- `extab` is an oracle for local-object layout

`__construct_array` resisted every `volatile` permutation and sat at 56.45%. The
target's `.text` showed five consecutive words at `0x8(SP)`, and the `extab`
entry for the function named the reason:

```
PC=00000060, Action: 000010
  Type: DESTROYLOCAL
  Local: 0x8(SP)
  Dtor: "__dt__26__partial_array_destructorFv"
```

So the original does not hand-roll the cleanup: it declares a
`__partial_array_destructor` local, runs the construct loop against `d.i`, and
lets the compiler inline the destructor at scope exit. That single change --
dropping all the `volatile`s and using the object -- reproduced the frame layout
exactly (`p, size, n, dtor, i` at `0x8`..`0x18`), and required changing the
class constructor from `i = n` to `i = 0` so the counter starts at zero and
reaches `n` through the loop. Semantics are unchanged: on normal exit `i == n`,
so the inlined destructor is a no-op and only the exception path uses it, which
is exactly what the `PC=0x60` action records.

The lesson generalises: when a function is stuck because values keep landing in
the wrong registers, do not reach for `volatile` first. Dump the *data* sections
and ask what the compiler thought the frame contained. `extab`, `extabindex` and
`.eh_frame` encode the frame layout, the saved-GPR range, and the cleanup objects
by name -- free structural information that the instruction-level diff never
shows. `volatile` was a plausible-sounding guess that cost several iterations and
produced nothing.

`__construct_array` still stops at 94.60%: the target assigns `n` to `r31` and
the walking pointer to `r30`, while MWCC picks the opposite. Sixteen
reformulations (declaration order, `register`, per-parameter `volatile`, index
arithmetic, do/while, for-loop, pointer-as-parameter) all reproduced the same
94.60%, so the ordering is decided inside the allocator and is not reachable from
the source. That is a real MWCC behaviour rather than a misguess, and it is a
better stopping point than the `volatile` version was.

## Finding 17 -- an inline virtual in a header emits an out-of-line copy in every TU that sees it

`Gecko_ExceptionPPC.o` reported `.text` at 100% while being 0x54 bytes too long,
and the symbol table said why:

```
target: __dt__Q23std9exceptionFv      *UND*      what__Q23std9exceptionCFv   *UND*
mine:   __dt__Q23std9exceptionFv      weak, .text  what__Q23std9exceptionCFv weak, .text
target: __vt__Q23std9exception        *UND*
mine:   __vt__Q23std9exception        weak, .data
```

`std::exception` in `include/PowerPC_EABI_Support/Runtime/exception.h` defined
`virtual ~exception() { }` and `virtual const char* what() const { ... }` inside
the class, so MWCC emitted an out-of-line copy of each in every translation unit
that includes the header -- even though this unit only ever instantiates
`bad_exception`, which overrides both. Declaring them and leaving the
definitions to the library removed both copies, took `.text` from 0x148c to
0x143c against a target of 0x1438, and moved `extabindex` from 87.14% to 99.29%
and `.data` from 96.67% to **100%**.

The lesson is about *which* symbol to look at. objdiff scores `.text` by byte
coverage, so a section that is 4 bytes too long can still read 100%: every
target byte is matched and the surplus is invisible in the percentage. Always
compare `objdump -h` sizes next to the match percent, and when they disagree
read the symbol table before reading any disassembly -- a symbol the target
leaves undefined but the build defines is a header-level decision, not a
codegen puzzle. `~bad_exception` is the tell: it grew by exactly one `bl`, the
call to the base destructor that should have been inlined away.

There is a real trade-off here, and it is not one-sided. Keeping the destructor
inline and only declaring `what()` leaves `~bad_exception` byte-identical to the
target (23 instructions, correct `mr.`/vtable stores) but keeps
`__dt__Q23std9exceptionFv`, which drags `extabindex` back to 87%. Declaring both
drops the extra symbol and wins on every data section. Chasing per-function
perfection first and unit-level score second would have picked the wrong one:
`.text` says 100% for the variant that is further from matching overall.

## Finding 18 -- a section can be too *small* and still score below 100%: look for `gap_` symbols

`Gecko_ExceptionPPC.o` still reports `.bss` at 75%. The target's `fragmentinfo`
occupies 12 bytes and the section is 16, so four bytes were missing and adding a
fourth field to `ProcessInfo` did reach `0x10` -- while dropping the section to
**37.5%**, because objdiff compares byte for byte and the two extra bytes are not
the two the target has.

The arithmetic in `__unregister_fragment` settles it:

```
mulli r4, r3, 0xc        ; stride is 12, so ProcessInfo is 3 words, not 4
```

and the symbol table then names the remaining four bytes outright:

```
target: 0000000c g  O .bss 00000004 .hidden gap_08_8025B29C_bss
mine:   (nothing)
```

`gap_*` is finding 13's family: an artefact of the original build, not something
the source can produce. It cannot be matched, and chasing it by adding struct
fields actively makes things worse, because the fabricated bytes score as
mismatches where absent bytes do not.

So `.bss`/`.data`/`.rodata` have two distinct failure modes that look identical
from the percentage alone. **Too large** is usually a real problem -- a surplus
symbol, a mis-sized array, a duplicate pool entry -- and worth chasing. **Too
small** with a `gap_`/`pad_` symbol sitting in the target is not: check
`objdump -t` for those names before touching the struct. The 75% here is 12 of
16 bytes matched, and the missing 4 are unreachable.

This is the second time in this session that reading symbols beat reading code.
The first was `__vt__Q23std9exception` being `UND` (finding 17); here it is a
`gap_` name that says "stop". Both were one `objdump -t` away, and both cost
several wrong source edits to find by disassembly alone.
